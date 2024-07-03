#ifndef BOARD_H
#define BOARD_H

#include "ui_board.h"
#include "classes.h"

namespace Ui {
class boardui;
}

class boardui : public QWidget
{
    Q_OBJECT

public:
    explicit boardui(QWidget *parent = nullptr);
    ~boardui();

    int red_button=0;

private:
    Ui::boardui* ui;
signals:
    void sendNodeInfo(Board::Node* node);


protected: void paintEvent(QPaintEvent* e);
private slots:
    void on_archam_clicked();
    void on_n_1_clicked();
    void on_n_2_clicked();
    void on_n_3_clicked();
    void on_n_4_clicked();
};
#endif // BOARD_H
