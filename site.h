#ifndef SITE_H
#define SITE_H

#include <QPointF>
#include <QPainter>
#include <QRectF>
#include <vector>

/**
 * @brief Classe représentant un site du diagramme
 * @class Site
 * @author Rémi Sormain
 * On définit un site à l'aide du rectangle englobant l'ellipse (boundingRectEllipse) et d'un angle qui donne l'orientation de l'ellipse.
 * Qt imposant des rectangles parrallèles à l'axe x, on ne peut pas stocker directement le rectangle orienté.
 * Ainsi, on applique une rotation à chaque fois que l'on désire obtenir et/ou dessiner des points.
 *
 */
class Site
{
public:
    Site();
    Site(QPointF topLeft, QPointF botRight, qreal _angle = 0);
    Site(QDataStream &stream);
    /**
     * @brief intersectionPoint
     * @param point
     * @return renvoie le point d'intersection entre de le segment [pointCentral,point] et l'ellipse
     */
    QPointF intersectionPoint( QPointF point);
    /**
     * @brief Ajoute un voisin au site, avec sa distance.
     * Permet d'envisager à l'avenir, pour l'étape de discrétisation, que le site génère un maximum de point près de ses voisins les plus proches.
     * @param neighbor
     * @param distance
     */
    void addNeighbors(Site *neighbor, qreal distance);
    ///Supprime l'ensemble des voisins
    inline void clearNeighbors(){neighbors.clear();}
    inline QPointF getCentralPoint() const {return centralPoint;}
    /**
     * @brief Dessine le site dans le painter
     * @param painter
     * @param drawEllipse indique si l'ellipse doit être dessinée ou non
     */
    void draw(QPainter &painter, bool drawEllipse);
    /**
     * @brief modifie l'angle de l'ellipse par rapport à l'axe x de référence pour Qt.
     * @param _angle
     */
    inline void setAngle(qreal _angle) {angle = _angle;}
    /**
     * @brief Renvoie des points appartenant au contour de l'ellipse et le point central du site.
     * Les points sont angulairement équirépartis sur la surface de l'ellipse. Le nombre de points renvoyés dépend de nmbOfPointsPerUnits et de la taille de l'ellipse.
     * Le nombre de points générés sera suffisament faible pour ne pas dépasser le nombre points maximum par unité de longueur.
     * @param nmbOfPointsPerUnits Défini le nombre de points par unité de longueur CTE_UNITE.
     * @return Un vecteur contenant au moins le centralPoint de l'ellipse
     */
    std::vector<QPointF> getPoints(int nmbOfPointsPerUnits) const;
    /**
     * @brief Renvoie des points appartenant au contour de l'ellipse et le point central du site.
     * Les points sont angulairement équirépartis sur la surface de l'ellipse. Le nombre de points renvoyés dépend de nmbOfPointsPerUnits et de la taille de l'ellipse.
     * Le nombre de points générés sera suffisament faible pour ne pas dépasser le nombre points maximum par unité de longueur.
     * Les points sont répartis de manière déterministe avec une forte concentration autour du point d"intersection entre le segment [pointCentral,point] et l'ellipse.
     * @param nmbOfPointsPerUnits Défini le nombre de points par unité de longueur CTE_UNITE.
     * @param point Point vers lequel doivent être générés la majorité des sous-sites.
     * @return Un vecteur contenant au moins le centralPoint de l'ellipse
     */
    std::vector<QPointF> getPointsOrientedTo(QPointF point, int nmbOfPointsPerUnits) const;
    /**
     * @brief Renvoie des points appartenant au contour de l'ellipse et le point central du site.
     * Les points sont angulairement équirépartis sur la surface de l'ellipse. Le nombre de points renvoyés dépend de nmbOfPointsPerUnits et de la taille de l'ellipse.
     * Le nombre de points générés sera suffisament faible pour ne pas dépasser le nombre points maximum par unité de longueur.
     * Les points sont répartis de manière aléatoire avec une forte concentration autour du point d"intersection entre le segment [pointCentral,point] et l'ellipse.
     * La répartition est une gaussienne (angulaire) avec une déviation standard de pi/3.
     * @param nmbOfPointsPerUnits Défini le nombre de points par unité de longueur CTE_UNITE.
     * @param point Point vers lequel doivent être générés la majorité des sous-sites.
     * @return Un vecteur contenant au moins le centralPoint de l'ellipse
     */
    std::vector<QPointF> getOrientedRandomPointsTo(QPointF point, int nmbOfPointsPerUnits) const;
    /**
     * @brief Écrit le site dans le flux passé en paramètre.
     * @param stream
     */
    virtual void print(QDataStream & stream) const;


private:
    QPointF centralPoint;
    QRectF boundingRectEllipse;
    qreal angle, a, b;
    std::map<Site*, qreal> neighbors;
    qreal neighborsDistanceSum = 0.0;
    bool isValid;
};

#endif // SITE_H
