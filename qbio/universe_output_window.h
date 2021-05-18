#ifndef UNIVERSE_OUTPUT_WINDOW_H
#define UNIVERSE_OUTPUT_WINDOW_H

#include <QDialog>
#include <QGraphicsScene>

#include "global_scene.h"

namespace Ui {
class universe_output_Window;
}

class UniverseOutputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UniverseOutputWindow(std::pair<int, int> size, QWidget *parent = nullptr);
    ~UniverseOutputWindow();

    void addPoint(int x, int y, int c);
    void onModifyProperty();
private:

    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_stop_universe_clicked();
    void make_a_cycle();
    void write_to_db();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_fearSlider_valueChanged(int value);
    void on_greedSlider_valueChanged(int value);
    void on_lazynessSlider_valueChanged(int value);
    void on_massSlider_valueChanged(int value);
    void on_speedSlider_valueChanged(int value);
    void on_cancelButton_clicked();
    void on_createNewButton_clicked();

private:
    bool _lock_sliders{false};
    Ui::universe_output_Window *ui;
    QGraphicsScene *scene;
    global_scene gscene;
    global_scene_stats stats;
};

#endif // UNIVERSE_OUTPUT_WINDOW_H
