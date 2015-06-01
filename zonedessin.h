#ifndef ZONEDESSIN_H
#define ZONEDESSIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QBrush>
#include <QString>
#include "forme.h"
#include <list>
#include <QPen>
//#include <QtQuick>

static const int MAX_PRECISION_SELECTION = 60;



class ZoneDessin : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneDessin(QWidget *parent = 0);

    virtual void save(QString fileName);
    virtual void openFromFile(QString fileName);
    virtual void clear();
    inline bool isSaved() {return sauvegarde;}

protected:
    void paintEvent(QPaintEvent * e);


private:
    virtual Forme * findAround(QPoint point);
    virtual void marquerSelection();

    ///true si le dessin actuel a été sauvé
    bool sauvegarde;

    QPoint depart;
    QPoint arrivee;

    QPen centerPen, ellipsePen, marquageSelectionPen;

    std::list<Forme> listeFormes;
    ///forme temporaire qui est en train d'être dessinée
    QPainterPath tempForme;

    QPainterPath marquageSelection;
    Forme * selectionCourante;


signals:

public slots:
    ///Premier clic pour tracer un site
    void clic1TraitSlot();
    ///Créer l'ellipse temporaire à tracer lorsque l'on déplace la souris bouton gauche enfoncé
    void moveEllipseSlot();
    ///Créer l'ellipse finale lorsque l'on relache la souris
    void clic2EllipseSlot();

    ///Clic pour selectionner
    void clic1Selection();
    ///Déplacer la selection
    void moveSelection();
    ///Relacher la selection
    void relacherSelection();


};

#endif // ZONEDESSIN_H
