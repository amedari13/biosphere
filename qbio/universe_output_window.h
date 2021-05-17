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
private:

    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_stop_universe_clicked();
    void make_a_cycle();
    void write_to_db();

private:
    Ui::universe_output_Window *ui;
    QGraphicsScene *scene;
    global_scene gscene;
    global_scene_stats stats;
};

#endif // UNIVERSE_OUTPUT_WINDOW_H
