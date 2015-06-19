#include "voronoidiscretmodule.h"
#include <CGAL/Qt/Converter.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Object.h>
#include <QPen>



void VoronoiDiscretModule::addSite(Site site, int subDiv){

    CGAL::Qt::Converter<K> cv;
    std::vector<Point_2> points;
    std::vector<QPointF> qtPoints = site.getPoints(subDiv);
    std::list<DT::Vertex_handle> subsitesList;

    unsigned int i;
    Point_2 centralPoint = cv(qtPoints[0]);
    for (i = 0; i < qtPoints.size() ; i++){
        DT::Vertex_handle vh = dt2.insert(cv(qtPoints[i]));
        subsites[vh] = centralPoint;
    }

    allPoints.insert(allPoints.begin(), qtPoints.begin(), qtPoints.end());
}

void VoronoiDiscretModule::drawDelaunay(QPainter *painter){
    std::set<std::pair<Point_2,Point_2>>::iterator it;
    CGAL::Qt::Converter<K> cv;

    for(it = delaunayGraph.begin(); it != delaunayGraph.end() ; it++){
        painter->setPen(delaunayPen);
        std::cout << "Du coup on dessine la ligne " << it->first <<" et "<<it->second <<std::endl;
        painter->drawLine(cv(it->first), cv(it->second));
    }
}

void VoronoiDiscretModule::drawSubsites(QPainter *painter){
    std::list<QPointF>::iterator it;
    painter->setPen(vertexPen);
    for(it = allPoints.begin(); it != allPoints.end(); it++){
        painter->drawPoint(*it);
    }
}

void VoronoiDiscretModule::draw(QPainter * painter, QRectF clippingRect, bool drawSubsitesBool, bool drawEntireSubsites){

    CGAL::Qt::PainterOstream<DT::Geom_traits> pos(painter, clippingRect);
    //delaunayGraph.clear();
    painter->setPen(edgesPen);
    for(DT::Finite_edges_iterator eit = dt2.finite_edges_begin();
        eit != dt2.finite_edges_end();
        eit++){

        DT::Face& f = *(eit->first);
        int i = eit->second;
        DT::Vertex_handle vs = f.vertex(f.cw(i));
        DT::Vertex_handle vt = f.vertex(f.ccw(i));

        if(subsites[vs] == subsites[vt] && !drawEntireSubsites) continue;

//        delaunayGraph.insert(std::pair<Point_2,Point_2>(subsites[vs], subsites[vt]));
//        delaunayGraph[subsites[vs]] = subsites[vt];
//        std::cout << "On lie " << subsites[vs] << " et " << subsites[vt] << std::endl;

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

    // drawDelaunay(painter);

    if (drawSubsitesBool) drawSubsites(painter);

}



