#include "statistics_window.h"
#include "ui_statistics_window.h"

#include "database_work.h"

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsWindow)
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

    QMap<int, QString> map = database_work::GetInstance()->get_games();
    for(auto& [id, name] : map.toStdMap())
    {
        ui->gameSelector->addItem( name, id);
    }
}

StatisticsWindow::~StatisticsWindow()
{
    delete ui;
}

void StatisticsWindow::on_gameSelector_currentIndexChanged(int index)
{
    int id = ui->gameSelector->itemData(index).toInt();
    auto data = database_work::GetInstance()->get_species_of_game(id);

    int row = 0;
    for( auto const & line : data)
    {
        if (row >= ui->tableWidget->rowCount())
            ui->tableWidget->insertRow(row);

        int col = 0;
        for( auto const & item : line)
        {
            ui->tableWidget->setItem(row, col++,
                                     new QTableWidgetItem(item));
        }
        ++row;
    }

    while( row > ui->tableWidget->rowCount())
        ui->tableWidget->removeRow( row );

}
