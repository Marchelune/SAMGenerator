#ifndef VORONOIDISCRETMODULE_H
#define VORONOIDISCRETMODULE_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <vector>
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
typedef CGAL::Delaunay_triangulation_2<K> Delaunay_triangulation_2;

class VoronoiDiscretModule
{
public:
    VoronoiDiscretModule(){};
    VoronoiDiscretModule(QRect _clippingRect);
    ///ajoute un site au diagramme courant, avec le nombre de points voulus pour le discrétiser.
    void addSite(Site site, int subDiv =1);
    void draw(QPainter *painter );

private:
    QRect clippingRect;
    Delaunay_triangulation_2 dt2;
    QPen edgesPen = QPen(Qt::black);

};

#endif // VORONOIDISCRETMODULE_H
