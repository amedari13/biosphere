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
        assert(false); \
        return false; \
    }

#define QUERY_CHECK2 \
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
        assert(false); \
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

int database_work::create_game(int w, int h)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO game (width, height) VALUES (:width, :height);");
    query.bindValue(0, w);
    query.bindValue(1, h);
    query.exec();
    QUERY_CHECK;

    int id = query.lastInsertId().toInt();
    return id;
}

QMap<int, QString> database_work::get_games()
{
    QMap<int, QString> result;

    QSqlQuery query(db);
    query.exec("SELECT id, width, height FROM game;");
    QUERY_CHECK2;

    while(query.next())
    {
        int id = query.value(0).toInt();
        int w = query.value(1).toInt();
        int h = query.value(2).toInt();

        auto v = QString("game #%1 (%2 x %3)")
                .arg(id).arg(w).arg(h);
        result[id] = v;
    }

    return result;
}

QList<QStringList> database_work::get_species_of_game(int id)
{
    QList<QStringList> result;

    QSqlQuery query(db);
    query.prepare("SELECT name, q_alive, q_dead FROM bio_species, bio_statistics"
                  " WHERE bio_species.game = :game"
                  " AND bio_statistics.species = bio_species.id"
                  ";");
    query.bindValue(":game", id);
    query.exec();
    QUERY_CHECK2;

    while(query.next())
    {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();

        result << row;
    }

    return result;
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

    query.exec("create table if not exists game"
               "(id INTEGER primary key AUTOINCREMENT"
               ",width INTEGER"
               ",height INTEGER"
               ");");
    QUERY_CHECK;

    query.exec("create table if not exists bio_species"
               "(id INTEGER primary key AUTOINCREMENT"
               ",game INTEGER"
               ",name nvarchar(20)"
               ")");
    QUERY_CHECK;

    query.exec(" create table if not exists bio_statistics"
               "(species INTEGER"
               ",q_alive INTEGER"
               ",q_dead INTEGER"
               ");");
    QUERY_CHECK;

    return true;
}

int database_work::save_species(species_ptr sp, const species_stat_entry &st, int game_id)
{
    QSqlQuery query(db);

    if ( sp->get_id() == 0)
    {
        query.prepare("INSERT INTO bio_species (name, game) VALUES (:name, :game);");
        query.bindValue(0, QString::fromStdString(sp->get_name()));
        query.bindValue(1, game_id);
        query.exec();
        QUERY_CHECK;

        int id = query.lastInsertId().toInt();
        sp->set_id( id );
    }

    {
        QSqlQuery query(db);

        query.prepare("INSERT OR REPLACE INTO bio_statistics (species, q_alive, q_dead) "
                      "VALUES (:sp, :alive, :dead);");
        query.bindValue(":sp", sp->get_id());
        query.bindValue(":alive", st.count);
        query.bindValue(":dead", st.dead);
        query.exec();
        QUERY_CHECK;
    }

    return true;
}

std::vector<QStringList> database_work::get_species()
{
    QSqlQuery query(db);

    query.exec(" select species, q_alive, q_dead from bio_statistics");
    if (query.lastError().isValid())\
    {
        QString binds;
        foreach(QString v, query.boundValues().keys())
            binds += v + " = " + query.boundValues()[v].toString() + ";  ";
        qDebug() << "** Error in query: " << query.lastQuery();
        qDebug() << "** Binds: " << binds;
        qDebug() << "** Error text:";
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().driverText();
        //return false;
    }

    std::vector<QStringList> items_list;
    while(query.next())
    {
        QStringList items;
        items << query.value(0).toString();
        items << query.value(1).toString();
        items << query.value(2).toString();
        items_list.push_back(items);
    }
    return items_list;
}
