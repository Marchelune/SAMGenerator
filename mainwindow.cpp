#include "mainwindow.h"
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QDebug>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QAbstractButton>
#include <QPixmap>
#include <QToolButton>
#include <QSlider>
#include <QStateMachine>
#include <QState>
#include <QColorDialog>
#include <QToolBar>
#include "Transitions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setStatusBar(statusBar());

    QMenuBar * menuBar = this->menuBar( );
    QMenu * fileMenu = menuBar->addMenu( tr ("&File") );

    QAction * newAction = new QAction( QIcon(":icons/new.png"), tr("&New..."), this);
    newAction->setShortcut( tr("Ctrl+N")); // accélérateur clavier
    newAction->setToolTip( tr("New file")); // bulle d’aide
    newAction->setStatusTip( tr("New file")); // barre de statut
    fileMenu->addAction(newAction); // rajouter l’action au menu déroulant
    connect(newAction, SIGNAL(triggered( )), this, SLOT(nouveau( ))); // connecter le signal à un slot de this

    QAction * openAction = new QAction( QIcon(":icons/open.png"), tr("&Open..."), this);
    openAction->setShortcut( tr("Ctrl+O")); // accélérateur clavier
    openAction->setToolTip( tr("Open a file")); // bulle d’aide
    openAction->setStatusTip( tr("Open a file")); // barre de statut
    fileMenu->addAction(openAction); // rajouter l’action au menu déroulant
    connect(openAction, SIGNAL(triggered( )), this, SLOT(open( ))); // connecter le signal à un slot de this

    QAction * saveAction = new QAction( QIcon(":icons/save.png"), tr("&Save..."), this);
    saveAction->setShortcut( tr("Ctrl+S"));
    saveAction->setToolTip( tr("Save a file"));
    saveAction->setStatusTip( tr("Save a file"));
    fileMenu->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered( )), this, SLOT(save( )));

    QAction * quitAction = new QAction( QIcon(":icons/quit.png"), tr("&Quit..."), this);
    quitAction->setShortcut( tr("Ctrl+Q"));
    quitAction->setToolTip( tr("Quit"));
    quitAction->setStatusTip( tr("Quit"));
    fileMenu->addAction(saveAction);
    connect(quitAction, SIGNAL(triggered( )), this, SLOT(quit( )));

    //Création du menu pour dessiner des sites ou sélectioner des sites  -------------------------------------------
    createChoixActions();
    QToolButton * boutonAction = new QToolButton(this);
    boutonAction->setMenu(choixAction);
    boutonAction->setIcon(QIcon(":/icons/pencil.png"));
    boutonAction->setPopupMode(QToolButton::InstantPopup);
    boutonAction->setToolTip("Changer la forme du tracé");
    boutonAction->setStatusTip("Changer la forme du tracé");

    //Création de la toolbar ------------------------------------------------------------------------
    QToolBar * toolBar = this->addToolBar( tr("File") );
    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(quitAction);
    toolBar->addWidget(boutonAction);

    zone = new ZoneDessin(this);
    setCentralWidget(zone);

    //Initialisation de la machine à état ---------------------------------------------------------
    initStateMachine();
    setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::nouveau()
{
    if(abandonner()) zone->clear();
}

void MainWindow::quit()
{
    if(abandonner())
    {
        exit(0);
    }
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/home", tr("Draw file (*.draw)") );
    zone->save(fileName);
}

void MainWindow::open()
{
    if(abandonner()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home" );
        zone->openFromFile(fileName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(abandonner())
    {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::createChoixActions(){

    QActionGroup * group = new QActionGroup (this);

    actionEllipse = group->addAction(QIcon(":/icons/ellipse.png"), tr("Ellipses"));
    actionEllipse->setToolTip(tr("Tracer des ellipses"));
    actionEllipse->setStatusTip(tr("Tracer des ellipses"));
    actionEllipse->setCheckable(true);

    actionSelection = group->addAction(QIcon("://icons/hand.png"), tr("Selection"));
    actionSelection->setToolTip(tr("Selectionner des figures"));
    actionSelection->setStatusTip(tr("Cliquez pour sélectionner un tracé"));
    actionSelection->setCheckable(true);

    choixAction = new QMenu(tr("Forme"));
    choixAction->addAction(actionSelection);
    choixAction->addAction(actionEllipse);


}

void MainWindow::initStateMachine(){
    machine = new QStateMachine();
    etatGlobal = new QState();
    etatEllipse = new QState(etatGlobal);
    etatSelection = new QState(etatGlobal);


    machine->addState(etatGlobal);
    machine->setInitialState(etatGlobal);
    etatGlobal->setInitialState(etatEllipse);

    //Gestion du tracé d'ellipses
    QState * etatEllipse1 = new QState(etatEllipse);
    QState * etatEllipse2 = new QState(etatEllipse);
    QState * etatEllipse3 = new QState(etatEllipse);
    etatEllipse->setInitialState(etatEllipse1);

    //Gestion de la sélection
    QState * SelectionInit = new QState(etatSelection);
    QState * SelectionMove = new QState(etatSelection);
    etatSelection->setInitialState(SelectionInit);



    machine->start();



    etatSelection->addTransition(actionEllipse, SIGNAL(triggered()), etatEllipse);

    addMouseTrans(etatEllipse1,etatEllipse2,this,QEvent::MouseButtonPress,Qt::LeftButton,zone, SLOT(clicSiteSlot()));
    addMouseTrans(etatEllipse2,etatEllipse2,this,QEvent::MouseMove,Qt::NoButton,zone,SLOT(moveEllipseSlot1()));
    addMouseTrans(etatEllipse2,etatEllipse3,this,QEvent::MouseButtonRelease,Qt::LeftButton,zone,SLOT(relacher1EllipseSlot()));
    addMouseTrans(etatEllipse3,etatEllipse3,this,QEvent::MouseMove,Qt::NoButton,zone,SLOT(moveEllipseSlot2()));
    addMouseTrans(etatEllipse3,etatEllipse1,this,QEvent::MouseButtonRelease,Qt::LeftButton,zone,SLOT(clicFinalEllipseSlot()));

    etatEllipse->addTransition(actionSelection, SIGNAL(triggered()), etatSelection );

    addMouseTrans(SelectionInit,SelectionMove, this, QEvent::MouseButtonPress,Qt::LeftButton,zone, SLOT(clic1Selection()));
    addMouseTrans(SelectionMove, SelectionMove, this, QEvent::MouseMove,Qt::NoButton,zone, SLOT(moveSelection()));
    addMouseTrans(SelectionMove,SelectionInit, this, QEvent::MouseButtonRelease,Qt::LeftButton,zone, SLOT(relacherSelection()));


}

bool MainWindow::abandonner(){

    if(zone->isSaved()) return true;

    QMessageBox msgBox;
    msgBox.setText(tr("Voulez vous vraiment abandonner votre travail ? "));
    QAbstractButton* pButtonYes = (QAbstractButton *) msgBox.addButton(tr("Oui"), QMessageBox::YesRole);
    msgBox.addButton(tr("Non"), QMessageBox::NoRole);

    msgBox.exec();

    if(msgBox.clickedButton() == pButtonYes)
    {
        return true;
    }

    return false;
}
