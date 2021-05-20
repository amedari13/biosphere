#include "show_database.h"
#include "ui_show_database.h"

#include "database_work.h"

show_database::show_database(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::show_database)
{
    ui->setupUi(this);

    std::vector<QStringList> db_list = database_work::GetInstance()->get_species();

    int index = db_list.size();
    if (index >= ui->tableWidget->rowCount())
        ui->tableWidget->insertRow(index);//задаем количество рядов

    for(int row = 0; row < index; ++row)
    {
        for(int col = 0; col < 3; ++col)//3 элемента, потом отредачить
            ui->tableWidget->setItem(row, col,
                new QTableWidgetItem(db_list[row][col]));
    }
}

show_database::~show_database()
{
    delete ui;
}
