#ifndef SPHERE_H
#define SPHERE_H

#include "model.h"

class Sphere : public Model {
public:
    Sphere();
    Sphere(const QVector3D& center, const double& radius, const Material& m);
    ~Sphere() {};

    std::tuple<bool, double, QVector3D> rayIntersect(const QVector3D& origin, const QVector3D& direction) override;
    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override;

    double get_radius();
    void set_radius(double r);

    QVector3D get_center() override;
    void set_center(QVector3D center);

private:
    double _rad;
    QVector3D _center;
};

#endif // SPHERE_H
