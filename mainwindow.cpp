#include <QMessageBox>
#include<fstream>
#include<ctime>
#include<filesystem>
#include"addelementaryevents.h"
#include "resultwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elementaryevent.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ElementaryEvent ev("ffff",0.5);
    Event e;
    e.setName("ev");
    e.addElementaryEvent(ev);
    events.push_back(e);
    ui->eventsList->addItem(e.getName());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_exit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_addEvent_clicked()
{
    if(ui->enterEventName->text().isEmpty()){
        QMessageBox::warning(this,"warning","Назва події не може бути порожнім!");
        return;
    }
    if(ui->enterEventName->text().contains(';')){
        QMessageBox::warning(this,"warning","Назва події не може містити ';'!");
        return;
    }
    Event event;
    event.setName(ui->enterEventName->text());
    if(!isContainEvent(event)){
        events.push_back(event);
        ui->eventsList->addItem(event.getName());
        AddElementaryEvents addWindow(event);
        addWindow.setModal(true);
        addWindow.exec();
        events[events.size()-1] = addWindow.getEvent();
    }else{
        QMessageBox::warning(this,"warning","Р†Рј'СЏ РєРѕР¶РЅРѕС— РІРёРїР°РґРєРѕРІРѕС— РїРѕРґС–С— РјР°С” Р±СѓС‚Рё СѓРЅС–РєР°Р»СЊРЅРёРј!");
    }
    //ElementaryEvent ev(ui->amountModel->text(),0.5);
    //ev.setName(ui->amountModel->text());
    //ev.setProbability(0.5);
}


void MainWindow::on_eventsList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","РћР±СЂР°РЅРѕ РЅРµРјРѕР¶Р»РёРІРёР№ РµР»РµРјРµРЅС‚ СЃРїРёСЃРєСѓ!");
        return;
    }
    Event event = events.at(index);
    AddElementaryEvents addWindow(event);
    addWindow.setModal(true);
    addWindow.exec();
    events[index] = addWindow.getEvent();

}
// Проводить моделювання для обраної випадкової величини
void MainWindow::on_model_clicked()
{
    // Якщо не обрано подію, моделювання не проводиться
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Оберіть подію для моделювання!");
        return;
    }
    // Зчитування кількості моделювань
    QString amountModelsStr = ui->amountModel->text();
    int index = indexByName(ui->eventsList->currentItem()->text());
    Event ev = events[index];
    // Якщо обрана подія порожня, моделювання не проводиться
    if(ev.getEvents().size()==0){
        QMessageBox::critical(this,"error","Обрана подія порожня!");
        return;
    }
    int amountModels = amountModelsStr.toDouble();
    QString generationKeyStr = ui->enterGenerationKey->text();
    int key = generationKeyStr.toDouble();
    if(amountModels <= 0){// Якщо введена к-ть моделювань менше 1, моделювання не проводиться
        QMessageBox::critical(this,"error","Кількість моделювань не може бути менше одиниці!");
        return;
    }else{
        if(generationKeyStr.isEmpty()){
            ResultWindow res(ev,amountModels);
            res.setModal(true);
            res.exec();
            if(res.getIsSavekey()) // Зберігаємо ключ генерації, якщо це необхідно
                saveGenerationKey(ev.getName(),res.getGenerationKey());
        }else{
            ResultWindow res(ev, amountModels, key);
            res.setModal(true);
            res.exec();
            if(res.getIsSavekey()) // Зберігаємо ключ генерації, якщо це необхідно
                saveGenerationKey(ev.getName(),res.getGenerationKey());
        }
    }
}
bool MainWindow::isContainEvent(Event ev){
    for(int i = 0;i <events.size();i++){
        Event event = events.at(i);
        if(ev.getName() == event.getName())
            return true;
    }
    return false;
}
int MainWindow::indexByName(QString name){
    for(int i = 0;i <events.size();i++){
        Event event = events.at(i);
        if(name == event.getName())
            return i;
    }
    return -1;
}

void MainWindow::on_deleteElemEvent_clicked()
{
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","РЎРїРѕС‡Р°С‚РєСѓ РѕР±РµСЂС–С‚СЊ РµР»РµРјРµРЅС‚Р°СЂРЅСѓ РїРѕРґС–СЋ");
        return;
    }
    int index = indexByName(ui->eventsList->currentItem()->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","РћР±СЂР°РЅРѕ РЅРµРјРѕР¶Р»РёРІРёР№ РµР»РµРјРµРЅС‚ СЃРїРёСЃРєСѓ!");
        return;
    }
    events.remove(index);
    reprintList();
}

