#ifndef PICTURE_H
#define PICTURE_H

#include <QImage>
#include <memory>

class Picture {
public:
    Picture();

    void set_height(int height);
    void set_width(int width);
    void set_maxDepth(int max_depth);

private:
    int _height;
    int _width;
    int _maxDepth;

    //    std::vector<Model> _figures;
    //    std::vector<BaseLight> _lights;
    //    Camera _cam;
};

#endif // PICTURE_H
