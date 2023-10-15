#pragma once
#include<QString>
#include "elementaryevent.h"


ElementaryEvent::ElementaryEvent(QString name, double probability){
    setName(name);
    setProbability(probability);
}
QString ElementaryEvent::getName(){
    return name;
}
double ElementaryEvent::getProbability(){
    return probability;
}
void ElementaryEvent::setName(QString name){
    this->name = name;
}
void ElementaryEvent::setProbability(double probability){
    if(probability > 1)
        probability = 1;
    else
        this->probability = probability;
}
