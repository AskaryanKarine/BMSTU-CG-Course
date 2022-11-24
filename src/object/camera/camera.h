#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

class Camera : public Object {
public:
    Camera();
    Camera(QVector3D pos, QVector3D dir);
    ~Camera() {};

    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override; // TODO

    QVector3D get_position();
    void set_position(QVector3D pos);

    QVector3D get_direction();
    void set_direction(QVector3D dir);

private:
    QVector3D _position;
    QVector3D _direction;
};

#endif // CAMERA_H
