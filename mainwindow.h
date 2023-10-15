#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    int indexByName(QString name);
    bool isContainEvent(Event ev);
    void on_exit_triggered();
    void on_addEvent_clicked();
    void on_eventsList_itemDoubleClicked(QListWidgetItem *item);

    void on_model_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Event> events;
};
#endif // MAINWINDOW_H
