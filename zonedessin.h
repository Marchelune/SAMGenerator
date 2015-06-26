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


/**
 * @brief Widget qui permet l'affichage et le tracer du diagramme.
 * Cette classe reçoit les signaux de tracer (clics de souris divers)
 */
class ZoneDessin : public QWidget
{
    Q_OBJECT
public:
    explicit ZoneDessin(QWidget *parent = 0);
    ///sauvegarde le diagramme courant
    virtual void save(QString fileName);
    ///ouvre le diagramme courant depuis un fichier
    virtual void openFromFile(QString fileName);
    ///créer un nouveau diagramme vide
    virtual void clear();
    ///@return true si le diagramme a été sauvegardé et non modifié depuis.
    inline bool isSaved() {return sauvegarde;}

protected:
    void paintEvent(QPaintEvent * e);


private:
    /**
     * @brief Récupère les segments séparant les cellules des ellipses
     * @deprecated N'est pas utilisé ici car pas optimal, il faudrait trouver un moyen d'ordonner la liste des arêtes pour ne former qu'un seul polygone tracable par Qt
     * L'idéal serait de récupérer un QPolygon par cellule que la classe ZoneDessin serait charger de tracer.
     * Pour l'instant, c'est la classe VoronoiDiscreteModule qui dessine le diagramme.
     */
    void getDiagramLines();
    ///indique en combien de sites les ellipses doivent être discrétisées.
    int subsitesDensity;
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

    /**
     * @brief slot de sélection
     * @warning NON FONCTIONNEL
     */
    void clic1Selection();
    /**
     * @brief slot de dépalcement de la sélection
     * @warning NON FONCTIONNEL
     */
    void moveSelection();
    /**
     * @brief slot au relachement de la sélection
     * @warning NON FONCTIONNEL
     */
    void relacherSelection();


};

#endif // ZONEDESSIN_H
