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
    ~ResultWindow();

private slots:
    void on_resList_itemDoubleClicked(QListWidgetItem *item);

private:
    Event event;
    int n;
    Ui::ResultWindow *ui;
    int indexByName(QString name);
    void PrintSimulation();
    ElementaryEvent chooseElemEvent();
};

#endif // RESULTWINDOW_H
