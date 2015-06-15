#include "voronoidiscretmodule.h"
#include <CGAL/Qt/Converter.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Object.h>
#include <QPen>



void VoronoiDiscretModule::addSite(Site site, int subDiv){

    CGAL::Qt::Converter<K> cv;
    std::vector<Point_2> points;
    std::vector<QPointF> qtPoints = site.getPoints(subDiv);

    unsigned int i;
    for (i = 0; i < qtPoints.size() ; i++){
        points.push_back( cv(qtPoints[i]) );
    }


    dt2.insert(points.begin(),points.end());



}

void VoronoiDiscretModule::draw(QPainter * painter, QRectF clippingRect){

    CGAL::Qt::PainterOstream<DT::Geom_traits> pos(painter, clippingRect);

    painter->setPen(edgesPen);
    for(DT::Finite_edges_iterator eit = dt2.finite_edges_begin();
        eit != dt2.finite_edges_end();
        eit++){
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



