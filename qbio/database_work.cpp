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
        QMessageBox::critical(0, "Error", "TCurrrent universe will not be saved. Unable to open the database.");
    }

    // если таблиц бд не существует -> создаем
    if(!create_database_tables())
    {
        QMessageBox::critical(0, "Error", "Currrent universe will not be saved. Unable to edit the database.");
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

QStringList database_work::get_species()
{
    QSqlQuery query(db);

    query.exec(" select species, q_alive, q_dead from bio_statistics");
    QUERY_CHECK;

    while(query.next())
    {
        QStringList items;
        items << query.value(0).toString();
        items << query.value(1).toString();
        items << query.value(2).toString();
    }
    return QStringList;
}
