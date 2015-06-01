#include "forme.h"
#include <QPen>
#include <QPainterPath>
#include<QDebug>

Forme::Forme(QPainterPath _forme, QPen _pen)
{
    forme = _forme ;
    pen = _pen;
}

void Forme::print(QDataStream &stream) const{

    stream << forme;

    stream << pen;

}
