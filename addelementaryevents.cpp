#include<QMessageBox>
#include "addelementaryevents.h"
#include "ui_addelementaryevents.h"
#include "elementaryevent.h"
#include "event.h"
#include "event.h"
AddElementaryEvents::AddElementaryEvents(Event &ev, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddElementaryEvents)
{
    this->event = ev;
    ui->setupUi(this);
    // Виводимо в список всі елементарні події
    for(int i =0;i<event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        QString name = ev.getName();
        ui->elemEventsList->addItem(name);
    }
}

AddElementaryEvents::~AddElementaryEvents()
{
    delete ui;
}

void AddElementaryEvents::on_addElementaryEvent_clicked()
{
    // Перевірка чи не порожнє введене ім'я
    QString eeName = ui->addElemEventName->text();
    ui->addElemEventName->setText("");
    if(eeName.isEmpty()){
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Ім'я елементарної події не може бути порожнім!");
        return;
    }
    if(!event.isContainElemEvent(eeName)){ // Якщо елементарної події з таким ім'ям не додано
        // Зчитуємо ймовірність елементарної події
        QString prob = ui->addProbability->text();
        ui->addProbability->setText("");
        double probability;
        probability = prob.toDouble();
        if(probability>1){
            QMessageBox::critical(this,"error","Введена ймовірність має бути <= 1");
            return;
        }
        // Повідомляємо про неправильні дані
        if(probability <= 0){
            QMessageBox::critical(this,"error","Введена ймовірність має бути додатнім числом");
            return;
        }
        try{
            //Додаємо до події елементарну подію і виводимо в список
            ElementaryEvent ee(eeName,probability);
            event.addElementaryEvent(ee);
            ui->generousProbability->setText(QString::number(event.getGenerouseProbabiliry()));
            ui->elemEventsList->addItem(eeName);
        }catch(std::out_of_range ex){ // Якщо загальна ймовірність > 1 виводимо повідомлення
            QMessageBox::critical(this,"error", "Сумма ймовірностей не може перевищувати 1!");
        }
    }else{
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Ім'я кожної випадкової події має бути унікальним!");
    }
}

