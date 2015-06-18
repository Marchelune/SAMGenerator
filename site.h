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
    inline QPointF getCentralPoint() const {return centralPoint;}
    void draw(QPainter &painter, bool drawEllipse);
    inline void setAngle(qreal _angle) {angle = _angle;}
    std::vector<QPointF> getPoints(int nmbOfPoints);
    virtual void print(QDataStream & stream) const;


private:
    QPointF centralPoint;
    QRectF boundingRectEllipse;
    qreal angle;
    bool isValid;
};

#endif // SITE_H
