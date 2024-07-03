#include "ingame.h"
#include "board.h"
#include "ui_ingame.h"
#include "classes.h"

extern QList<Player> pls;
extern Deck deck;


ingame::ingame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ingame)
{
    ui->setupUi(this);
    ui->boardholder->setParent(this);
    ui->buy->hide();
    ui->drop->hide();
    connect(ui->boardholder,SIGNAL(sendNodeInfo(Board::Node*)),this,SLOT(showNodeInfo(Board::Node*)));
}

ingame::~ingame()
{
    delete ui;
}

void ingame::updateStats()
{
    int the_increase=0;
    the_increase = pls.back().improvements[Player::book];
    the_increase+=pls.back().modifications[Player::book];
    ui->l_book_impr->setText("+ " + QString::number(the_increase));
    the_increase = pls.back().improvements[Player::trade];
    the_increase+=pls.back().modifications[Player::trade];
    ui->l_trade_impr->setText("+ " + QString::number(the_increase));
    the_increase = pls.back().improvements[Player::eyes];
    the_increase+=pls.back().modifications[Player::eyes];
    ui->l_eyes_impr->setText("+ " + QString::number(the_increase));
    the_increase = pls.back().improvements[Player::strength];
    the_increase+=pls.back().modifications[Player::strength];
    ui->l_strength_impr->setText("+ " + QString::number(the_increase));
    the_increase = pls.back().improvements[Player::will];
    the_increase+=pls.back().modifications[Player::will];
    ui->l_will_impr->setText("+ " + QString::number(the_increase));
    ui->l_healthnum->setText(QString::number(pls.back().health));
    ui->l_sanitynum->setText(QString::number(pls.back().sanity));
    ui->l_cluesnum->setText(QString::number(pls.back().clues));
}
// TODO ИНФУ ЧТО УВЕЛИЧИВАЕТ В ТИПБОКС

void ingame::updateHand()
{
    ui->hand_things->clear();
    if (pls.back().hand_things.size()!=0){
        foreach (const Card *value, pls.back().hand_things) {
            ui->hand_things->addItem(value->header);
        }
    }
     ui->hand_magic->clear();
    if (pls.back().hand_magic.size()!=0){
        foreach (const Card *value, pls.back().hand_magic) {
            ui->hand_magic->addItem(value->header);
        }
    }
     ui->hand_arts->clear();
    if (pls.back().hand_arts.size()!=0){
        foreach (const Card *value, pls.back().hand_arts) {
            ui->hand_arts->addItem(value->header);
        }
    }
     ui->hand_bads->clear();
     if (pls.back().hand_arts.size()!=0){
         foreach (const Card *value, pls.back().hand_bads) {
             ui->hand_bads->addItem(value->header);
         }
     }
}

void ingame::setuplayer()
{
ui->l_book->setText(QString::number(pls.back().chars[Player::book]));
ui->l_trade->setText(QString::number(pls.back().chars[Player::trade]));
ui->l_eyes->setText(QString::number(pls.back().chars[Player::eyes]));
ui->l_strength->setText(QString::number(pls.back().chars[Player::strength]));
ui->l_will->setText(QString::number(pls.back().chars[Player::will]));
ui->name->setText(pls.back().name + " (" + pls.back().username + ")");
updateHand();
updateStats();
}

void ingame::showChecks(int Type)
{
    ui->check_count->setText(QString::number(pls.back().currentCheck));
}

void ingame::fillReserve()
{
    ui->reserve->clear();
    for (int i = 0;i<4;i++)
        if (deck.reserve[i].card!=nullptr)
        ui->reserve->addItem(deck.reserve[i].card->header);
        else
        ui->reserve->addItem("empty");
}

void ingame::showNodeInfo(Board::Node* node)
{

    ui->infobox->clear();
    ui->infobox->setText(node->gatherText());
}

void ingame::on_hand_things_currentIndexChanged(int index)
{
    if (index!=-1){
        ui->infobox->clear();
        ui->infobox->setText(pls.back().hand_things.at(index)->gatherText());
    }
}


void ingame::on_hand_things_activated(int index)
{
    if (index!=-1){
        ui->infobox->clear();
        ui->infobox->setText(pls.back().hand_things.at(index)->gatherText());
    }
}


void ingame::on_b_trade_clicked()
{
    ui->list_throws->clear();
    pls.back().goTrade();
    foreach (const int &value, pls.back().dices)
    {
        ui->list_throws->append(QString::number(value));
    }
    ui->drop->show();
    ui->buy->show();
    ui->buy->setEnabled(false);
    showChecks(NULL);
    updateStats();
    updateHand();
}


void ingame::on_reserve_activated(int index)
{
    if (deck.reserve[index].card!=nullptr){
    if (index!=-1){
        ui->infobox->clear();
        ui->infobox->setText(deck.reserve[index].card->gatherText());
    }
    if (!ui->drop->isHidden())
    {
        if (pls.back().currentCheck>NULL)                   //СЮДА СРАВНЕНИЕ ПО КОСТУ
        {
            ui->buy->setEnabled(true);
        }
        else ui->buy->setEnabled(false);
    }
    }
    else
    {
        ui->infobox->clear();
        ui->infobox->setText("empty");
        ui->buy->setEnabled(false);
    }
}


void ingame::on_buy_clicked()
{
    pls.back().hand_things.push_back(deck.reserve[ui->reserve->currentIndex()].card);
    deck.reserve[ui->reserve->currentIndex()].card->playAction(pls.back());
    deck.reserve[ui->reserve->currentIndex()].card=nullptr;
    fillReserve();
    ui->buy->hide();
    ui->buy->setEnabled(false);
    ui->drop->hide();
    updateHand();
    updateStats();
}


void ingame::on_drop_clicked()
{
    //deck.reserve[ui->reserve->currentIndex()].card=nullptr;             //передать в сброс!!!!!!!!!
    fillReserve();
    ui->buy->hide();
    ui->buy->setEnabled(false);
    ui->drop->hide();
}

