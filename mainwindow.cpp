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
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Створює та додає випадкову подію
void MainWindow::on_addEvent_clicked()
{
    // Якщо введена назва порожня, виводимо попередження та виходимо з функції
    if(ui->enterEventName->text().isEmpty()){
        QMessageBox::warning(this,"warning","Назва події не може бути порожнім!");
        return;
    }
    // ';' є недопустимим символом. Якщо він входить у задане ім'я, завершуємо роботу
    if(ui->enterEventName->text().contains(';')){
        QMessageBox::warning(this,"warning","Назва події не може містити ';'!");
        return;
    }
    Event event;
    event.setName(ui->enterEventName->text());
    if(!isContainEvent(event)){ // Якщо не існує події з введеним ім'ям, то створюємо її
        events.push_back(event);
        ui->eventsList->addItem(event.getName());
        AddElementaryEvents addWindow(event);
        addWindow.setModal(true);
        addWindow.exec();
        events[events.size()-1] = addWindow.getEvent();
    }else{
        QMessageBox::warning(this,"warning","Випадкова подія з введеним ім'ям вже існує!");
    }
}


// Відкриває вікно для редагування обраної випадкової події
void MainWindow::on_eventsList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","Не знайдено обрану подію!");
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
// Чи містить список всіх подій задану подію
bool MainWindow::isContainEvent(Event ev){
    for(int i = 0;i <events.size();i++){
        Event event = events.at(i);
        if(ev.getName() == event.getName())
            return true;
    }
    return false;
}
// Повертає індекс випадкової події. -1 якщо не знайдено
int MainWindow::indexByName(QString name){
    for(int i = 0;i <events.size();i++){
        Event event = events.at(i);
        if(name == event.getName())
            return i;
    }
    return -1;
}
// Видаляє випадкову подіюю
void MainWindow::on_deleteElemEvent_clicked()
{
    if(!ui->eventsList->currentItem()->isSelected()){
        QMessageBox::critical(this,"error","Оберіть випадкову подію!");
        return;
    }
    int index = indexByName(ui->eventsList->currentItem()->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","Не знайдено обрану подію!");
        return;
    }
    events.remove(index);
    reprintList();
}
// Виводить список всіх подій
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
// Зберігає ключ генерації даного моделювання та виводить у вікно
void  MainWindow::saveGenerationKey(QString eventName,int key){
    QString simulationName = eventName +"_Generation_key: " + QString::number(key);
    ui->generationKeysList->addItem(simulationName);
    return;
}
// Виводить інформацію про елемент списку з ключами генерації
void MainWindow::on_generationKeysList_itemDoubleClicked(QListWidgetItem *item)
{
    QMessageBox::information(this,"information",item->text());
}

// Обраховує загальну кількість настання кожної події в усіх моделюваннях
QMap<QString,int> MainWindow::summModelResults( QList<QMap<QString,int>> simulationStatistic){
    QMap<QString,int> summModelResult;
    for(int i = 0;i<simulationStatistic.size();i++){
        for(int j = 0; j <simulationStatistic[i].size();j++){
            auto it = simulationStatistic[i].cbegin();
            std::advance(it, j);
            it.key();
            QString currName = it.key();
            if(!summModelResult.contains(currName))
                summModelResult.insert(currName,simulationStatistic[i][currName]);
            else{
                summModelResult[currName] = summModelResult[currName] +
                                                                    simulationStatistic[i][currName];
            }
        }
    }
    return summModelResult;
}
// Обраховує середню кількість настання кожної події в усіх моделюваннях
QMap<QString,double> MainWindow::averageModelResult(QMap<QString,int> summModelResult, int simulationAmount){
    QMap<QString,double>res;
    for(int i =0;i<summModelResult.size();i++){
        auto it = summModelResult.cbegin();
        std::advance(it, i);
        it.key();
        QString currName = it.key();
        res.insert(currName,static_cast<double>(summModelResult[currName])/simulationAmount);
    }
    return res;
}
// Обраховує відсоток настання кожної події в середньому для усіх моделюваннях
QMap<QString,double> MainWindow::percentageModelResult(QMap<QString,int> summModelResult, int n, int amountModel ){
    QMap<QString,double> res;
    for(int i =0;i<summModelResult.size();i++){
        auto it = summModelResult.cbegin();
        std::advance(it, i);
        it.key();
        QString currName = it.key();
        res.insert(currName,static_cast<double>(summModelResult[currName])/n/amountModel);
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
// Проводить симуляцію з вказаної кількості моделювань, та записує статистичні результати у файл формату csv
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
            // Визначення іммені поточної елементарної події. За побудовою всі QMap на однаковому індексі
            // містять інформацію про одну й ту саму подію
            auto it = summModelResult.cbegin();
            std::advance(it, i);
            it.key();
            QString currName = it.key();
            file<< currName.toStdString()+ ";" + QString::number(summModelResult[currName]).toStdString() +";"+
                        QString::number(averageModelsResult[currName]).toStdString() +";"+
                        QString::number(percentageModelsResult[currName]).toStdString() +";\n";
        }
    }
    file.close();
}
// Проводить симуляцію з вказаної кількості моделювань, та записує статистичні результати у файл формату txt
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
            auto it = summModelResult.cbegin();
            std::advance(it, i);
            it.key();
            QString currName = it.key();
            file<<"Подія " + currName.toStdString() +" настала " +
               QString::number(summModelResult[currName]).toStdString() +" разів.\n";
        }
        file<<"Середня кількість настання кожної події:\n";
        for(int i=0;i<averageModelsResult.size();i++){
            auto it = averageModelsResult.cbegin();
            std::advance(it, i);
            it.key();
            QString currName = it.key();
            file<<"Подія " + currName.toStdString() +" відбувалась в середньому " +
               QString::number(averageModelsResult[currName]).toStdString() +" разів.\n";
        }
        file<<"Відсоток настання кожної події:\n";
        for(int i=0;i<percentageModelsResult.size();i++){
            auto it = percentageModelsResult.cbegin();
            std::advance(it, i);
            it.key();
            QString currName = it.key();
            file<<"подія " + currName.toStdString() +" настала у " +
               QString::number(percentageModelsResult[currName]).toStdString() +" частині випробувань.\n";
        }
        file<<"Подія " + minEvent.toStdString() + " настала менше всього разів: " +
                QString::number(summModelResult[minEvent]).toStdString() + '\n';
        file<<"Подія " + maxEvent.toStdString() + " настала більше всього разів: " +
                QString::number(summModelResult[maxEvent]).toStdString();
    }
    file.close();
}
// Виводить QMessageBox з інструкцією до програми
void MainWindow::on_action_triggered()
{
    QMessageBox::information(this,"instruction",
                             "Ця програма дозволяє додавати, редагувати випадкові події, "
                             "що складаються з елементарних подій. Проводити задану кількість моделювань, "
                             "отримуючи результати та деякі статистичні дані. Є можливість збереження результатів "
                             "у файли формату txt та csv. Можна зібрати статистичні дані за задану кількість моделювань "
                             "із заданою кількість експериментів у файли формату txt та csv. Передбачається можливість "
                             "збереження ключа генерації для проведеного моделювання. Відкрити вікно редагування "
                             "події можна двічі натиснувши на її назву у списку.");
}
// Завершує програму
void MainWindow::on_exit_triggered()
{
    QMessageBox::StandardButton ex = QMessageBox::question(this,"exit","Ви дійсно бажаєте вийти?",QMessageBox::Yes|QMessageBox::No);
    if(ex == QMessageBox::Yes)
        QApplication::quit();
}

