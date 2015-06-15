#include "site.h"
#include <cmath>
#include <QPen>
#include <QLineF>
#include <QTransform>

static const float CTE_PI = 3.14159265359;

Site::Site(QPointF topLeft, QPointF botRight, qreal _angle){

    boundingRectEllipse = QRectF(topLeft,botRight);
    angle = _angle;
    centralPoint = boundingRectEllipse.center();
    isValid = true;
}

Site::Site(){
    centralPoint = QPointF();
    boundingRectEllipse = QRectF();
    angle= (qreal) 0;
    isValid = false;
}

void Site::draw(QPainter & painter, bool drawEllipse){

    if(isValid){
        QPen pen;
        if(drawEllipse){
            pen = QPen(Qt::blue);
            //painter.save();
            painter.resetTransform();
            painter.translate(centralPoint);
            painter.rotate(angle);
            painter.setPen(pen);
            painter.drawEllipse(boundingRectEllipse.translated(-centralPoint));
            //painter.restore();
            painter.resetTransform();
        }
        pen = QPen(Qt::red);
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawPoint(centralPoint);
    }

}

std::vector<QPointF> Site::getPoints(int nmbOfPoints){

    std::vector<QPointF> points;
    if(nmbOfPoints <2) {
        points.push_back(centralPoint);
        return points;
    }

    qreal a = boundingRectEllipse.width() / (double) 2.0;
    qreal b = boundingRectEllipse.height() / (double) 2.0;

    int k=0;
    float t = 0;
    while (t < 2*CTE_PI){
        points.push_back(QPointF(a*cos(t), b* sin(t)) );
        k++;
        t= (double) 2.0 * (double) k * CTE_PI /(double) nmbOfPoints;
    }

    QTransform transform;
    transform.rotate(angle);
    //transform.translate(centralPoint.x(), centralPoint.y());


    unsigned int i = 0;
    for(i=0; i < points.size(); i++){
        points[i] = transform.map(points[i]);
        points[i].setX( points[i].x() + centralPoint.x());
        points[i].setY( points[i].y() + centralPoint.y());
    }

    return points;


}
