#include<ctime>
#include<iostream>
#include<QMessageBox>
#include<QString>
#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(Event ev, int n, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    this->n = n;
    event = ev;
    srand(time(0));
    ui->setupUi(this);
    PrintSimulation();
}

ResultWindow::~ResultWindow()
{
    delete ui;
}
void ResultWindow::PrintSimulation(){
    while(n>0){
        ElementaryEvent cur = chooseElemEvent();
        QString name = cur.getName();
        ui->resList->addItem(name);
        --n;
    }
}
ElementaryEvent ResultWindow::chooseElemEvent(){
    int r = rand();
    double position = static_cast<double>(r)/RAND_MAX;//RAND_MAX=32767
    ElementaryEvent choosen("",0);
    double currentPosition = 0;
    for(int i = 0;i<event.getEvents().size();i++){
        choosen = event.getEvents().at(i);
        currentPosition += choosen.getProbability();
        if(currentPosition >= position)
            return choosen;
    }
    if(currentPosition < position){
        choosen.setName("Жодна з заданих подій не відбулась!");
        choosen.setProbability(1-currentPosition);
    }
    return choosen;
}

void ResultWindow::on_resList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","Обрано неможливий елемент списку!");
        return;
    }
    ElementaryEvent ev = event.getEvents().at(index);
    QMessageBox::information(this,"information","Подія настала з ймовірністю:\n"
                                + QString::number(ev.getProbability()) );
}
int  ResultWindow::indexByName(QString name){
    for(int i = 0;i <event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        if(name == ev.getName())
            return i;
    }
    return -1;
}

