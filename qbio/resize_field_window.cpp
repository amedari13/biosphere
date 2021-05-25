#include "resize_field_window.h"
#include "ui_resize_field_window.h"
#include <QKeyEvent>
#include <QString>
#include <QIntValidator>
#include <QMessageBox>

ResizeFieldWindow::ResizeFieldWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resize_field_Window)
{
    ui->setupUi(this);
    ui->get_height->setValidator (new QIntValidator (this));
    ui->get_width->setValidator  (new QIntValidator (this));
    ui->get_width->setFocus();
}

ResizeFieldWindow::~ResizeFieldWindow(){ delete ui; }

bool ResizeFieldWindow::TryToConfirm()
{
    int _width, _height;
    _width  = ui->get_width ->text().toInt();
    _height = ui->get_height->text().toInt();

    if (_width < 10 || _width > 1000
            || _height < 10 || _height > 1000)
    {
        QMessageBox::warning(this, "Error", "The width and height should be between 10 and 1000");
        return false;
    }
    size.first = _width;
    size.second = _height;
    close();
    return true;
}

void ResizeFieldWindow::on_ok_Button_clicked()
{
    TryToConfirm();
}

void ResizeFieldWindow::on_cancel_Button_clicked()
{
    close();
}

void ResizeFieldWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Down || event->key() == Qt::Key_Up)
    {
        if(counter % 2 == 0)
            ui->get_height->setFocus();
        else
            ui->get_width->setFocus();
        counter++;
    }
}










