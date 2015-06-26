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


    nmbSubsites=1;

    point1 = QPoint(0,0);
    point2 = QPoint(0,0);


    tempForme = QPainterPath();
    tempSite = Site();

    ellipsePen = QPen(Qt::blue);


    marquageSelection = QPainterPath();
    marquageSelectionPen = QPen();
    marquageSelectionPen.setColor(Qt::gray);
    marquageSelectionPen.setWidth(5);
    marquageSelectionPen.setStyle(Qt::DotLine);

    sauvegarde = true;

    vdm = VoronoiDiscretModule();

    setMouseTracking(true);


}

void ZoneDessin::getDiagramLines(){
    diagramLines.clear();
    std::map<Site*,QVector<QLineF>> result = vdm.getEdges(QRectF(this->rect()));

    for(auto& site : result){
        diagramLines.push_back(site.second);
    }
    QPainterPath path;
    for(auto lines :diagramLines ){
        path = QPainterPath();
        for(auto line : lines){
            path.moveTo(line.p1());
            path.lineTo(line.p2());
        }
        cellules.push_back(path.toFillPolygon());
    }

}

void ZoneDessin::paintEvent(QPaintEvent *e){
    QPainter painter(this);

    painter.setRenderHint(painter.Antialiasing);
    //dessine le diagramme
    vdm.draw(&painter, this->rect(),vueSousSites, vueBorduresSousSites);
//    for(auto lines : diagramLines){
//        painter.drawLines(lines);
//    }


//    for(auto cellule : cellules){
//        painter.drawPolygon(cellule);
//        QPainterPath path = QPainterPath();
//        path.addRect(this->rect());
//        QPainterPath path2 = QPainterPath();
//        path2.addPolygon(cellule);
//        path = path.intersected(path2);
//        painter.fillPath(path,Qt::blue);
//    }


    //dessine les sites
    std::list<Site>::iterator siteIt;
    for(siteIt = listeSites.begin(); siteIt != listeSites.end(); siteIt++){
        siteIt->draw(painter, vueEllipse);
    }

    //dessine la forme courante
    painter.setPen(ellipsePen);
    painter.drawPath(tempForme);
    //dessine le site courant
    tempSite.draw(painter,true);

    painter.setPen(marquageSelectionPen);
    marquageSelectionPen.setDashOffset(3.0);
    painter.drawPath(marquageSelection);


}

//Forme * ZoneDessin::findAround(QPoint point){

//        int tailleZone= 8;
//        QRect rect = QRect();
//        int facteur = 1;
//        while (facteur*tailleZone < MAX_PRECISION_SELECTION ){
//            rect = QRect(point.x() - (int) (facteur*tailleZone) / 2, point.y() - (int) (facteur*tailleZone) / 2,
//                         facteur*tailleZone, facteur*tailleZone );

//            std::list<Forme>::reverse_iterator it;
//            for(it = listeFormes.rbegin() ; it != listeFormes.rend() ; it++ ){
//                if (it->getForme().intersects(rect)) return &(*it);
//            }

//            facteur++; //Si on ne trouve rien, on agrandit la zone de recherche avec le facteur
//        }
//    return nullptr;

//}


void ZoneDessin::clicSiteSlot(){
    point1 = cursorPos(this);


    sauvegarde = false;

    //On désactive la sélection
//    selectionCourante = nullptr;
//    marquageSelection = QPainterPath();
    tempForme = QPainterPath();

}


void ZoneDessin::relacher1EllipseSlot(){
    tempForme = QPainterPath();
    point2 = cursorPos(this);
    tempSite = Site(point1, point2,0);

    update();
}
void ZoneDessin::moveEllipseSlot1(){
    point2 = cursorPos(this);
    tempForme = QPainterPath();
    tempForme.addEllipse(QRectF(point1, point2));

    update();
}

void ZoneDessin::moveEllipseSlot2(){
    point3 = cursorPos(this);
    QLineF tempLine = QLineF(tempSite.getCentralPoint(),point3);
    tempForme = QPainterPath();
    tempForme.moveTo(tempSite.getCentralPoint());
    tempForme.lineTo(point3);
    tempSite.setAngle(- tempLine.angle());
    update();
}
void ZoneDessin::clicFinalEllipseSlot(){
    point3 = cursorPos(this);
    tempForme = QPainterPath();
    QLineF tempLine = QLineF(tempSite.getCentralPoint(),point3);
    tempSite.setAngle(- tempLine.angle());

    listeSites.push_back(tempSite);
    vdm.addSite(&listeSites.back(),nmbSubsites,useSmartDiscretisation);
    getDiagramLines();

    tempSite = Site();

    update();
}
void ZoneDessin::recompute(){
    vdm.clear();
    for(auto site : listeSites){
        vdm.addSite(&site,nmbSubsites,useSmartDiscretisation);
    }
    getDiagramLines();
    update();

}


void ZoneDessin::changeSubsitesDensitySlot(int n){
    nmbSubsites = n;
    vdm.recompute(nmbSubsites, useSmartDiscretisation);
    getDiagramLines();
    update();
}

void ZoneDessin::selectSimpleRepartitionSlot(){
    useSmartDiscretisation = false;
    vdm.recompute(nmbSubsites,useSmartDiscretisation);
    update();
}

void ZoneDessin::selectSmartRandomizedRepartitionSlot(){
    useSmartDiscretisation = true;
    vdm.recompute(nmbSubsites,useSmartDiscretisation);
    update();
}

void ZoneDessin::save(QString fileName){

    QFile file(fileName);
    if (file.open((QIODevice::WriteOnly | QIODevice::Text )  )){
        QDataStream stream (&file);
        stream << qint32(listeSites.size());
        std::list<Site>::const_iterator it;
        for(it=listeSites.begin(); it != listeSites.end();it++){
            it->print(stream);
        }
        file.close();
    }
    sauvegarde = true;
}
void ZoneDessin::clear(){
    tempForme = QPainterPath();
    listeSites.clear();
    vdm.clear();
//    selectionCourante = nullptr;
//    marquageSelection = QPainterPath();
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
        for(i=0; i< size; i++){
            listeSites.push_back(Site(stream));
        }
        file.close();
    }

    for(auto& site : listeSites){
        vdm.addSite(&site, nmbSubsites,useSmartDiscretisation);
    }

    sauvegarde = true;
    update();
}

void ZoneDessin::clic1Selection(){

//    this->setCursor(Qt::OpenHandCursor);
//    point1 = cursorPos(this);
//    sauvegarde = false;
//    tempForme = QPainterPath();
//    selectionCourante = findAround(cursorPos(this));
//    marquerSelection();
//    update();

}

void ZoneDessin::moveSelection(){
//    this->setCursor(Qt::ClosedHandCursor);
//    point2 = cursorPos(this);
//    if (selectionCourante != nullptr){

//        QTransform t;
//        t.translate(point2.x()-point1.x(), point2.y() - point1.y());

//        selectionCourante->setForme(t.map(selectionCourante->getForme()));
//        marquerSelection();
//    }
//    point1 = point2;

//    update();
}
void ZoneDessin::relacherSelection(){

    //this->setCursor(Qt::OpenHandCursor);

}
//void ZoneDessin::marquerSelection(){
//    if(selectionCourante != nullptr){
//        marquageSelection = QPainterPath();
//        marquageSelection.addEllipse(selectionCourante->getForme().boundingRect());

//    }else{
//        marquageSelection = QPainterPath();
//    }
//}
