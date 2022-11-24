#ifndef MODEL_H
#define MODEL_H

#include "material.h"
#include "object.h"
#include <QVector3D>

class Model : public Object {
public:
    Model() = default;
    explicit Model(Material m)
        : _mat(m)
    {
    }
    virtual ~Model() = default;

    Material get_material();
    void set_material(Material m);

    virtual bool rayIntersect(const QVector3D& origin, const QVector3D& direction, double& t) = 0;

protected:
    Material _mat;
};

#endif // MODEL_H
