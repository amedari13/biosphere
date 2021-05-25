#ifndef STATISTICS_WINDOW_H
#define STATISTICS_WINDOW_H

#include <QDialog>

namespace Ui {
class StatisticsWindow;
}

class StatisticsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsWindow(QWidget *parent = nullptr);
    ~StatisticsWindow();

private slots:
    void on_gameSelector_currentIndexChanged(int index);

private:
    Ui::StatisticsWindow *ui;
};

#endif // STATISTICS_WINDOW_H
