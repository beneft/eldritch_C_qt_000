#ifndef INGAME_H
#define INGAME_H

#include <QWidget>
#include <QListWidget>
#include "board.h"
#include "QBoxLayout"

namespace Ui {
class ingame;
}

class ingame : public QWidget
{
    Q_OBJECT

public:
    explicit ingame(QWidget *parent = nullptr);
    ~ingame();
public slots:
    void setuplayer();
    void updateStats();
    void updateHand();
    void showChecks(int Type);
    void fillReserve();

private slots:
    void showNodeInfo(Board::Node* node);

    void on_hand_things_currentIndexChanged(int index);

    void on_hand_things_activated(int index);

    void on_b_trade_clicked();

    void on_reserve_activated(int index);

    void on_buy_clicked();

    void on_drop_clicked();

private:
    Ui::ingame* ui;
    QMap<QString,int> itemmap;

};

#endif // INGAME_H
