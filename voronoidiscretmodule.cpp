#include "voronoidiscretmodule.h"
#include <CGAL/Qt/Converter.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Object.h>
#include <QPen>



void VoronoiDiscretModule::addSite(Site* site, int subDivDensity, bool useSmartDiscretisation){

    CGAL::Qt::Converter<K> cv;
    std::vector<QPointF> qtPoints;

    allSites.push_back(site);
    if(dt2.number_of_vertices() > 0){
        meanPoint += site->getCentralPoint();
        meanPoint /= 2.0;
        if(useSmartDiscretisation){
            qtPoints = site->getOrientedRandomPointsTo(meanPoint, subDivDensity);
        }else{
            qtPoints = site->getPoints(subDivDensity);
        }
    }else{
        qtPoints = site->getPoints(subDivDensity);
        meanPoint = qtPoints[0];
    }

    unsigned int i;
    for (i = 0; i < qtPoints.size() ; i++){
        DT::Vertex_handle vh = dt2.insert(cv(qtPoints[i]));
        subsites[vh] = site;
    }

    allPoints.insert(allPoints.begin(), qtPoints.begin(), qtPoints.end());
}

void VoronoiDiscretModule::clear(){
    dt2.clear();
    allPoints.clear();
    subsites.clear();
    allSites.clear();
}

void VoronoiDiscretModule::recompute(int subDivDensity, bool useSmartDiscretisation){
    allPoints.clear();
    subsites.clear();
    dt2.clear();
    CGAL::Qt::Converter<K> cv;
    std::vector<QPointF> qtPoints;

    for(Site* site : allSites){
        qtPoints.clear();
        if(useSmartDiscretisation){
            qtPoints = site->getOrientedRandomPointsTo(meanPoint, subDivDensity);
        }else{
            qtPoints = site->getPoints(subDivDensity);
        }
        unsigned int i;
        for (i = 0; i < qtPoints.size() ; i++){
            DT::Vertex_handle vh = dt2.insert(cv(qtPoints[i]));
            subsites[vh] = site;
        }
        allPoints.insert(allPoints.begin(), qtPoints.begin(), qtPoints.end());
    }

}

void VoronoiDiscretModule::drawDelaunay(QPainter *painter){
    std::map<Point_2,Point_2>::iterator it;

    CGAL::Qt::Converter<K> cv;
    for(it = delaunayGraph.begin(); it != delaunayGraph.end() ; it++){
        painter->setPen(delaunayPen);
        painter->drawLine(cv(it->first), cv(it->second));
    }
}

std::map<Site*,QVector<QLineF> >  VoronoiDiscretModule::getEdges(QRectF clippingRect){

    std::map<Site*,QVector<QLineF> > result;
    //initialisation
    for(auto site : allSites){
        result[site] = QVector<QLineF>();
    }

    CGAL::Qt::Converter<K> cv(clippingRect);
    for(DT::Finite_edges_iterator eit = dt2.finite_edges_begin();
        eit != dt2.finite_edges_end();
        eit++){

        DT::Face& f = *(eit->first);
        int i = eit->second;
        DT::Vertex_handle vs = f.vertex(f.cw(i));
        DT::Vertex_handle vt = f.vertex(f.ccw(i));

        if(subsites[vs] == subsites[vt]) continue;

        CGAL::Object o = dt2.dual(eit);
        DT::Segment s;
        DT::Geom_traits::Ray_2 r;
        DT::Geom_traits::Line_2 l;
        if(CGAL::assign(s,o)){
            result[subsites[vs]].push_back(cv(s));
        } else if(CGAL::assign(r,o)) {
            result[subsites[vs]].push_back(cv(r));
        }else if(CGAL::assign(l,o)) {
            result[subsites[vs]].push_back(cv(l));
        }
    }

    return result;
}

void VoronoiDiscretModule::drawSubsites(QPainter *painter){
    std::list<QPointF>::iterator it;
    painter->setPen(vertexPen);
    for(it = allPoints.begin(); it != allPoints.end(); it++){
        painter->drawPoint(*it);
    }

    QPen  temp =  QPen(Qt::green);
    temp.setWidth(5);
    painter->setPen(temp);
    painter->drawPoint(meanPoint);
}

void VoronoiDiscretModule::draw(QPainter * painter, QRectF clippingRect, bool drawSubsitesBool, bool drawEntireSubsites){

    CGAL::Qt::PainterOstream<DT::Geom_traits> pos(painter, clippingRect);
//    delaunayGraph.clear();
    painter->setPen(edgesPen);
    for(DT::Finite_edges_iterator eit = dt2.finite_edges_begin();
        eit != dt2.finite_edges_end();
        eit++){

        DT::Face& f = *(eit->first);
        int i = eit->second;
        DT::Vertex_handle vs = f.vertex(f.cw(i));
        DT::Vertex_handle vt = f.vertex(f.ccw(i));

        if(subsites[vs] == subsites[vt] && !drawEntireSubsites) continue;


//--------DELAUNAY - A REVOIR - NON FONCTIONNEL
//        CGAL::Qt::Converter<K> cv(clippingRect);
//        delaunayGraph[cv(subsites[vs]->getCentralPoint())] = cv(subsites[vt]->getCentralPoint());
//        delaunayGraph[cv(subsites[vt]->getCentralPoint())] = cv(subsites[vs]->getCentralPoint());

        CGAL::Object o = dt2.dual(eit);
        DT::Segment s;
        DT::Geom_traits::Ray_2 r;
        DT::Geom_traits::Line_2 l;
        if(CGAL::assign(s,o)){
            pos << s;
        } else if(CGAL::assign(r,o)) {
            pos << r;
        }else if(CGAL::assign(l,o)) {
            pos << l;
        }
    }

//    drawDelaunay(painter);

    if (drawSubsitesBool) drawSubsites(painter);

}



