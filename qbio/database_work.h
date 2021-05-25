#ifndef DATABASE_WORK_H
#define DATABASE_WORK_H

#include <QSqlDatabase>

#include "global_scene.h"

// непотокобезопасный одиночка
class database_work
{

protected:
    database_work();
    static database_work* database;

public:
    database_work(database_work & other) = delete;
    void operator=(const database_work &) = delete;
    static database_work *GetInstance();

    int create_game(int w, int h);
    QMap<int, QString> get_games();
    QList<QStringList> get_species_of_game(int id);

    int save_species(species_ptr sp, species_stat_entry const& st , int game_id);
    std::vector<QStringList> get_species();

private:
    bool open_database();
    bool create_database_tables();

private:
    QSqlDatabase db;
};

#endif // DATABASE_WORK_H
