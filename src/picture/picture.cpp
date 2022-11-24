#include "picture.h"

Picture::Picture()
{
}

Picture::Picture(int height, int width, int depth)
    : _height(height)
    , _width(width)
    , _maxDepth(depth)
{
}

void Picture::set_height(int height)
{
    _height = height;
}

int Picture::get_height()
{
    return _height;
}

void Picture::set_width(int width)
{
    _width = width;
}

int Picture::get_width()
{
    return _width;
}

void Picture::set_maxDepth(int max_depth)
{
    _maxDepth = max_depth;
}

int Picture::get_maxDepth()
{
    return _maxDepth;
}

void Picture::set_camera(Camera cam)
{
    _cam = cam;
}

Camera Picture::get_camera()
{
    return _cam;
}

std::shared_ptr<QImage> Picture::drawingFgure()
{
    _cam = Camera();
    _cam.set_direction(QVector3D(0, 0, 3000));

    std::shared_ptr<QImage> image = std::make_shared<QImage>(_width, _height, QImage::Format_RGB32);
    image->fill(Qt::black);

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            QVector3D pix(x, y, 200);
        }
    }

    return image;
}
