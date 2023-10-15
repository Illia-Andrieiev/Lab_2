#pragma once
#ifndef ADDELEMENTARYEVENTS_H
#define ADDELEMENTARYEVENTS_H

#include <QDialog>
#include"event.h"
namespace Ui {
class AddElementaryEvents;
}

class AddElementaryEvents : public QDialog
{
    Q_OBJECT

public:
    explicit AddElementaryEvents(Event &ev,QWidget *parent = nullptr);
    ~AddElementaryEvents();

private slots:
    void on_addElementaryEvent_clicked();

private:
    Ui::AddElementaryEvents *ui;
    Event event;
};

#endif // ADDELEMENTARYEVENTS_H
