#ifndef OBJECT_H
#define OBJECT_H

#include <QVector3D>

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) = 0;
};

#endif // OBJECT_H
