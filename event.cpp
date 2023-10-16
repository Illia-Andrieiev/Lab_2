#include "event.h"
#include<QString>
#include<vector>

double Event::getGenerouseProbabiliry(){
    return generouseProbabiliry;
}
QVector<ElementaryEvent> Event::getEvents(){
    return events;
}
void Event::addElementaryEvent(ElementaryEvent event){
    double newGenProb = generouseProbabiliry + event.getProbability();
    if(newGenProb>1)
        throw std::out_of_range("generouseProbabiliry more than 1");
    generouseProbabiliry = newGenProb;
    events.push_back(event);
}
void Event::remove(int index){
    if(index <0 || index>=events.size())
        return;
    generouseProbabiliry = generouseProbabiliry - events[index].getProbability();
    events.removeAt(qsizetype(index));
}
QString Event::getName(){
    return name;
}
void Event::setName(QString name){
    this->name = name;
}
bool Event::isContainElemEvent(QString name){
    for(int i = 0;i <events.size();i++){
        ElementaryEvent event = events.at(i);
        if(name == event.getName())
            return true;
    }
    return false;
}
