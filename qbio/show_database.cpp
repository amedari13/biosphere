#include "show_database.h"
#include "ui_show_database.h"

#include "database_work.h"

show_database::show_database(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::show_database)
{
    ui->setupUi(this);

    if (index >= ui->tableWidget->rowCount())
        ui->tableWidget->insertRow(index);

    // set items
    for(int col = 0; col < items.size(); ++col)
        ui->tableWidget->setItem(index, col,
            new QTableWidgetItem(items[col]));
}

show_database::~show_database()
{
    delete ui;
}
