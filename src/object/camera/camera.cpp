#include "camera.h"

Camera::Camera()
{
}

Camera::Camera(QVector3D pos, QVector3D dir)
    : _position(pos)
    , _direction(dir)
{
}

void Camera::transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate)
{
    _position += move;
}

QVector3D Camera::get_position()
{
    return _position;
}

void Camera::set_position(QVector3D pos)
{
    _position = pos;
}

QVector3D Camera::get_direction()
{
    return _direction;
}

void Camera::set_direction(QVector3D dir)
{
    _direction = dir;
}
