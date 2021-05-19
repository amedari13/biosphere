#include "universe_output_window.h"
#include "ui_universe_output_window.h"
#include <QMessageBox>
#include "database_work.h"
#include <string.h>
#include "scene.h"
#include <QGraphicsView>
#include <QTimer>
#include <QtAlgorithms>

QColor int2color( int c )
{
    static QStringList ns = QString(
                "blue red yellow violet orange yellowgreen "
                "pink plum olive navy khaki").split(" ");
    return QColor{ ns[ c % ns.size() ] };
}

UniverseOutputWindow::UniverseOutputWindow(std::pair<int,int> size, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::universe_output_Window)
    , gscene(size.first, size.second)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->tableWidget->setColumnWidth(0, 170);
    ui->tableWidget->setColumnWidth(1, 60);
    ui->tableWidget->setColumnWidth(2, 60);
    ui->tableWidget->setColumnWidth(3, 60);
    ui->tableWidget->setColumnWidth(4, 60);

    srand(time(0));
    for (int c = 6; c-- > 0; )
            gscene.add(15, species::random());

    startTimer(100);
}

void UniverseOutputWindow::make_a_cycle()
{
    gscene.calculate();
    stats = gscene.get_statistics();

    int index = 0;
    for(auto const& [sp, st] : stats.by_species)
    {
        if (index >= ui->tableWidget->rowCount())
            ui->tableWidget->insertRow(index);

        QStringList items;
        items << QString::fromStdString(sp->get_name());
        items << QString::number( st.count );
        items << QString::number( st.dead );
        items << QString::number( st.count ? st.total_mass / st.count : 0);
        items << QString::number( st.max_mass );

        // set items
        for(int col = 0; col < items.size(); ++col)
            ui->tableWidget->setItem(index, col,
                new QTableWidgetItem(items[col]));

        // set color
        ui->tableWidget->item(index, 0)->setForeground(
                    QBrush{int2color(sp->get_color())});

        ++index;
    }
    while(index < ui->tableWidget->rowCount())
        ui->tableWidget->removeRow( index );

    scene->clear();
    scene->addRect(0, 0, gscene.get_width(), gscene.get_height(),
                   QPen(QColor("black")),
                   QBrush(QColor("black")));
    for(auto const & b : gscene.get_beings())
    {
        auto p = b->get_position();
        addPoint(p.x, p.y,
                 b->get_species()->get_color());
    }
}

void UniverseOutputWindow::write_to_db()
{
    for(auto const& [sp, st] : stats.by_species)
    {
        database_work::GetInstance()->save_species( sp, st );
    }
}


UniverseOutputWindow::~UniverseOutputWindow()
{
    delete ui;
}

void UniverseOutputWindow::addPoint(int x, int y, int c)
{
    double r = 0.1;
    QColor clr = int2color(c);
    scene->addEllipse(x-r, y-r, 2*r, 2*r,
                      QPen(clr), QBrush{Qt::transparent});
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
    killTimer(0);
    write_to_db();
}