void MainWindow::reprintList(){
    ui->eventsList->clear();
    for(int i =0;i<events.size();i++){
        Event ev = events.at(i);
        QString name = ev.getName();
        ui->eventsList->addItem(name);
    }
}
// Збирає статистику за n проведених симуляцій для обраної події по amountModel моделювань
QList<QMap<QString,int>> MainWindow:: collectStatisticFromSimulations(Event ev,int n, int amountModel){
    srand(time(0));
    QList<QMap<QString,int>> stat;
    for(int i =0;i<n;i++){
        ResultWindow res(ev,amountModel,rand());
        stat.push_back(res.getStatistic());
    }
    return stat;
}
void  MainWindow::saveGenerationKey(QString eventName,int key){
    QString simulationName = eventName +"_Generation_key: " + QString::number(key);
    ui->generationKeysList->addItem(simulationName);
    return;
}

void MainWindow::on_generationKeysList_itemDoubleClicked(QListWidgetItem *item)
{
    QMessageBox::information(this,"information",item->text());
}


void MainWindow::on_saveTxt_clicked()
{
    QList<QMap<QString,int>> stat;
    // Якщо не обрано подію, моделювання не проводиться
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Оберіть подію для моделювання!");
        return;
    }
    // Зчитування кількості моделювань
    QString nStr = ui->setNModelsLineEdit->text();
    int index = indexByName(ui->eventsList->currentItem()->text());
    Event ev = events[index];
    // Якщо обрана подія порожня, моделювання не проводиться
    if(ev.getEvents().size()==0){
        QMessageBox::critical(this,"error","Обрана подія порожня!");
        return;
    }
    int n = nStr.toDouble();
    QString amountModelStr = ui->amountModel->text();
    int amountModel = amountModelStr.toDouble();
    if(n <= 0){// Якщо введена к-ть моделювань менше 1, моделювання не проводиться
        QMessageBox::critical(this,"error","Кількість моделювань не може бути менше одиниці!");
        return;
    }else{
        if(amountModel == 0)
            amountModel = n;
        stat = collectStatisticFromSimulations(ev, n, amountModel); // Збір статистики за n cимуляцій по amountModel моделювань
    }
    // Перехід до потрібного імені файлу
    int i = 0;
    while(std::filesystem::exists(ev.getName().toStdString()+"_statistic_"+QString::number(i).toStdString()+".txt")){
        ++i;
    }
    //Створення нового файлу з результатом
    std::ofstream file;
    file.open(ev.getName().toStdString()+"_statistic_"+QString::number(i).toStdString()+".txt");
    if (!file.is_open()) { // Print error if can not open
        QMessageBox::critical(this,"error","File can not be opened!");
    }
    else {
        // обрахування статистичних даних
        QMap<QString,int> summModelResult = summModelResults(stat);
        QMap<QString,double> averageModelsResult = averageModelResult(summModelResult,n);
        QMap<QString,double> percentageModelsResult = percentageModelResult(summModelResult,n,amountModel);
        QString minEvent = findMinEvent(summModelResult);
        QString maxEvent = findMaxEvent(summModelResult);
        // Вивід даних у файл
        file<<"Назва модельованої події: "<<ev.getName().toStdString()<<'\n';
        file<<"Симуляцію було проведено "<<n<<" разів по "<< amountModel<<" Моделювань\n";
        file<<"Загальна кількість настання кожної події:\n";
        for(int i=0;i<summModelResult.size();i++){
            file<<"Подія " + summModelResult.keys().at(i).toStdString() +" настала " +
               QString::number(summModelResult[summModelResult.keys()[i]]).toStdString() +" разів.\n";
        }
        file<<"Середня кількість настання кожної події:\n";
        for(int i=0;i<averageModelsResult.size();i++){
            file<<"Подія " + averageModelsResult.keys().at(i).toStdString() +" відбувалась в середньому " +
               QString::number(averageModelsResult[averageModelsResult.keys()[i]]).toStdString() +" разів.\n";
        }
        file<<"Відсоток настання кожної події:\n";
        for(int i=0;i<percentageModelsResult.size();i++){
            file<<"подія " + percentageModelsResult.keys().at(i).toStdString() +" настала у " +
               QString::number(percentageModelsResult[percentageModelsResult.keys()[i]]).toStdString() +" частині випробувань.\n";
        }
        file<<"Подія " + minEvent.toStdString() + " настала менше всього разів: " +
                QString::number(summModelResult[minEvent]).toStdString() + '\n';
        file<<"Подія " + maxEvent.toStdString() + " настала більше всього разів: " +
                QString::number(summModelResult[maxEvent]).toStdString();
    }
    file.close();
}
// Обраховує загальну кількість настання кожної події в усіх моделюваннях
QMap<QString,int> MainWindow::summModelResults( QList<QMap<QString,int>> simulationStatistic){
    QMap<QString,int> summModelResult;
    for(int i = 0;i<simulationStatistic.size();i++){
        for(int j = 0; j <simulationStatistic[i].size();j++){
            if(!summModelResult.contains(simulationStatistic[i].keys()[j]))
                summModelResult.insert(simulationStatistic[i].keys()[j],simulationStatistic[i][simulationStatistic[i].keys()[j]]);
            else{
                summModelResult[simulationStatistic[i].keys()[j]] = summModelResult[simulationStatistic[i].keys()[j]] +
                                                                    simulationStatistic[i][simulationStatistic[i].keys()[j]];
            }
        }
    }
    return summModelResult;
}
// Обраховує середню кількість настання кожної події в усіх моделюваннях
QMap<QString,double> MainWindow::averageModelResult(QMap<QString,int> summModelResult, int simulationAmount){
    QMap<QString,double>res;
    for(int i =0;i<summModelResult.size();i++){
        res.insert(summModelResult.keys()[i],static_cast<double>(summModelResult[summModelResult.keys()[i]])/simulationAmount);
    }
    return res;
}
// Обраховує відсоток настання кожної події в середньому для усіх моделюваннях
QMap<QString,double> MainWindow::percentageModelResult(QMap<QString,int> summModelResult, int n, int amountModel ){
    QMap<QString,double> res;
    for(int i =0;i<summModelResult.size();i++){
        res.insert(summModelResult.keys()[i],static_cast<double>(summModelResult[summModelResult.keys()[i]])/n/amountModel);
    }
    return res;
}
// Повертає ім'я події, яка настала менше всього разів
QString MainWindow::findMinEvent(QMap<QString,int> summModelResult){
    QList <QString> keys = summModelResult.keys();
    QString curMinName = keys[0];
    int curMinValue = summModelResult[curMinName];
    for(int i = 1;i< keys.size();i++){
        if(summModelResult[keys.at(i)] < curMinValue)   {
            curMinName = keys[i];
            curMinValue = summModelResult[keys[i]];
        }
    }
    return curMinName;
}
// Повертає ім'я події, яка настала більше всього разів
QString MainWindow::findMaxEvent(QMap<QString,int> summModelResult){
    QList <QString> keys = summModelResult.keys();
    QString curMaxName = keys[0];
    int curMaxValue = summModelResult[curMaxName];
    for(int i = 1;i< keys.size();i++){
        if(summModelResult[keys.at(i)] > curMaxValue)   {
            curMaxName = keys[i];
            curMaxValue = summModelResult[keys[i]];
        }
    }
    return curMaxName;
}

