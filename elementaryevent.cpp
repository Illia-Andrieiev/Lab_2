#include<QString>
#include "elementaryevent.h"

// Конструктор
ElementaryEvent::ElementaryEvent(QString name, double probability){
    setName(name);
    setProbability(probability);
}
// Повертає ім'я елементарної події
QString ElementaryEvent::getName(){
    return name;
}
// Повертає ймовірність елементарної події
double ElementaryEvent::getProbability(){
    return probability;
}
// Задає ім'я елементарної події
void ElementaryEvent::setName(QString name){
    this->name = name;
}
// Задає ймовірність елементарної події
void ElementaryEvent::setProbability(double probability){
    if(probability > 1)
        this->probability = 1;
    else if(probability<0)
        this->probability = 0;
    else
        this->probability = probability;
}
