#include "site.h"
#include <cmath>
#include <QPen>
#include <QLineF>



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
