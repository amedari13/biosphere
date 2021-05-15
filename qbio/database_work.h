#ifndef DATABASE_WORK_H
#define DATABASE_WORK_H
#include <QSqlDatabase>

// непотокобезопасный одиночка
class database_work
{

protected:
    database_work(){};
    static database_work* database;

public:
    database_work(database_work & other) = delete;
    void operator=(const database_work &) = delete;
    static database_work *GetInstance();

    void DB_connectNwrite(QString);
    void DB_delete_all(QSqlDatabase &db);

private:
    bool open_database();
    bool create_database_tables();
    bool exist_db();
    bool add_info_db(QString outer_select);
    void close_db();
private:
    QSqlDatabase db;
};

#endif // DATABASE_WORK_H
