#ifndef SHOW_DATABASE_H
#define SHOW_DATABASE_H

#include <QDialog>

namespace Ui {
class show_database;
}

class show_database : public QDialog
{
    Q_OBJECT

public:
    explicit show_database(QWidget *parent = nullptr);
    ~show_database();

private:
    Ui::show_database *ui;
};

#endif // SHOW_DATABASE_H
