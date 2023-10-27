#include<ctime>
#include<filesystem>
#include<fstream>
#include<QMessageBox>
#include<QString>
#include "resultwindow.h"
#include "ui_resultwindow.h"
//Генерація за допомогою функції time(0)
ResultWindow::ResultWindow(Event ev, int n, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    generationKey = time(0);
    this->n = n;
    event = ev;
    isSaveKey = false;
    srand(generationKey); // Встановлюємо ключ генерації
    ui->setupUi(this);
    PrintSimulation(); // Проводимо симуляцію
    statistic = collectStatistic(); // Обраховуємо статистику
    printStatistic(); // Виводимо статистику
}
// Генерація із заданим ключем
ResultWindow::ResultWindow(Event ev, int n, int generationKey, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    this->n = n;
    this->generationKey = generationKey;
    event = ev;
    isSaveKey = false;
    srand(generationKey); // Встановлюємо ключ генерації
    ui->setupUi(this);
    PrintSimulation(); // Проводимо симуляцію
    statistic = collectStatistic(); // Обраховуємо статистику
    printStatistic(); // Виводимо статистику
}

ResultWindow::~ResultWindow()
{
    delete ui;
}
// Проводить симуляцію n разів та виводить у вікно.
void ResultWindow::PrintSimulation(){
    int i=n;
    while(i>0){
        ElementaryEvent cur = chooseElemEvent();
        QString name = cur.getName();
        ui->resList->addItem(name);
        --i;
    }
}
// Випадкове обрання елементарної події, в залежності від їхніх ймовірностей.
// Використовується геометричне означення ймовірності.
ElementaryEvent ResultWindow::chooseElemEvent(){
    // Випадкове обрання точки на відрізку [0;1]
    int r = rand();
    double position = static_cast<double>(r)/RAND_MAX;//RAND_MAX=32767
    ElementaryEvent choosen("",0); // обрана подія
    double currentPosition = 0;
    // Проходимось по всіх подіях, приймаючи їх ймовірності за довжини інтервалів та відповідне зміщення.
    for(int i = 0;i<event.getEvents().size();i++){
        choosen = event.getEvents().at(i);
        currentPosition += choosen.getProbability();
        if(currentPosition >= position) // Якщо обрана точка потраплє в інтервал, то обираємо подію
            return choosen;
    }
    // Якщо точка не потрапила в жоден з інтервалів,задаємо значення
    if(currentPosition < position){
        choosen.setName("none of entered events happened!");
        choosen.setProbability(1-currentPosition);
    }
    return choosen;
}
// Вивід інформації про обрану елементарну подію у QMessageBox
void ResultWindow::on_resList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){ // Якщо не знайдено обрану елементарну подію, виходимо із функції
        QMessageBox::warning(this,"warning","Обрану елементару подію не знайдено!");
        return;
    }
    // Вивід інформації
    ElementaryEvent ev = event.getEvents().at(index);
    QMessageBox::information(this,"information","Ймовірніть події " + ev.getName() + ":\n"
                                + QString::number(ev.getProbability()) );
}
// Повертає індекс елементарної події в випадковій події. -1 якщо не знайдено
int  ResultWindow::indexByName(QString name){
    for(int i = 0;i <event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        if(name == ev.getName())
            return i;
    }
    return -1;
}
// Змінює статус збереження ключа при кожному натиску на протилежний
void ResultWindow::on_saveKey_clicked()
{
    isSaveKey = !isSaveKey;
    if(isSaveKey)
        ui->keyStatus->setText("Ключ генерації буде збережено");
    else
        ui->keyStatus->setText("Ключ генерації не буде збережено");
}
// Виводить статистику моделювання у вікно
void ResultWindow::printStatistic(){
    QList <QString> keys = statistic.keys();
    QString currText =  ui->statisticInformation->text();
    for(int i = 0;i< keys.size();i++){
        ui->statisticInformation->setText(currText +'\n'+ keys.at(i) + " настала "+ QString::number(statistic[keys.at(i)])+
                                          " разів");
        currText =  ui->statisticInformation->text();
    }
    QString min = findMinEvent();
    QString max = findMaxEvent();
    ui->statisticInformation->setText(currText +'\n' + "Подія, яка відбувалась найбільше:\n" +
                                        max + ' ' + QString::number(statistic[max]) +" разів"+'\n' +
                                      "Подія, яка відбувалась найменше:\n" +
                                        min+ ' ' + QString::number(statistic[min])+" разів");

}
// Підраховує кількість настання кожної елементарної події.
QMap<QString,int> ResultWindow::collectStatistic(){
    QMap<QString,int> statistic;
    // Додавання всіх елементарних подій
    for(int i = 0;i<event.getEvents().size();i++){
        ElementaryEvent curEv = event.getEvents().at(i);
        QString currEventName = curEv.getName();
        statistic.insert(currEventName,0);
    }
    // Підрахунок кількості входжень
    for(int i = 0;i < ui->resList->count();i++){
        QString currEventName = ui->resList->item(i)->text();
        statistic[currEventName] = statistic[currEventName] + 1;
    }
    return statistic;
}
// Повертає ім'я елементарної події, яка настала менше всього разів
QString ResultWindow::findMinEvent(){
    QList <QString> keys = statistic.keys();
    QString curMinName = keys.at(0);
    int curMinValue = statistic[curMinName];
    for(int i = 1;i< keys.size();i++){
        if(statistic[keys.at(i)] < curMinValue)   {
            curMinName = keys.at(i);
            curMinValue = statistic[keys.at(i)];
        }
    }
    return curMinName;
}
// Повертає ім'я елементарної події, яка настала більше всього разів
QString ResultWindow::findMaxEvent(){
    QList <QString> keys = statistic.keys();
    QString curMaxName = keys.at(0);
    int curMaxValue = statistic[curMaxName];
    for(int i = 1;i< keys.size();i++){
        if(statistic[keys.at(i)] > curMaxValue)   {
            curMaxName = keys.at(i);
            curMaxValue = statistic[keys.at(i)];
        }
    }
    return curMaxName;
}
// Повертає ключ генерації
int ResultWindow::getGenerationKey(){
    return generationKey;
}
// Повертає чи потрібно зберігати ключ генерації
bool ResultWindow::getIsSavekey(){
    return isSaveKey;
}
// Повертає QMap зі статистикою проведеного моделювання
QMap<QString,int> ResultWindow::getStatistic(){
    return statistic;
}

