#include "voronoidiscretmodule.h"
#include <CGAL/Qt/Converter.h>
#include <CGAL/Qt/PainterOstream.h>
#include <CGAL/Object.h>
#include <QPen>

VoronoiDiscretModule::VoronoiDiscretModule(QRect _clippingRect) : clippingRect(_clippingRect)
{
}

void VoronoiDiscretModule::addSite(Site site, int subDiv){

    CGAL::Qt::Converter<K> cv;
    std::vector<Point_2> points;

    if(subDiv == 1){
        Point_2 centralPoint = cv(site.getCentralPoint());
        points.push_back(centralPoint);
    }




    dt2.insert(points.begin(),points.end());
}

void VoronoiDiscretModule::draw(QPainter * painter){

    CGAL::Qt::PainterOstream<Delaunay_triangulation_2::Geom_traits> pos(painter, clippingRect);

    painter->setPen(edgesPen);
      for(Delaunay_triangulation_2::Finite_edges_iterator eit = dt2.finite_edges_begin();
          eit != dt2.finite_edges_end();
          eit++){
        CGAL::Object o = dt2.dual(eit);
        Delaunay_triangulation_2::Segment s;
        Delaunay_triangulation_2::Geom_traits::Ray_2 r;
        Delaunay_triangulation_2::Geom_traits::Line_2 l;
        if(CGAL::assign(s,o)){
          pos << s;
        } else if(CGAL::assign(r,o)) {
          pos << r;
        }else if(CGAL::assign(l,o)) {
          pos << l;
        }
      }

}
