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
}

void VoronoiDiscretModule::draw(QPainter * painter, QRectF clippingRect){

    CGAL::Qt::PainterOstream<DT::Geom_traits> pos(painter, clippingRect);

    painter->setPen(edgesPen);
    for(DT::Finite_edges_iterator eit = dt2.finite_edges_begin();
        eit != dt2.finite_edges_end();
        eit++){

        DT::Face& f = *(eit->first);
        int i = eit->second;
        DT::Vertex_handle vs = f.vertex(f.cw(i));
        DT::Vertex_handle vt = f.vertex(f.ccw(i));

        if(subsites[vs] != subsites[vt]){
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
    }

}



