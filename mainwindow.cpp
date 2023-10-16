#include <QMessageBox>
#include"addelementaryevents.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elementaryevent.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ElementaryEvent ev("ffff",0.5);
    Event e;
    e.setName("ev");
    e.addElementaryEvent(ev);
    events.push_back(e);
    ui->eventsList->addItem(e.getName());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_exit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_addEvent_clicked()
{
    if(ui->enterEventName->text().isEmpty()){
        QMessageBox::warning(this,"warning","Ім'я випадкової події не може бути порожнім!");
        return;
    }
    Event event;
    event.setName(ui->enterEventName->text());
    if(!isContainEvent(event)){
        events.push_back(event);
        ui->eventsList->addItem(event.getName());
        AddElementaryEvents addWindow(event);
        addWindow.setModal(true);
        addWindow.exec();
        events[events.size()-1] = addWindow.getEvent();
    }else{
        QMessageBox::warning(this,"warning","Ім'я кожної випадкової події має бути унікальним!");
    }
    //ElementaryEvent ev(ui->amountModel->text(),0.5);
    //ev.setName(ui->amountModel->text());
    //ev.setProbability(0.5);
}


void MainWindow::on_eventsList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","Обрано неможливий елемент списку!");
        return;
    }
    Event event = events.at(index);
    AddElementaryEvents addWindow(event);
    addWindow.setModal(true);
    addWindow.exec();
    events[index] = addWindow.getEvent();

}
void MainWindow::on_model_clicked()
{
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Спочатку оберіть подію");
        return;
    }
    QString str = ui->amountModel->text();
    int amountModels;
    amountModels = str.toInt();
    if(amountModels <= 0){
        QMessageBox::critical(this,"error","Введена кількість моделювань має бути додатнім цілим числом");
    }else{
        QMessageBox::information(this,"",QString::number(amountModels));
    }
}
bool MainWindow::isContainEvent(Event ev){
    for(int i = 0;i <events.size();i++){
        Event event = events.at(i);
        if(ev.getName() == event.getName())
            return true;
    }
    return false;
}
int MainWindow::indexByName(QString name){
    for(int i = 0;i <events.size();i++){
        Event event = events.at(i);
        if(name == event.getName())
            return i;
    }
    return -1;
}

void MainWindow::on_deleteElemEvent_clicked()
{
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Спочатку оберіть елементарну подію");
        return;
    }
    int index = indexByName(ui->eventsList->currentItem()->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","Обрано неможливий елемент списку!");
        return;
    }
    events.remove(index);
    reprintList();
}

void MainWindow::reprintList(){
    ui->eventsList->clear();
    for(int i =0;i<events.size();i++){
        Event ev = events.at(i);
        QString name = ev.getName();
        ui->eventsList->addItem(name);
    }
}
