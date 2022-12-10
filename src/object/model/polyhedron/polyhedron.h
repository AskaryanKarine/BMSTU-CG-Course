#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "model.h"
#include "polygon.h"

class Polyhedron : public Model {
public:
    Polyhedron();
    Polyhedron(const Polyhedron& p);
    Polyhedron(const std::vector<QVector3D>& points, const std::vector<Polygon>& polygins, const Material& m);
    ~Polyhedron() {};

    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override;

    std::tuple<bool, double, QVector3D> rayIntersect(const QVector3D& origin, const QVector3D& direction) override;
    std::vector<QVector3D> get_points();
    std::vector<Polygon> get_polygins();

    void set_points(std::vector<QVector3D> p);
    void set_polygons(std::vector<Polygon> p);

    QVector3D get_center() override;

    std::shared_ptr<Model> clone() override;

private:
    QVector3D _center;
    std::vector<QVector3D> _points;
    std::vector<Polygon> _pols;

    void calcCenter();
    void _rotate(const QVector3D& rotate);
    void _move(const QVector3D& move);
    void _scale(const QVector3D& scale);
};

#endif // POLYHEDRON_H
