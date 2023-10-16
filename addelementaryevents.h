#pragma once
#ifndef ADDELEMENTARYEVENTS_H
#define ADDELEMENTARYEVENTS_H

#include <QDialog>
#include <QListWidgetItem >
#include"event.h"
namespace Ui {
class AddElementaryEvents;
}

class AddElementaryEvents : public QDialog
{
    Q_OBJECT

public:
    explicit AddElementaryEvents(Event ev,QWidget *parent = nullptr);
    ~AddElementaryEvents();
    Event getEvent();
private slots:
    void on_addElementaryEvent_clicked();

    void on_addEvent_clicked();
    int indexByName(QString name);
    void on_elemEventsList_itemDoubleClicked(QListWidgetItem *item);

    void on_deleteElemEvent_clicked();
    void reprintList();
private:
    Ui::AddElementaryEvents *ui;
    Event event;

};

#endif // ADDELEMENTARYEVENTS_H
