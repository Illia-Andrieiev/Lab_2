#pragma once
#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>
#include<QListWidgetItem>
#include"event.h"
namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultWindow(Event ev,int n,QWidget *parent = nullptr);
    explicit ResultWindow(Event ev,int n, int generationKey, QWidget *parent = nullptr);
    ~ResultWindow();
    int getGenerationKey();
    bool getIsSavekey();
    QMap<QString,int> getStatistic();
private slots:
    void on_saveTxt_clicked();

private slots:
    void on_saveKey_clicked();

private slots:
    void on_resList_itemDoubleClicked(QListWidgetItem *item);

private:
    Event event;
    int n;
    QMap<QString,int> statistic;
    bool isSaveKey;
    int generationKey;
    Ui::ResultWindow *ui;
    int indexByName(QString name);
    void printStatistic();
    void PrintSimulation();
    QMap<QString,int> collectStatistic();
    QString findMinEvent();
    QString findMaxEvent();
    ElementaryEvent chooseElemEvent();
};

#endif // RESULTWINDOW_H
