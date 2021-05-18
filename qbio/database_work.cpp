#include "database_work.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QApplication>
#include <QMessageBox>

#include <QSqlError>
#include <QDebug>

#define QUERY_CHECK \
    if (query.lastError().isValid())\
    {\
        QString binds;\
        foreach(QString v, query.boundValues().keys()) \
            binds += v + " = " + query.boundValues()[v].toString() + ";  "; \
        qDebug() << "** Error in query: " << query.lastQuery(); \
        qDebug() << "** Binds: " << binds; \
        qDebug() << "** Error text:";\
        qDebug() << query.lastError().databaseText(); \
        qDebug() << query.lastError().driverText(); \
        return false; \
    }

#define DB_CHECK(db) \
if (db.lastError().isValid())\
{\
    qDebug() << db.lastError().databaseText(); \
    qDebug() << db.lastError().driverText(); \
    return false; \
}


database_work* database_work::database = nullptr;
database_work::database_work()
{
    if(!open_database())
    {
        QMessageBox::critical(0, "Error", "The program will be closed. Unable to connect to database.");
        QApplication::exit(-1);
    }

    // если таблиц бд не существует -> создаем
    if(!create_database_tables())
    {
        QMessageBox::critical(0, "Error", "The program will be closed. Unable to create database.");
        QApplication::exit(-1);
    }
}

database_work* database_work::GetInstance()
{
    if(database == nullptr){
        database = new database_work();
    }
    return database;
}

bool database_work::open_database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("biosphere_simulation.database");
    db.setHostName("localhost");

    if(!db.open())
    {
        DB_CHECK(db);
        return false;
    }       

    return true;
}

bool database_work::exist_db()
{
    return QFile::exists(db.databaseName());
}

bool database_work::create_database_tables()
{
    QSqlQuery query(db);

    query.exec("PRAGMA foreign_keys = true;");
    QUERY_CHECK;

    query.exec("create table if not exists bio_species"
               "(id INTEGER primary key AUTOINCREMENT"
               ",name nvarchar(20)"
               ")");
    QUERY_CHECK;

    query.exec(" create table if not exists bio_piece"
                "(id INTEGER primary key AUTOINCREMENT"
                ",species INTEGER unique"
                ",alive INTEGER default 1"
                ",lifespan INTEGER);");
    QUERY_CHECK;

    query.exec(" create table if not exists bio_statistics"
                  "(species INTEGER"
                  ",q_alive INTEGER"
                  ",q_dead INTEGER"
                  ",avg_lifespan INTEGER"
                  ",q_tics INTEGER);");
    QUERY_CHECK;

    return true;
}

bool database_work::add_info_db(QString outer_select)
{
    QSqlQuery query(db);
    query.prepare(outer_select);
    if(query.exec())
        return true;
    return false;
}

void database_work::close_db()
{
    db.close();
}

//основная логика работы с бд
void database_work::DB_connectNwrite(QString outer_select)
{
    // бд существует -> добавляем записи
    if(!add_info_db(outer_select))
    {
        QMessageBox::critical(0, "Error", "The program will be closed. Unable to edit the database.");
        QApplication::exit(-1);
    }
//    close_db();
}

void database_work::DB_delete_all(QSqlDatabase &db)
{
    QSqlQuery del_query(db);
    del_query.exec("use master; drop database biosphere_simulation;");
}

int database_work::save_species(species_ptr sp, const species_stat_entry &st)
{
    QSqlQuery query(db);

    if ( sp->get_id() == 0)
    {
        query.prepare("INSERT INTO bio_species (name) VALUES (:name);");
        query.bindValue(0, QString::fromStdString(sp->get_name()));
        int id = query.exec();
        QUERY_CHECK;
        sp->set_id( id );
    }

    {
        QSqlQuery query(db);

        query.prepare("INSERT OR REPLACE bio_statistics (species, q_alive, q_dead) "
                  " VALUES (:sp, :alive, :dead);");
        query.bindValue(0, sp->get_id());
        query.bindValue(1, st.count);
        query.bindValue(2, st.dead);
        query.exec();
        QUERY_CHECK;
    }

    return true;
}