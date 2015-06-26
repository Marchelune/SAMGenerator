#include "site.h"
#include <cmath>
#include <QPen>
#include <QLineF>
#include <QTransform>
#include <iostream>
#include <string>

static const float CTE_PI = 3.14159265359;
static const qreal CTE_UNITE = 500.0;

Site::Site(QPointF topLeft, QPointF botRight, qreal _angle){

    boundingRectEllipse = QRectF(topLeft,botRight);
    angle = _angle;
    centralPoint = boundingRectEllipse.center();
    isValid = true;
    a = abs(boundingRectEllipse.width() / (double) 2.0);
    b = abs(boundingRectEllipse.height() / (double) 2.0);

}

Site::Site(){
    centralPoint = QPointF();
    boundingRectEllipse = QRectF();
    angle= (qreal) 0;
    isValid = false;
}

Site::Site(QDataStream &stream){
    centralPoint = QPointF();
    stream >> centralPoint;
    boundingRectEllipse = QRectF();
    stream >> boundingRectEllipse;
    stream >> angle;
    stream >> isValid;

    a = abs(boundingRectEllipse.width() / (double) 2.0);
    b = abs(boundingRectEllipse.height() / (double) 2.0);

}

void Site::print(QDataStream &stream) const{
    stream << centralPoint;
    stream << boundingRectEllipse;
    stream << angle;
    stream << isValid;
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

QPointF Site::intersectionPoint(QPointF point){
    qreal lineAngle = QLineF(centralPoint, point).angle();
    //nécessaire car l'ellipse n'est orientée que lors du tracé dans Qt, il ne semble pas possible d'orienter un QRect
    lineAngle -= angle;
    QTransform transform;
    transform.rotate(angle);
    return transform.map(QPointF(a*cos(lineAngle), b* sin(lineAngle)));
}


void Site::addNeighbors(Site * neighbor, qreal distance){
    neighborsDistanceSum += distance;
    neighbors[neighbor] = distance;
}

std::vector<QPointF> Site::getOrientedRandomPointsTo(QPointF point, int nmbOfPointsPerUnits){
    if(!isValid) throw std::string("Attention, on tente de récupérer on lance getPointsOrientedTo sur un site non valide");

    std::vector<QPointF> points;
    points.push_back(centralPoint);
    if(nmbOfPointsPerUnits <2) return points;
    //approximation du périmètre de l'ellipse
    qreal P = CTE_PI * sqrt(2*(a*a +b*b));
    unsigned int nmbOfPoints= (unsigned int) (nmbOfPointsPerUnits * P / CTE_UNITE);

    qreal lineAngle = - QLineF(centralPoint, point).angle();
    //nécessaire car l'ellipse n'est orientée que lors du tracé dans Qt, il ne semble pas possible d'orienter un QRect
    lineAngle -= angle;
    lineAngle *= (CTE_PI /180.0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> d(lineAngle,CTE_PI/3.0);

    points.push_back(QPointF(a*cos(lineAngle), b* sin(lineAngle)));

    while(points.size() < nmbOfPoints) {
        double t = d(gen);
        points.push_back(QPointF(a*cos(t), b* sin(t)));
    }

    QTransform transform;
    transform.rotate(angle);
    unsigned int i;
    for(i=1; i < points.size(); i++){
        points[i] = transform.map(points[i]);
        points[i].setX( points[i].x() + centralPoint.x());
        points[i].setY( points[i].y() + centralPoint.y());
    }


    return points;

}

std::vector<QPointF> Site::getPointsOrientedTo(QPointF point, int nmbOfPointsPerUnits){
    if(!isValid) throw std::string("Attention, on tente de récupérer on lance getPointsOrientedTo sur un site non valide");

    std::vector<QPointF> points;
    points.push_back(centralPoint);
    if(nmbOfPointsPerUnits <2) return points;
    //approximation du périmètre de l'ellipse
    qreal P = CTE_PI * sqrt(2*(a*a +b*b));
    unsigned int nmbOfPoints= (unsigned int) (nmbOfPointsPerUnits * P / CTE_UNITE);

    qreal lineAngle = - QLineF(centralPoint, point).angle();
    //nécessaire car l'ellipse n'est orientée que lors du tracé dans Qt, il ne semble pas possible d'orienter un QRect
    lineAngle -= angle;
    lineAngle *= (CTE_PI /180.0);

    points.push_back(QPointF(a*cos(lineAngle), b* sin(lineAngle)));
    qreal alpha = CTE_PI /nmbOfPoints;
    while(points.size() < nmbOfPoints && alpha <= CTE_PI/2) {
        points.push_back(QPointF(a*cos(lineAngle+alpha), b* sin(lineAngle+alpha)));
        if(points.size() >= nmbOfPoints) break;
        points.push_back(QPointF(a*cos(lineAngle-alpha), b* sin(lineAngle-alpha)));
        alpha *= 3;
    }

    QTransform transform;
    transform.rotate(angle);
    unsigned int i;
    for(i=1; i < points.size(); i++){
        points[i] = transform.map(points[i]);
        points[i].setX( points[i].x() + centralPoint.x());
        points[i].setY( points[i].y() + centralPoint.y());
    }


    return points;

}

std::vector<QPointF> Site::getPoints(int nmbOfPointsPerUnits){

    std::vector<QPointF> points;
    points.push_back(centralPoint);
    if(nmbOfPointsPerUnits <2) return points;

    //approximation du périmètre de l'ellipse
    qreal P = CTE_PI * sqrt(2*(a*a +b*b));
    int nmbOfPoints= (int) (nmbOfPointsPerUnits * P / CTE_UNITE);

    int k=0;
    float t = 0;
    while (t < 2*CTE_PI){
        points.push_back(QPointF(a*cos(t), b* sin(t)) );
        k++;
        t= (double) 2.0 * (double) k * CTE_PI /(double) nmbOfPoints ;
    }

    QTransform transform;
    transform.rotate(angle);


    unsigned int i;
    for(i=1; i < points.size(); i++){
        points[i] = transform.map(points[i]);
        points[i].setX( points[i].x() + centralPoint.x());
        points[i].setY( points[i].y() + centralPoint.y());
    }

    return points;


}
