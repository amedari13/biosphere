#include "universe_output_window.h"
#include "ui_universe_output_window.h"
#include <QMessageBox>
#include "database_work.h"
#include <string.h>
#include "scene.h"
#include <QGraphicsView>
#include <QTimer>
#include <QtAlgorithms>
#include <show_database.h>

QColor int2color( int c )
{
    static QStringList ns = QString(
                "blue red yellow violet orange yellowgreen "
                "pink plum olive khaki white").split(" ");
    return QColor{ ns[ c % ns.size() ] };
}

UniverseOutputWindow::UniverseOutputWindow(std::pair<int,int> size, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::universe_output_Window)
    , gscene(size.first, size.second)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setVisible(false);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->tableWidget->setColumnWidth(0, 173);
    ui->tableWidget->setColumnWidth(1, 85);
    ui->tableWidget->setColumnWidth(2, 85);
    ui->tableWidget->setColumnWidth(3, 85);
    ui->tableWidget->setColumnWidth(4, 85);

    ui->fearSlider->setRange( 0, 400 );
    ui->greedSlider->setRange( 0, 400 );
    ui->lazynessSlider->setRange( 0, 400 );
    ui->massSlider->setRange( 0, 400 );
    ui->speedSlider->setRange( 0, 400 );

    srand(time(0));
    for (int c = 6; c-- > 0; )
            gscene.add(15, species::random());

    timer_id = startTimer(100);
}

void UniverseOutputWindow::make_a_cycle()
{
    gscene.calculate();
    stats = gscene.get_statistics();

    int index = 0;
    for(auto const& [sp, st] : stats.by_species)
    {
        if (st.count ==0 && st.dead == 0)
            continue;

        QStringList items;
        items << QString::fromStdString(sp->get_name());
        items << QString::number( st.count );
        items << QString::number( st.dead );
        items << QString::number( st.count ? st.total_mass / st.count : 0);
        items << QString::number( st.max_mass );

        if (index >= ui->tableWidget->rowCount())
            ui->tableWidget->insertRow(index);

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

void UniverseOutputWindow::onModifyProperty()
{
    if (_lock_sliders) return;
    ui->createNewButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);
    ui->propertyGroup->setTitle(QString("Unknown creature"));
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

void UniverseOutputWindow::showEvent(QShowEvent * /*event*/)
{
    resize(1099, 593);
}

void UniverseOutputWindow::on_stop_universe_clicked()
{
    killTimer(timer_id);
    write_to_db();
    show_database win;
    win.exec();
}


void UniverseOutputWindow::on_tableWidget_currentCellChanged(int currentRow,
                                                             int /*currentColumn*/,
                                                             int /*previousRow*/,
                                                             int /*previousColumn*/)
{
    int index=0;
    for(auto const& [sp, st] : stats.by_species)
    {
        if (st.count ==0 && st.dead == 0)
            continue;
        if (currentRow == index++)
        {
            _lock_sliders = true;
            ui->propertyGroup->setTitle(QString::fromStdString(sp->get_name()));
            ui->fearSlider->setValue(sp->_fear * 100 / fear_default);
            ui->greedSlider->setValue(sp->_greed * 100 / greed_default);
            ui->lazynessSlider->setValue(sp->_lazyness * 100 / lazyness_default);
            ui->massSlider->setValue(sp->_mass_limit * 100 / mass_limit_default);
            ui->speedSlider->setValue(sp->_speed * 100 / speed_default);
            _lock_sliders = false;
            return;
        }
    }
}

void UniverseOutputWindow::on_fearSlider_valueChanged(int /*value*/)
{
    onModifyProperty();
}

void UniverseOutputWindow::on_greedSlider_valueChanged(int /*value*/)
{
    onModifyProperty();
}

void UniverseOutputWindow::on_lazynessSlider_valueChanged(int /*value*/)
{
    onModifyProperty();
}

void UniverseOutputWindow::on_massSlider_valueChanged(int /*value*/)
{
    onModifyProperty();
}

void UniverseOutputWindow::on_speedSlider_valueChanged(int /*value*/)
{
    onModifyProperty();
}

void UniverseOutputWindow::on_cancelButton_clicked()
{
    ui->createNewButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);
    on_tableWidget_currentCellChanged(
                ui->tableWidget->currentRow(), 1, 0, 0);
}

void UniverseOutputWindow::on_createNewButton_clicked()
{
    auto sp = species::random();
    sp->_fear = fear_default * ui->fearSlider->value() / 100;
    sp->_greed= greed_default * ui->greedSlider->value() / 100;
    sp->_lazyness = lazyness_default * ui->lazynessSlider->value() / 100;
    sp->_mass_limit = std::max(40, mass_limit_default * ui->massSlider->value() / 100);
    sp->_speed = speed_default * ui->speedSlider->value() / 100;
    gscene.add(15, sp);

    on_cancelButton_clicked();
}
