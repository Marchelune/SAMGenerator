#ifndef SITE_H
#define SITE_H

#include <QPointF>
#include <QPainter>
#include <QRectF>
#include <vector>

class Site
{
public:
    Site();
    Site(QPointF topLeft, QPointF botRight, qreal _angle = 0);
    Site(QDataStream &stream);
    /**
     * @brief intersectionPoint
     * @param point
     * @return renvoie le point d'intersection entre de la droite pointCentral-point et l'ellipse
     */
    QPointF intersectionPoint( QPointF point);
    void addNeighbors(Site *neighbor, qreal distance);
    inline void clearNeighbors(){neighbors.clear();}
    inline QPointF getCentralPoint() const {return centralPoint;}
    void draw(QPainter &painter, bool drawEllipse);
    inline void setAngle(qreal _angle) {angle = _angle;}
    std::vector<QPointF> getPoints(int nmbOfPointsPerUnits);
    std::vector<QPointF> getPointsOrientedTo(QPointF point, int nmbOfPointsPerUnits);
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