void ResultWindow::on_saveTxt_clicked()
{
    // Перехід до потрібного імені файлу
    int i = 0;
    while(std::filesystem::exists(event.getName().toStdString()+"_"+QString::number(i).toStdString()+".txt")){
        ++i;
    }
    //Створення нового файлу з результатом
    std::ofstream file;
    file.open(event.getName().toStdString()+"_"+QString::number(i).toStdString()+".txt");
    if (!file.is_open()) { // Print error if can not open
        QMessageBox::critical(this,"error","File can not be opened!");
    }
    else {
        // обрахування статистичних даних
        QString minEvent = findMinEvent();
        QString maxEvent = findMaxEvent();
        // Запис даних у файл
        file<<"Назва модельованої події: "<<event.getName().toStdString()<<'\n';
        file<<"Моделювання було проведено "<<n<<" разів.\n";
        for(int i=0;i<statistic.size();i++){
            file<<"подія " + statistic.keys().at(i).toStdString() +" настала " +
               QString::number(statistic[statistic.keys()[i]]).toStdString() +" разів.\n";
        }
        file<<"Подія " + minEvent.toStdString() + " настала менше всього разів: " +
                QString::number(statistic[minEvent]).toStdString() + '\n';
        file<<"Подія " + maxEvent.toStdString() + " настала більше всього разів: " +
                QString::number(statistic[maxEvent]).toStdString();
    }
    file.close();
}

// Зберігає результати моделювання у файлі формату csv
void ResultWindow::on_saveCsv_clicked()
{
    // Перехід до потрібного імені файлу
    int i = 0;
    while(std::filesystem::exists(event.getName().toStdString()+"_"+QString::number(i).toStdString()+".csv")){
        ++i;
    }
    //Створення нового файлу з результатом
    std::ofstream file;
    file.open(event.getName().toStdString()+"_"+QString::number(i).toStdString()+".csv");
    if (!file.is_open()) { // Print error if can not open
        QMessageBox::critical(this,"error","File can not be opened!");
    }
    else {
        // Запис результатів у файл
        file<<"event name;number of occurrences\n";
        for(int i=0;i<statistic.size();i++){
            file<<statistic.keys().at(i).toStdString() +';' +
               QString::number(statistic[statistic.keys()[i]]).toStdString() + '\n';
        }
    }
    file.close();
}

