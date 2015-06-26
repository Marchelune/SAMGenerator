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
#include <QVector>
#include <vector>
#include <map>
#include <set>
#include "site.h"

/**
 * @brief Réalise le calcul du diagramme de Voronoi d'ellipses en discrétisant chaque ellipse
 * @class VoronoiDiscretModule
 *
 *
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
    VoronoiDiscretModule(){vertexPen.setWidth(3);};
    ///ajoute un site au diagramme courant, avec le nombre de points voulus pour le discrétiser (1 par défaut)
    void addSite(Site * site, int subDivDensity =1, bool useSmartDiscretisation = false);
    ///Dessine le diagramme dans le QPainter. Le diagramme est délimité par le clippingRect.
    void draw(QPainter *painter , QRectF clippingRect, bool drawSubsitesBool, bool drawEntireSubsites);
    /**
     * @brief Dessine les sous-sites discrétisant les ellipses
     * @param painter
     */
    void drawSubsites(QPainter * painter);
    /**
     * @brief Dessine la triangulation des sites principaux (ellipses)
     * @param painter
     * @warning Doit être appelé après la fonction draw. NON FONCTIONNEL
     */
    void drawDelaunay(QPainter *painter);
    ///Efface le diagramme courant
    void clear();
    /**
     * @brief Permet de recalculer le diamgramme avec de nouvelles valeurs de densité de sous_site ou une autre répartition de ces sous-sites
     * @param subDivDensity
     * @param useSmartDiscretisation
     */
    void recompute(int subDivDensity, bool useSmartDiscretisation = false);
    /**
     * @brief Renvoie les segments délimitant les cellules de Voronoi
     * @param clippingRect
     * @deprecated N'est pas utilisé ici car pas optimal, il faudrait trouver un moyen d'ordonner la liste des arêtes pour ne former qu'un seul polygone tracable par Qt
     */
    std::map<Site*,QVector<QLineF>> getEdges(QRectF clippingRect);

private:
    DT dt2;
    ///Conserve la correspondance entre les sites principaux (ellipses) et les sites secondaires discrétisés
    std::map<DT::Vertex_handle , Site* > subsites;
    std::map<Point_2,Point_2> delaunayGraph;
    std::list<QPointF> allPoints;
    std::list<Site*> allSites;
    QPen edgesPen = QPen(Qt::black);
    QPen vertexPen = QPen(Qt::black);
    QPen delaunayPen = QPen(Qt::green);
    /**
     * @brief meanPoint
     * Point moyen des sites principaux (ellipses). L'algorithme par sous-sites orientés et randomisés place un maximum de sous-site du diamgramme vers ce point moyen
     * @see getOrientedRandomPointsTo
     */
    QPointF meanPoint;

};

#endif // VORONOIDISCRETMODULE_H
