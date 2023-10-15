#pragma once
#include<QString>
#ifndef ELEMENTARYEVENT_H
#define ELEMENTARYEVENT_H


class ElementaryEvent
{
private:
    QString name;
    double probability;
public:
    ElementaryEvent(QString name, double probability);
    QString getName();
    double getProbability();
    void setName(QString name);
    void setProbability(double probability);
};
#endif // ELEMENTARYEVENT_H
