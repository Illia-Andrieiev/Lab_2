#pragma once
#include"elementaryevent.h"
#include<QVector>
#include<QString>
#ifndef EVENT_H
#define EVENT_H
class Event
{
private:
    double generouseProbabiliry = 0;
    QVector<ElementaryEvent> events;
    QString name;
public:
    bool isContainElemEvent(QString name);
    double getGenerouseProbabiliry();
    QString getName();
    void setName(QString name);
    QVector<ElementaryEvent> getEvents();
    void addElementaryEvent(ElementaryEvent event);
};

#endif // EVENT_H
