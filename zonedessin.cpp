#include "zonedessin.h"
#include <QCursor>
#include <QPainter>
#include <QPen>
#include <QCursor>
#include <list>
#include <iostream>
#include "Transitions.h"
#include <QDebug>
#include <QPainterPath>
#include <QFileDialog>
#include <QTransform>
#include "forme.h"





ZoneDessin::ZoneDessin(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(700,700);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->setCursor(Qt::CrossCursor);
    this->show();



    depart = QPoint(0,0);
    arrivee = QPoint(0,0);
    selectionCourante = nullptr;

    tempForme = QPainterPath();

    centerPen = QPen(Qt::black);
    centerPen.setWidth(5);
    ellipsePen = QPen(Qt::blue);


    marquageSelection = QPainterPath();
    marquageSelectionPen = QPen();
    marquageSelectionPen.setColor(Qt::gray);
    marquageSelectionPen.setWidth(5);
    marquageSelectionPen.setStyle(Qt::DotLine);

    sauvegarde = true;

    setMouseTracking(true);


}

void ZoneDessin::paintEvent(QPaintEvent *e){
    QPainter painter(this);

    std::list<Forme>::iterator it;
    for(it = listeFormes.begin(); it != listeFormes.end(); it++){
        painter.setPen(it->getPen());
        painter.drawPath(it->getForme());
    }
    //dessine la forme courante
    painter.setPen(ellipsePen);
    painter.drawPath(tempForme);

    painter.setPen(marquageSelectionPen);
    marquageSelectionPen.setDashOffset(3.0);
    painter.drawPath(marquageSelection);


}

Forme * ZoneDessin::findAround(QPoint point){

    int tailleZone= 8;
    QRect rect = QRect();
    int facteur = 1;
    while (facteur*tailleZone < MAX_PRECISION_SELECTION ){
        rect = QRect(point.x() - (int) (facteur*tailleZone) / 2, point.y() - (int) (facteur*tailleZone) / 2,
                     facteur*tailleZone, facteur*tailleZone );

        std::list<Forme>::reverse_iterator it;
        for(it = listeFormes.rbegin() ; it != listeFormes.rend() ; it++ ){
            if (it->getForme().intersects(rect)) return &(*it);
        }

        facteur++; //Si on ne trouve rien, on agrandit la zone de recherche avec le facteur
    }
    return nullptr;

}


void ZoneDessin::clic1TraitSlot(){
    depart = cursorPos(this);
    this->setCursor(Qt::CrossCursor);

    sauvegarde = false;

    //On désactive la sélection
    selectionCourante = nullptr;
    marquageSelection = QPainterPath();
    tempForme = QPainterPath();

}


void ZoneDessin::clic2EllipseSlot(){
    arrivee = cursorPos(this);
    QPainterPath forme = QPainterPath();
    forme.addEllipse(QRectF(QRect(depart,arrivee)));
    listeFormes.push_back(Forme(forme, ellipsePen));
    tempForme= QPainterPath();

    update();
}
void ZoneDessin::moveEllipseSlot(){
    arrivee = cursorPos(this);
    tempForme = QPainterPath();
    tempForme.addEllipse(QRectF(QRect(depart,arrivee)));

    update();
}


void ZoneDessin::save(QString fileName){

    QFile file(fileName);
    if (file.open((QIODevice::WriteOnly | QIODevice::Text )  )){
        QDataStream stream (&file);
        stream << qint32(listeFormes.size());
        std::list<Forme>::const_iterator it;
        for(it=listeFormes.begin(); it != listeFormes.end();it++){
            it->print(stream);
        }
        file.close();
    }
    sauvegarde = true;
}
void ZoneDessin::clear(){
    tempForme = QPainterPath();
    listeFormes.clear();

    selectionCourante = nullptr;
    marquageSelection = QPainterPath();

    update();
}

void ZoneDessin::openFromFile(QString fileName){

    clear();

    QFile file(fileName);
    QDataStream stream (&file);
    int size;

    if ( file.open( QIODevice::ReadOnly ) ){
        stream >> size;

        int i;
        QPainterPath path;
        QPen pen;

        for(i=0; i< size; i++){
            pen = QPen();
            path = QPainterPath();
            stream >> path;
            stream >> pen;
            listeFormes.push_back(Forme(path, pen));
        }
        file.close();
    }

    sauvegarde = true;
    update();
}

void ZoneDessin::clic1Selection(){

    this->setCursor(Qt::OpenHandCursor);
    depart = cursorPos(this);
    sauvegarde = false;
    tempForme = QPainterPath();
    selectionCourante = findAround(cursorPos(this));
    marquerSelection();
    update();

}

void ZoneDessin::moveSelection(){
    this->setCursor(Qt::ClosedHandCursor);
    arrivee = cursorPos(this);
    if (selectionCourante != nullptr){

        QTransform t;
        t.translate(arrivee.x()-depart.x(), arrivee.y() - depart.y());

        selectionCourante->setForme(t.map(selectionCourante->getForme()));
        marquerSelection();
    }
    depart = arrivee;

    update();
}
void ZoneDessin::relacherSelection(){

    this->setCursor(Qt::OpenHandCursor);

}
void ZoneDessin::marquerSelection(){
    if(selectionCourante != nullptr){
        marquageSelection = QPainterPath();
        marquageSelection.addEllipse(selectionCourante->getForme().boundingRect());

    }else{
        marquageSelection = QPainterPath();
    }
}
