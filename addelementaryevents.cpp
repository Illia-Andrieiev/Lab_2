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
    //QMessageBox::warning(this,"warning",QString::number(event.getEvents().size()));
    // Р’РёРІРѕРґРёРјРѕ РІ СЃРїРёСЃРѕРє РІСЃС– РµР»РµРјРµРЅС‚Р°СЂРЅС– РїРѕРґС–С—
    reprintList();
}

AddElementaryEvents::~AddElementaryEvents()
{
    delete ui;
}

void AddElementaryEvents::on_addElementaryEvent_clicked()
{
    // РџРµСЂРµРІС–СЂРєР° С‡Рё РЅРµ РїРѕСЂРѕР¶РЅС” РІРІРµРґРµРЅРµ С–Рј'СЏ
    QString eeName = ui->addElemEventName->text();
    ui->addElemEventName->setText("");
    if(eeName.isEmpty()){
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Р†Рј'СЏ РµР»РµРјРµРЅС‚Р°СЂРЅРѕС— РїРѕРґС–С— РЅРµ РјРѕР¶Рµ Р±СѓС‚Рё РїРѕСЂРѕР¶РЅС–Рј!");
        return;
    }
    if(!event.isContainElemEvent(eeName)){ // РЇРєС‰Рѕ РµР»РµРјРµРЅС‚Р°СЂРЅРѕС— РїРѕРґС–С— Р· С‚Р°РєРёРј С–Рј'СЏРј РЅРµ РґРѕРґР°РЅРѕ
        // Р—С‡РёС‚СѓС”РјРѕ Р№РјРѕРІС–СЂРЅС–СЃС‚СЊ РµР»РµРјРµРЅС‚Р°СЂРЅРѕС— РїРѕРґС–С—
        QString prob = ui->addProbability->text();
        ui->addProbability->setText("");
        double probability;
        probability = prob.toDouble();
        if(probability>1){
            QMessageBox::critical(this,"error","Р’РІРµРґРµРЅР° Р№РјРѕРІС–СЂРЅС–СЃС‚СЊ РјР°С” Р±СѓС‚Рё <= 1");
            return;
        }
        // РџРѕРІС–РґРѕРјР»СЏС”РјРѕ РїСЂРѕ РЅРµРїСЂР°РІРёР»СЊРЅС– РґР°РЅС–
        if(probability <= 0){
            QMessageBox::critical(this,"error","Р’РІРµРґРµРЅР° Р№РјРѕРІС–СЂРЅС–СЃС‚СЊ РјР°С” Р±СѓС‚Рё РґРѕРґР°С‚РЅС–Рј С‡РёСЃР»РѕРј");
            return;
        }
        try{
            //Р”РѕРґР°С”РјРѕ РґРѕ РїРѕРґС–С— РµР»РµРјРµРЅС‚Р°СЂРЅСѓ РїРѕРґС–СЋ С– РІРёРІРѕРґРёРјРѕ РІ СЃРїРёСЃРѕРє
            ElementaryEvent ee(eeName,probability);
            event.addElementaryEvent(ee);
            ui->generousProbability->setText(QString::number(event.getGenerouseProbabiliry()));
            ui->elemEventsList->addItem(eeName);
        }catch(std::out_of_range ex){ // РЇРєС‰Рѕ Р·Р°РіР°Р»СЊРЅР° Р№РјРѕРІС–СЂРЅС–СЃС‚СЊ > 1 РІРёРІРѕРґРёРјРѕ РїРѕРІС–РґРѕРјР»РµРЅРЅСЏ
            QMessageBox::critical(this,"error", "РЎСѓРјРјР° Р№РјРѕРІС–СЂРЅРѕСЃС‚РµР№ РЅРµ РјРѕР¶Рµ РїРµСЂРµРІРёС‰СѓРІР°С‚Рё 1!");
        }
    }else{
        ui->addProbability->setText("");
        QMessageBox::warning(this,"warning","Р†Рј'СЏ РєРѕР¶РЅРѕС— РµР»РµРјРµРЅС‚Р°СЂРЅРѕС— РїРѕРґС–С— РјР°С” Р±СѓС‚Рё СѓРЅС–РєР°Р»СЊРЅРёРј!");
    }
}

Event AddElementaryEvents::getEvent(){
    return event;
}
void AddElementaryEvents::on_addEvent_clicked()
{
    this->close();
}


void AddElementaryEvents::on_elemEventsList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","РћР±СЂР°РЅРѕ РЅРµРјРѕР¶Р»РёРІРёР№ РµР»РµРјРµРЅС‚ СЃРїРёСЃРєСѓ!");
        return;
    }
    ElementaryEvent ev = event.getEvents().at(index);
    QMessageBox::information(this,"information","Р™РјРѕРІС–СЂРЅС–СЃС‚СЊ РЅР°СЃС‚Р°РЅРЅСЏ РѕР±СЂР°РЅРѕС— РµР»РµРјРµРЅС‚Р°СЂРЅРѕС— РїРѕРґС–С—:\n"
                                + QString::number(ev.getProbability()) );
}
int AddElementaryEvents::indexByName(QString name){
    for(int i = 0;i <event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        if(name == ev.getName())
            return i;
    }
    return -1;
}

void AddElementaryEvents::on_deleteElemEvent_clicked()
{
    if(!ui->elemEventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","РЎРїРѕС‡Р°С‚РєСѓ РѕР±РµСЂС–С‚СЊ РµР»РµРјРµРЅС‚Р°СЂРЅСѓ РїРѕРґС–СЋ");
        return;
    }
    int index = indexByName(ui->elemEventsList->currentItem()->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","РћР±СЂР°РЅРѕ РЅРµРјРѕР¶Р»РёРІРёР№ РµР»РµРјРµРЅС‚ СЃРїРёСЃРєСѓ!");
        return;
    }
    event.remove(index);
    reprintList();
    ui->generousProbability->setText(QString::number(event.getGenerouseProbabiliry()));
}
void AddElementaryEvents::reprintList(){
    ui->elemEventsList->clear();
    for(int i =0;i<event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        QString name = ev.getName();
        ui->elemEventsList->addItem(name);
    }
}

