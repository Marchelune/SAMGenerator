#ifndef FORME_H
#define FORME_H

#include <QPainterPath>
#include <QPen>
#include <QLine>
#include <QRect>
#include <QDataStream>

/**
 *\class Forme
 *
 * \brief Classe modélisant une forme graphique quelconque (Ligne, rectangle etc) contenue dans un QPainterPath
 *
 * Permet de manipuler indifferement tout type de forme
 *
 */
class Forme
{
public:
    Forme(QPainterPath _forme, QPen _pen);
    inline  QPainterPath getForme() {return forme;}
    inline void setForme(QPainterPath _forme) { forme = _forme;}
    inline QPen getPen() {return pen;}
    ///renvoie un pointeur vers le QPen afin de le modifier.
    inline QPen * changePen() {return &pen;}
    ///sérialise la forme dans un QDataStream
    virtual void print(QDataStream & stream) const;

private :
    QPainterPath forme;
    QPen pen;

};

#endif // FORME_H
