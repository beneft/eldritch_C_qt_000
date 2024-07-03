#include "pickmenu.h"
#include "mainwindow.h"
#include "ingame.h"
#include "classes.h"
#include "ui_pickmenu.h"
#include <QStackedWidget>

extern Storage storage;

pickmenu::pickmenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pickmenu)
{
    ui->setupUi(this);
    showchars();
}

pickmenu::~pickmenu()
{
    delete ui;
}

void pickmenu::showchars()
{
    for (int i = 0; i< storage.data_characters.size();i++ )
    {
    ui->list_chara->addItem(storage.data_characters.at(i).name);
    }
}

void pickmenu::on_list_chara_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current!=previous)
    {
        ui->info_chara->clear();
        const Player* dummy = &storage.data_characters.at(storage.namemap_chara[current->text()]);
        ui->info_chara->addItem("bio " + dummy->bio);
        if (dummy->start_loadout!="") ui->info_chara->addItem("start_loadout " + dummy->start_loadout);
        ui->info_chara->addItem("health " + QString::number(dummy->health));
        ui->info_chara->addItem("sanity " + QString::number(dummy->sanity));
        ui->info_chara->addItem("chars: "+ QString::number(dummy->chars[0])+ QString::number(dummy->chars[1])+ QString::number(dummy->chars[2])+ QString::number(dummy->chars[3])+ QString::number(dummy->chars[4]));
        ui->info_chara->addItem("improvements: " + QString::number(dummy->improvements[0])+ QString::number(dummy->improvements[1])+ QString::number(dummy->improvements[2])+ QString::number(dummy->improvements[3])+ QString::number(dummy->improvements[4]));
        ui->info_chara->addItem("clues " + QString::number(dummy->clues));
    }
}


void pickmenu::on_confirm_clicked()
{
if (ui->nickname_input->text()!="" && ui->list_chara->currentIndex().row()!=-1)
{
    addPlayer(storage.namemap_chara[ui->list_chara->currentItem()->text()],ui->nickname_input->text());
    pullStartLoadout();
    QWidget* dummy = static_cast<QStackedWidget*>(this->parent())->widget(1);
    qobject_cast<ingame*>(dummy)->setuplayer();
    Deck::pullReserve();
    qobject_cast<ingame*>(dummy)->fillReserve();
    static_cast<QStackedWidget*>(this->parent())->setCurrentIndex(1);
}
}

