#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "model.h"
#include "polygon.h"

class Polyhedron : public Model {
public:
    Polyhedron();
    Polyhedron(const std::vector<QVector3D>& points, const std::vector<Polygon>& polygins, const Material& m);
    ~Polyhedron() {};

    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override;
    std::tuple<bool, double, QVector3D> rayIntersect(const QVector3D& origin, const QVector3D& direction) override;
    std::vector<QVector3D> get_points();

    void set_points(std::vector<QVector3D> p);
    void set_polygons(std::vector<Polygon> p);

private:
    QVector3D _center;
    std::vector<QVector3D> _points;
    std::vector<Polygon> _pols;

    void calcCenter();
};

#endif // POLYHEDRON_H
