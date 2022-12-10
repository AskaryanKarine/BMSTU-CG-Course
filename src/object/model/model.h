#ifndef MODEL_H
#define MODEL_H

#include "material.h"
#include "object.h"

class Model : public Object {
public:
    Model() = default;
    explicit Model(Material m)
        : _mat(m)
    {
    }
    virtual ~Model() = default;

    Material get_material() { return _mat; };
    void set_material(Material m) { _mat = m; };
    virtual QVector3D get_center() = 0;

    virtual std::tuple<bool, double, QVector3D> rayIntersect(const QVector3D& origin, const QVector3D& direction) = 0;

    virtual std::shared_ptr<Model> clone() = 0;

protected:
    Material _mat;
};

#endif // MODEL_H
