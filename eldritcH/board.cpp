#include "board.h"
#include <QPainter>
#include "classes.h"


extern Board board;



boardui::boardui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::boardui)
{
    ui->setupUi(this);
    //QPainter painter(this);
   // painter.drawPolygon()
    paintEvent(0);
}

boardui::~boardui()
{
    delete ui;
}

void boardui::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QPen pen (Qt::black,3);
    QSet<int> drawn;
    QPushButton* butbuf[5] = {ui->archam,ui->n_1,ui->n_2,ui->n_3,ui->n_4};
    for (int currentNode =0; currentNode < 5; currentNode++ )               //here is quantity of nodes is not constvar
    {
        drawn.insert(currentNode);
        foreach(const Board::Road road, board.graph[currentNode].heighbours)
        {
            if (!drawn.contains(road.where->id))
            {
                QPoint start = butbuf[currentNode]->pos();
                start.setX(start.x()+45);
                start.setY(start.y()+15);
                QPoint end = butbuf[road.where->id]->pos();
                end.setX(end.x()+45);
                end.setY(end.y()+15);
                switch (road.how){
                case Board::dirt:{
                    pen.setBrush(QColor(255,230,0));
                    pen.setStyle(Qt::DotLine);
                    break;
                }
                case Board::train:{
                    pen.setBrush(Qt::red);
                    pen.setStyle(Qt::DashLine);
                    break;
                }
                case Board::ferry:{
                    pen.setBrush(Qt::blue);
                    pen.setStyle(Qt::SolidLine);
                    break;
                }
                default: pen.setWidth(90);
                }
                painter.setPen(pen);
                painter.drawLine(start,end);
            }
        }
    }
}

void boardui::on_archam_clicked()
{
    emit sendNodeInfo(&board.graph[0]);
    ui->archam->setPalette(QPalette(QColor(225,0,0)));      //////////dodelat zdes
    ui->archam->update();
    red_button=0;
}


void boardui::on_n_1_clicked()
{
    emit sendNodeInfo(&board.graph[1]);
}


void boardui::on_n_2_clicked()
{
    emit sendNodeInfo(&board.graph[2]);
}


void boardui::on_n_3_clicked()
{
    emit sendNodeInfo(&board.graph[3]);
}


void boardui::on_n_4_clicked()
{
    emit sendNodeInfo(&board.graph[4]);
}

