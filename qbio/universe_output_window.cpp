#include "universe_output_window.h"
#include "ui_universe_output_window.h"
#include <QMessageBox>
#include "database_work.h"
#include <string.h>
#include "scene.h"
#include <QGraphicsView>
#include <QTimer>
#include <QtAlgorithms>

UniverseOutputWindow::UniverseOutputWindow(std::pair<int,int> size, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::universe_output_Window)
    , gscene(size.first, size.second)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    for (int c = 6; c-- > 0; )
            gscene.add(15, species::random());

    startTimer(100);

    //database_work::GetInstance()->DB_connectNwrite("");
}

void UniverseOutputWindow::make_a_cycle()
{
    gscene.calculate();
    stats = gscene.get_statistics();

    scene->clear();
    for(auto const & b : gscene.get_beings())
    {
        auto p = b->get_position();
        addPoint(p.x, p.y);
    }
}


UniverseOutputWindow::~UniverseOutputWindow()
{
    delete ui;
}

void UniverseOutputWindow::addPoint(int x, int y)
{
    double r = 0.1;
    scene->addEllipse(x-r, y-r, 2*r, 2*r,
                      QPen(), QBrush(Qt::SolidPattern));
}

void UniverseOutputWindow::timerEvent(QTimerEvent * /*event*/)
{
    make_a_cycle();
}

void UniverseOutputWindow::resizeEvent(QResizeEvent * /*event*/)
{
    auto sceneRect = QRectF(QPointF(0,0), QSizeF(gscene.get_width(), gscene.get_height()));
    ui->graphicsView->setSceneRect(sceneRect);
    ui->graphicsView->fitInView(sceneRect);
}

void UniverseOutputWindow::on_stop_universe_clicked()
{
}

