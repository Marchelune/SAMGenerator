#ifndef VORONOIDISCRETMODULE_H
#define VORONOIDISCRETMODULE_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <vector>
#include <map>
#include "site.h"

/**
 * @brief The voronoiDiscretModule class
 * Réalise le calcul du diagramme de Voronoi d'ellipses en discrétisant chaque ellipse
 */

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;
typedef K::Segment_2 Segment_2;
typedef K::Ray_2 Ray_2;
typedef K::Line_2 Line_2;
typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT,AT,AP>                                    VD;

class VoronoiDiscretModule
{
public:
    VoronoiDiscretModule(){};
    ///ajoute un site au diagramme courant, avec le nombre de points voulus pour le discrétiser.
    void addSite(Site site, int subDiv =1);
    void draw(QPainter *painter , QRectF clippingRect);

private:
    DT dt2;
    ///Conserve la correspondance entre les sites principaux (ellipses) et les sites secondaires discrétisés
    std::map<DT::Vertex_handle , Point_2 > subsites;
    QPen edgesPen = QPen(Qt::black);

};

#endif // VORONOIDISCRETMODULE_H