void MainWindow::on_saveCsv_clicked()
{
    QList<QMap<QString,int>> stat;
    // Якщо не обрано подію, моделювання не проводиться
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Оберіть подію для моделювання!");
        return;
    }
    // Зчитування кількості моделювань
    QString nStr = ui->setNModelsLineEdit->text();
    int index = indexByName(ui->eventsList->currentItem()->text());
    Event ev = events[index];
    // Якщо обрана подія порожня, моделювання не проводиться
    if(ev.getEvents().size()==0){
        QMessageBox::critical(this,"error","Обрана подія порожня!");
        return;
    }
    int n = nStr.toDouble();
    QString amountModelStr = ui->amountModel->text();
    int amountModel = amountModelStr.toDouble();
    if(n <= 0){// Якщо введена к-ть моделювань менше 1, моделювання не проводиться
        QMessageBox::critical(this,"error","Кількість моделювань не може бути менше одиниці!");
        return;
    }else{
        if(amountModel == 0)
            amountModel = n;
        stat = collectStatisticFromSimulations(ev, n, amountModel); // Збір статистики за n cимуляцій по amountModel моделювань
    }
    // Перехід до потрібного імені файлу
    int i = 0;
    while(std::filesystem::exists(ev.getName().toStdString()+"_statistic_"+QString::number(i).toStdString()+".csv")){
        ++i;
    }
    //Створення нового файлу з результатом
    std::ofstream file;
    file.open(ev.getName().toStdString()+"_statistic_"+QString::number(i).toStdString()+".csv");
    if (!file.is_open()) { // Print error if can not open
        QMessageBox::critical(this,"error","File can not be opened!");
    }
    else {
        // обрахування статистичних даних
        QMap<QString,int> summModelResult = summModelResults(stat);
        QMap<QString,double> averageModelsResult = averageModelResult(summModelResult,n);
        QMap<QString,double> percentageModelsResult = percentageModelResult(summModelResult,n,amountModel);
        // Вивід даних у файл
        file<<"event name;generous number of occurrences;average number of occurrences;proportion of occurrences\n";
        for(int i=0;i<summModelResult.size();i++){
            QString currName = summModelResult.keys().at(i);
            file<< currName.toStdString()+ ";" + QString::number(summModelResult[currName]).toStdString() +";"+
                        QString::number(averageModelsResult[averageModelsResult.keys()[i]]).toStdString() +";"+
                        QString::number(percentageModelsResult[percentageModelsResult.keys()[i]]).toStdString() +";\n";
        }
    }
    file.close();
}

