#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "model.h"

class Polyhedron {
public:
    Polyhedron();

private:
    QVector3D _center;
    std::vector<QVector3D> _points;
};

#endif // POLYHEDRON_H
