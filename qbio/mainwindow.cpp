#include "mainwindow.h"
#include "resize_field_window.h"
#include "universe_output_window.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_create_universe_Button_clicked()
{
    hide();
    ResizeFieldWindow universe_size;
    universe_size.setWindowFlags (universe_size.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    universe_size.exec();

    if(universe_size.size.first != 0 && universe_size.size.second != 0)
    {

        auto universe = new UniverseOutputWindow{universe_size.size, this};
        universe->setWindowFlags (Qt::WindowMaximizeButtonHint |
                                 (universe->windowFlags() & ~Qt::WindowContextHelpButtonHint));
        universe->show();

    }
    show();
}

void MainWindow::on_about_Button_clicked()
{
    QMessageBox::about(this, "About project", "This project was created by Daria Amialiushka\n\t\t\t 2020");
}














