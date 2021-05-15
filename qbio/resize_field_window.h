#ifndef RESIZE_FIELD_WINDOW_H
#define RESIZE_FIELD_WINDOW_H

#include <QDialog>

namespace Ui {
class resize_field_Window;
}

class ResizeFieldWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeFieldWindow(QWidget *parent = nullptr);

    static std::pair<int,int> GetSize();
    std::pair<int,int> size;
    void keyPressEvent(QKeyEvent *event) override;

    ~ResizeFieldWindow();


private slots:
    void on_ok_Button_clicked();
    void on_cancel_Button_clicked();

    bool TryToConfirm();

private:
    int counter = 0;
    Ui::resize_field_Window *ui;
};

#endif // RESIZE_FIELD_WINDOW_H
