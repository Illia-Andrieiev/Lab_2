#include<QMessageBox>
#include "addelementaryevents.h"
#include "ui_addelementaryevents.h"
#include "elementaryevent.h"
#include "event.h"
#include "event.h"
AddElementaryEvents::AddElementaryEvents(Event ev, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddElementaryEvents)
{
    this->event = ev;
    ui->setupUi(this);
    ui->generousProbability->setText(QString::number(event.getGenerouseProbabiliry()));
    reprintList();
}
AddElementaryEvents::~AddElementaryEvents()
{
    delete ui;
}
// Додає елементарну подію до випадкової події
void AddElementaryEvents::on_addElementaryEvent_clicked()
{
    // Зчитується ім'я елементарної події. Якщо порожнє виходить із функції
    QString eeName = ui->addElemEventName->text();
    ui->addElemEventName->setText(""); // Робить lineEdit порожнім
    if(eeName.isEmpty()){
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Назва елементарної події не може бути порожньою!");
        return;
    }
    // ';' є недопустимим символом. Якщо він входить у задане ім'я, завершуємо роботу
    if(eeName.contains(';')){
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Назва елементарної події не може містити ';'!");
        return;
    }

    if(!event.isContainElemEvent(eeName)){ // Якщо випадковій події не належить подія з заданим ім'ям, додаємо
        // Зчитування ймовірності елементарної події
        QString prob = ui->addProbability->text();
        ui->addProbability->setText("");
        double probability = prob.toDouble();
        // Якщо ймовірність > 1 виходимо з функції
        if(probability>1){
            QMessageBox::critical(this,"error","Ймовірність має бути <= 1!");
            return;
        }
        // Якщо ймовірність <= 0  виходимо з функції
        if(probability <= 0){
            QMessageBox::critical(this,"error","Ймовірність має бути > 0!");
            return;
        }
        try{
            // Створення елементарної події, додавання її до випадкової події та до списку елементарних подій
            ElementaryEvent ee(eeName,probability);
            event.addElementaryEvent(ee);
            ui->generousProbability->setText(QString::number(event.getGenerouseProbabiliry())); // Зміна загальної ймовірності
            ui->elemEventsList->addItem(eeName);
        }catch(std::out_of_range ex){ // Якщо загальна ймовірність > 1
            QMessageBox::critical(this,"error", "Сума ймовірностей елементарних подій не може перевищувати 1!");
        }
    }else{
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Випадковій події вже належить елементарна подія\nіз введеним ім'ям!");
    }
}
// Закриває вікно та завершує редагування
void AddElementaryEvents::on_addEvent_clicked()
{
    this->close();
}

// Виводить інфрмацію про натиснуту елементарну подію у QMessageBox
void AddElementaryEvents::on_elemEventsList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){ // Якщо подію не знйдено, виходимо із функції
        QMessageBox::warning(this,"warning","Не знайдено обрану елементарну подію!");
        return;
    }
    ElementaryEvent ev = event.getEvents().at(index);
    QMessageBox::information(this,"information","Ймовірніть події " + ev.getName() + ":\n"
                                + QString::number(ev.getProbability()) );
}
// Повертає індекс елементарної події в випадковій події. -1 якщо не знайдено
int AddElementaryEvents::indexByName(QString name){
    for(int i = 0;i <event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        if(name == ev.getName())
            return i;
    }
    return -1;
}
// Видаляє обрану елементарну подію з випадкової події
void AddElementaryEvents::on_deleteElemEvent_clicked()
{
    // Якщо не обрано елементарну подію, виходимо із функції
    if(!ui->elemEventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Оберіть елементарну подію!");
        return;
    }
    int index = indexByName(ui->elemEventsList->currentItem()->text());
    //Якщо не знайдено обрану елементарну подію, виходимо із функції
    if(index == -1){
        QMessageBox::warning(this,"warning","Не знайдено обрану елементарну подію!");
        return;
    }
    event.remove(index); // Видаляємо елементарну подію
    reprintList(); // Заново виводимо у список всі елементарні події
    ui->generousProbability->setText(QString::number(event.getGenerouseProbabiliry()));// Перезапис суми ймовірностей
}
// Виводить список всіх елементарних подій переданої випадкової події при відкриті вікна
void AddElementaryEvents::reprintList(){
    ui->elemEventsList->clear();
    for(int i =0;i<event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        QString name = ev.getName();
        ui->elemEventsList->addItem(name);
    }
}
// Повертає відредаговану випадкову подію
Event AddElementaryEvents::getEvent(){
    return event;
}
