#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<fstream>
#include <QListWidgetItem>
#include "event.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_saveTxt_clicked();

private slots:
    void on_generationKeysList_itemDoubleClicked(QListWidgetItem *item);

private slots:
    int indexByName(QString name);
    bool isContainEvent(Event ev);
    void saveGenerationKey(QString eventName, int key);
    void on_exit_triggered();
    void on_addEvent_clicked();
    void on_eventsList_itemDoubleClicked(QListWidgetItem *item);
    void on_model_clicked();
    QList<QMap<QString,int>> collectStatisticFromSimulations(Event ev,int n, int amountModel);
    void on_deleteElemEvent_clicked();
    void reprintList();
    QMap<QString,int> summModelResults( QList<QMap<QString,int>> simulationStatistic);
    QMap<QString,double> averageModelResult(QMap<QString,int> summModelResult, int simulationAmount);
    QMap<QString,double> percentageModelResult(QMap<QString,int> summModelResult, int n, int amountModel);
    QString findMinEvent(QMap<QString,int> summModelResult);
    QString findMaxEvent(QMap<QString,int> summModelResult);
private:

    Ui::MainWindow *ui;
    QVector<Event> events;
};
#endif // MAINWINDOW_H
