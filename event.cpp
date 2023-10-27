#include "event.h"
#include<QString>
#include<vector>


// Додає елементарну подію. Кидає виключення std::out_of_range якщо загальна ймовірність > 1
void Event::addElementaryEvent(ElementaryEvent event){
    double newGenProb = generouseProbabiliry + event.getProbability();
    if(newGenProb>1)
        throw std::out_of_range("generouseProbabiliry more than 1");
    generouseProbabiliry = newGenProb;
    events.push_back(event);
}
// Видаляє елементарну подію
void Event::remove(int index){
    if(index <0 || index>=events.size())
        return;
    generouseProbabiliry = generouseProbabiliry - events[index].getProbability();
    events.removeAt(qsizetype(index));
}
// Повертає назву випадкової події
QString Event::getName(){
    return name;
}
// Задати назву
void Event::setName(QString name){
    this->name = name;
}
// Чи належить випадковій події елементарна подія з заданим ім'ям
bool Event::isContainElemEvent(QString name){
    for(int i = 0;i <events.size();i++){
        ElementaryEvent event = events.at(i);
        if(name == event.getName())
            return true;
    }
    return false;
}
// Повертає суму ймовірностей всіх елементарних подій
double Event::getGenerouseProbabiliry(){
    return generouseProbabiliry;
}
// Повертає вектор елементарних подій, які належать випадковій події
QVector<ElementaryEvent> Event::getEvents(){
    return events;
}
