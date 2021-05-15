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

    query.exec("create table bio_species"
               "(id int primary key,"
               "name nvarchar(20)"
               ")");
    QUERY_CHECK;

    query.exec(" create table bio_piece"
                "(species int foreign key references bio_species(id)"
                ",id int primary key"
                ",alive bit default 1"
                ",lifespan int);");
    QUERY_CHECK;

    query.exec(" create table bio_statistics"
                  "(species int foreign key references bio_species(id)"
                  ",q_alive int"
                  ",q_dead int"
                  ",avg_lifespan int"
                  ",q_tics int);");
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

    // бд существует -> добавляем записи
    if(!add_info_db(outer_select))
    {
        QMessageBox::critical(0, "Error", "The program will be closed. Unable to edit the database.");
        QApplication::exit(-1);
    }
    close_db();
}

void database_work::DB_delete_all(QSqlDatabase &db)
{
    QSqlQuery del_query(db);
    del_query.exec("use master; drop database biosphere_simulation;");
}
