#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "model.h"

class Polyhedron {
public:
    Polyhedron();

    //    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override;

private:
    QVector3D _center;
    std::vector<QVector3D> _points;
};

#endif // POLYHEDRON_H
