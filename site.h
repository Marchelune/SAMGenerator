#ifndef SITE_H
#define SITE_H

#include <QPointF>
#include <QPainter>
#include <QRectF>


class Site
{
public:
    Site();
    Site(QPointF topLeft, QPointF botRight, qreal _angle = 0);
    inline QPointF getCentralPoint() const {return centralPoint;}
    void draw(QPainter &painter, bool drawEllipse);
    inline void setAngle(qreal _angle) {angle = _angle;}


private:
    QPointF centralPoint;
    QRectF boundingRectEllipse;
    qreal angle;
};

#endif // SITE_H
