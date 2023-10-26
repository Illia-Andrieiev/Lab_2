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
    srand(generationKey);
    ui->setupUi(this);
    PrintSimulation();
    statistic = collectStatistic();
    printStatistic();
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
    srand(generationKey);
    ui->setupUi(this);
    PrintSimulation();
    statistic = collectStatistic();
    printStatistic();
}

ResultWindow::~ResultWindow()
{
    delete ui;
}
void ResultWindow::PrintSimulation(){
    int i=n;
    while(i>0){
        ElementaryEvent cur = chooseElemEvent();
        QString name = cur.getName();
        ui->resList->addItem(name);
        --i;
    }
}
ElementaryEvent ResultWindow::chooseElemEvent(){
    int r = rand();
    double position = static_cast<double>(r)/RAND_MAX;//RAND_MAX=32767
    ElementaryEvent choosen("",0);
    double currentPosition = 0;
    for(int i = 0;i<event.getEvents().size();i++){
        choosen = event.getEvents().at(i);
        currentPosition += choosen.getProbability();
        if(currentPosition >= position)
            return choosen;
    }
    if(currentPosition < position){
        choosen.setName("Не настала жодна з заданих подій!");
        choosen.setProbability(1-currentPosition);
    }
    return choosen;
}

void ResultWindow::on_resList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = indexByName(item->text());
    if(index == -1){
        QMessageBox::warning(this,"warning","РћР±СЂР°РЅРѕ РЅРµРјРѕР¶Р»РёРІРёР№ РµР»РµРјРµРЅС‚ СЃРїРёСЃРєСѓ!");
        return;
    }
    ElementaryEvent ev = event.getEvents().at(index);
    QMessageBox::information(this,"information","РџРѕРґС–СЏ РЅР°СЃС‚Р°Р»Р° Р· Р№РјРѕРІС–СЂРЅС–СЃС‚СЋ:\n"
                                + QString::number(ev.getProbability()) );
}
int  ResultWindow::indexByName(QString name){
    for(int i = 0;i <event.getEvents().size();i++){
        ElementaryEvent ev = event.getEvents().at(i);
        if(name == ev.getName())
            return i;
    }
    return -1;
}

void ResultWindow::on_saveKey_clicked()
{
    isSaveKey = !isSaveKey;
    if(isSaveKey)
        ui->keyStatus->setText("Ключ генерації буде збережено");
    else
        ui->keyStatus->setText("Ключ генерації не буде збережено");
}
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
QMap<QString,int> ResultWindow::collectStatistic(){
    QMap<QString,int> statistic;
    for(int i = 0;i < ui->resList->count();i++){
        QString currEventName = ui->resList->item(i)->text();
        if(!statistic.contains(currEventName))
            statistic.insert(currEventName,1);
        else{
            statistic[currEventName] = statistic[currEventName] + 1;
        }
    }
    return statistic;
}
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
int ResultWindow::getGenerationKey(){
    return generationKey;
}
bool ResultWindow::getIsSavekey(){
    return isSaveKey;
}
QMap<QString,int> ResultWindow::getStatistic(){
    return statistic;
}

void ResultWindow::on_saveTxt_clicked()
{
    // Перехід до потрібного імені файлу
    int i = 0;
    while(std::filesystem::exists(event.getName().toStdString() + "/" + event.getName().toStdString()+"_"+QString::number(i).toStdString()+".txt")){
        ++i;
    }
    //Створення нового файлу з результатом
    std::ofstream file;
    file.open(event.getName().toStdString() + "/" + event.getName().toStdString()+"_"+QString::number(i).toStdString()+".txt");
    if (!file.is_open()) { // Print error if can not open
        QMessageBox::critical(this,"error","File can not be opened!");
    }
    else {
        // обрахування статистичних даних
        QString minEvent = findMinEvent();
        QString maxEvent = findMaxEvent();
        // Вивід даних у файл
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
}

