#ifndef ZONEDESSIN_H
#define ZONEDESSIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QBrush>
#include <QString>
#include <list>
#include <QPen>
#include "forme.h"
#include "site.h"
#include "voronoidiscretmodule.h"

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
    virtual void recompute();
    ///indique en combien de sites les ellipses doivent être discrétisées.
    int nmbSubsites;
    ///true si le dessin actuel a été sauvé
    bool sauvegarde;

    QPointF point1;
    QPointF point2;
    QPointF point3;

    QPen ellipsePen, marquageSelectionPen;

    VoronoiDiscretModule vdm;

    std::list<Site> listeSites;
    ///forme temporaire qui est en train d'être dessinée
    QPainterPath tempForme;
    ///Site temporaire en train d'être desssiné
    Site tempSite;

    QPainterPath marquageSelection;
    Forme * selectionCourante;


signals:

public slots:
    ///Premier clic pour placer un site
    void clicSiteSlot();
    ///Crée l'ellipse temporaire à tracer lorsque l'on déplace la souris
    void moveEllipseSlot1();
    /// Fixe la forme de l'ellipse
    void relacher1EllipseSlot();
    /// Change l'angle de l'ellipse
    void moveEllipseSlot2();
    /// Fixe l'ellipse correctement orientée
    void clicFinalEllipseSlot();


    void changeSubsitesNmbSlot(int n);

    ///Clic pour selectionner
    void clic1Selection();
    ///Déplacer la selection
    void moveSelection();
    ///Relacher la selection
    void relacherSelection();


};

#endif // ZONEDESSIN_H
