#ifndef ZONEDESSIN_H
#define ZONEDESSIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QBrush>
#include <QString>
#include <list>
#include <QPen>
#include "site.h"
#include "voronoidiscretmodule.h"



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
    //virtual Forme * findAround(QPoint point);
    //virtual void marquerSelection();
    void getDiagramLines();
    void recompute();
    ///indique en combien de sites les ellipses doivent être discrétisées.
    int nmbSubsites;
    ///true si le dessin actuel a été sauvé
    bool sauvegarde;
    bool vueEllipse = true;
    bool vueSousSites = false;
    bool vueBorduresSousSites = false;
    bool useSmartDiscretisation = false;

    QPointF point1;
    QPointF point2;
    QPointF point3;

    QPen ellipsePen, marquageSelectionPen;

    VoronoiDiscretModule vdm;

    std::list<Site> listeSites;
    std::list<QVector<QLineF>> diagramLines;
    std::list<QPolygonF> cellules;
    ///forme temporaire qui est en train d'être dessinée
    QPainterPath tempForme;
    ///Site temporaire en train d'être desssiné
    Site tempSite;

    QPainterPath marquageSelection;



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

    void vueEllipsesSlot(bool b){vueEllipse =b;update();}
    void vueSousSitesSlot(bool b){vueSousSites = b;update();}
    void vueBorduresSousSitesSlot(bool b){vueBorduresSousSites = b;update();}
    void changeSubsitesDensitySlot(int n);

    void selectSimpleRepartitionSlot();
    void selectSmartRandomizedRepartitionSlot();

    ///Clic pour selectionner
    void clic1Selection();
    ///Déplacer la selection
    void moveSelection();
    ///Relacher la selection
    void relacherSelection();


};

#endif // ZONEDESSIN_H
