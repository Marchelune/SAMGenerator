#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QBrush>
#include <QMenu>
#include <QStateMachine>
#include <QState>
#include <QSpinBox>
#include "zonedessin.h"


//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    virtual void closeEvent(QCloseEvent *event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    virtual void createChoixActions();
    virtual void initStateMachine();
    virtual bool abandonner();

    ZoneDessin * zone;


    QAction * actionEllipse;
    QAction * actionSelection;

    QMenu * choixAction;
    QSpinBox * choixDiscretisation;

    QState * etatGlobal;
    QState * etatEllipse;
    QState * etatSelection;
    QStateMachine * machine;




public slots :
    void nouveau();
    void open();
    void save();
    void quit();

};

#endif // MAINWINDOW_H
