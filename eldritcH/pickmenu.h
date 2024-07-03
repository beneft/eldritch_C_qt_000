#ifndef PICKMENU_H
#define PICKMENU_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class pickmenu;
}

class pickmenu : public QWidget
{
    Q_OBJECT

public:
    explicit pickmenu(QWidget *parent = nullptr);
    ~pickmenu();  

private slots:


    void on_list_chara_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_confirm_clicked();

private:
    Ui::pickmenu *ui;
    void showchars();
};

#endif // PICKMENU_H
