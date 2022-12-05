#include "picture.h"

#include <iostream>
#include <thread>

Picture::Picture()
    : _height(100)
    , _width(100)
    , _maxDepth(3)
{
    init();
}

Picture::Picture(int height, int width, int depth)
    : _height(height)
    , _width(width)
    , _maxDepth(depth)
{
    init();
}

void Picture::init()
{
    _scene.init_scene();
    _cam.set_position(QVector3D(0, 0, 3000));
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

void Picture::set_backColor(QColor c)
{
    _scene.set_backfroundColor(c);
}

QColor Picture::get_backColor()
{
    return _scene.get_backgroundColor();
}

void Picture::set_mainSphereColor(QColor c)
{
    _scene.set_mainSphereColor(c);
}

QColor Picture::get_mainSphereColor()
{
    return _scene.get_mainSphereColor();
}

void Picture::set_refIndex(double r)
{
    _scene.set_refractiveIndex(r);
}

void Picture::set_transparient(double tr)
{
    _scene.set_trasparient(tr);
}

void Picture::move_camera(QVector3D offset)
{
    _cam.transform(offset, offset, offset);
}

QVector3D Picture::get_cam_pos()
{
    return _cam.get_position();
}

bool Picture::scene_intersect(QVector3D orig, QVector3D dir, double& t, int& closest, QVector3D& norm)
{
    double dist = std::numeric_limits<double>::max();
    auto models = _scene.get_model();
    QVector3D N;
    closest = -1;
    t = 0;
    for (size_t i = 0; i < models.size(); i++) {
        double dist_i = 0;
        if (models[i]->rayIntersect(orig, dir, dist_i, N) && dist_i < dist) {
            dist = dist_i;
            closest = i;
            norm = N;
        }
    }
    if (closest == -1)
        return false;

    t = dist;
    return true;
}

QVector3D reflect(const QVector3D& I, const QVector3D& N)
{
    return I - N * 2.f * QVector3D::dotProduct(I, N);
}

QVector3D refract(const QVector3D& I, const QVector3D& N, const double& refractive_index)
{
    double cosi = -std::max(-1.f, std::min(1.f, QVector3D::dotProduct(I, N)));
    double etai = 1, etat = refractive_index;
    QVector3D n = N;
    if (cosi < 0) {
        cosi = -cosi;
        std::swap(etai, etat);
        n = -N;
    }
    double eta = etai / etat;
    double k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? QVector3D(0, 0, 0) : I * eta + n * (eta * cosi - sqrt(k));
}

QColor getColor(Material m, double difIntency, double specIntency, QColor reflect, QColor refract)
{
    auto elemColor = [](double in, double specIn, QVector4D alb, int fCol, int LCol, int refl, int refr) {
        double c = in * fCol * alb.x() + specIn * LCol * alb.y() + refl * alb.z() + refr * alb.w();
        c = c > 255 ? 255 : c;
        return c;
    };

    QColor res;
    int Rdc, Rsc, Rrefl, Rrefr;
    int Gdc, Gsc, Grefl, Grefr;
    int Bdc, Bsc, Brefl, Brefr;
    m.get_difColor().getRgb(&Rdc, &Gdc, &Bdc);
    m.get_specColor().getRgb(&Rsc, &Gsc, &Bsc);
    reflect.getRgb(&Rrefl, &Grefl, &Brefl);
    refract.getRgb(&Rrefr, &Grefr, &Brefr);

    res.setRed(elemColor(difIntency, specIntency, m.get_albedo(), Rdc, Rsc, Rrefl, Rrefr));
    res.setGreen(elemColor(difIntency, specIntency, m.get_albedo(), Gdc, Gsc, Grefl, Grefr));
    res.setBlue(elemColor(difIntency, specIntency, m.get_albedo(), Bdc, Bsc, Brefl, Brefr));

    return res;
}

QColor Picture::cast_ray(QVector3D orig, QVector3D dir, int depth)
{
    int closest = 0;
    double t;
    auto lights = _scene.get_light();
    auto models = _scene.get_model();
    QVector3D N;

    if (depth > _maxDepth || !scene_intersect(orig, dir, t, closest, N)) {
        return (_scene.get_backgroundColor());
    }

    auto material = models[closest]->get_material();

    QVector3D point = orig + dir * t;

    QVector3D reflect_dir = reflect(dir, N).normalized();
    QColor reflect_color = cast_ray(point, reflect_dir, depth + 1);

    QVector3D refract_dir = refract(dir, N, material.get_refractive_index()).normalized();
    QColor refract_color = cast_ray(point, refract_dir, depth + 1);

    double diffuse_intensity = 0, specular_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++) {
        //        QVector3D light_dir = (lights[i]->get_position() - point).normalized();
        QVector3D light_dir = (point - lights[i]->get_position()).normalized();
        QVector3D tN;
        models[closest]->rayIntersect(lights[i]->get_position(), light_dir, t, tN);
        double t1 = 0;
        int tmp;
        if (scene_intersect(lights[i]->get_position(), light_dir, t1, tmp, tN) && t1 < t)
            continue;

        diffuse_intensity += lights[i]->get_intensity() * std::max(0.f, QVector3D::dotProduct(N, (-1) * light_dir));
        specular_intensity += powf(std::max(0.f, QVector3D::dotProduct(reflect(light_dir, N).normalized(), dir * (-1))), material.get_spec_exp()) * lights[i]->get_intensity();
    }

    return getColor(material, diffuse_intensity, specular_intensity, reflect_color, refract_color);
}

std::shared_ptr<QImage> Picture::drawingFigure()
{
    std::shared_ptr<QImage> image = std::make_shared<QImage>(_width, _height, QImage::Format_RGB32);
    image->fill(_scene.get_backgroundColor());

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            QVector3D screen(x, y, 1000);
            QVector3D dir = (screen - _cam.get_position()).normalized();
            _cam.set_direction(dir);
            QColor res = cast_ray(_cam.get_position(), _cam.get_direction(), 0);
            image->setPixelColor(x, y, res);
        }
    }

    std::cout << "done" << std::endl;
    return image;
}

std::shared_ptr<QImage> Picture::drawingFigure(int nThr)
{
    std::shared_ptr<QImage> image = std::make_shared<QImage>(_width, _height, QImage::Format_RGB32);
    image->fill(Qt::black);

    std::vector<std::thread> thrs(nThr);
    std::vector<std::vector<QColor>> buffer(_height, std::vector<QColor>(_width, QColor(0, 0, 0)));
    std::vector<int> limits;

    int one_thr = _height / nThr;
    for (int i = 0; i < nThr; i++) {
        limits.push_back(i * one_thr);
    }
    limits.push_back(_height);

    for (int i = 0; i < nThr; i++)
        thrs[i]
            = std::thread(&Picture::drawThr, this, limits[i], limits[i + 1], std::ref(buffer));

    for (int i = 0; i < nThr; i++)
        thrs[i].join();

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            image->setPixelColor(x, y, buffer[y][x]);
        }
    }

    std::cout << "done" << std::endl;
    return image;
}

void Picture::drawThr(int start, int end, std::vector<std::vector<QColor>>& img)
{
    for (int y = start; y < end; y++) {
        for (int x = 0; x < _width; x++) {
            QVector3D screen(x, y, 200);
            QVector3D dir = (screen - _cam.get_position()).normalized();
            _cam.set_direction(dir);
            QColor res = cast_ray(_cam.get_position(), _cam.get_direction(), 0);
            img[y][x] = res;
        }
    }
}
