#include "picture.h"

#include <iostream>

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

bool Picture::scene_intersect(QVector3D orig, QVector3D dir, QVector3D& hit, QVector3D& N, Material& m, int& closest)
{
    double dist = std::numeric_limits<double>::max();
    auto models = _scene.get_model();
    closest = -1;
    N = QVector3D(0, 0, 0);

    for (size_t i = 0; i < models.size(); i++) {
        double dist_i = 0;
        if (models[i]->rayIntersect(orig, dir, dist_i) && dist_i < dist) {
            dist = dist_i;
            hit = orig + dir * dist;
            m = models[i]->get_material();
            closest = i;
            N = models[i]->get_normal();
        }
    }

    if (closest == -1)
        return false;

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
    int a, dc, sc, refl, refr;
    dc = m.get_difColor().red();
    sc = m.get_specColor().red();
    refl = reflect.red();
    refr = refract.red();
    res.setRed(elemColor(difIntency, specIntency, m.get_albedo(), dc, sc, refl, refr));

    dc = m.get_difColor().green();
    sc = m.get_specColor().green();
    refl = reflect.green();
    refr = refract.green();
    res.setGreen(elemColor(difIntency, specIntency, m.get_albedo(), dc, sc, refl, refr));

    dc = m.get_difColor().blue();
    sc = m.get_specColor().blue();
    refl = reflect.blue();
    refr = refract.blue();
    res.setBlue(elemColor(difIntency, specIntency, m.get_albedo(), dc, sc, refl, refr));

    return res;
}

QColor Picture::cast_ray(QVector3D orig, QVector3D dir, int depth)
{
    QVector3D point, N;
    Material material;
    int closest = 0;

    auto lights = _scene.get_light();
    auto models = _scene.get_model();

    if (depth > _maxDepth || !scene_intersect(orig, dir, point, N, material, closest)) {
        return (_scene.get_backgroundColor());
    }

    // вместо *_orig = point
    QVector3D reflect_dir = reflect(dir, N).normalized();

    QVector3D reflect_orig = QVector3D::dotProduct(reflect_dir, N) < 0 ? point - N * 1e-3 : point + N * 1e-3;
    QColor reflect_color = cast_ray(reflect_orig, reflect_dir, depth + 1);

    QVector3D refract_dir = refract(dir, N, material.get_refractive_index()).normalized();
    QVector3D refract_orig = QVector3D::dotProduct(refract_dir, N) < 0 ? point - N * 1e-3 : point + N * 1e-3;
    QColor refract_color = cast_ray(refract_orig, refract_dir, depth + 1);

    double diffuse_intensity = 0, specular_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++) {
        QVector3D light_dir = (lights[i]->get_position() - point).normalized();

        double light_distance = (lights[i]->get_position() - point).length();

        QVector3D shadow_orig = QVector3D::dotProduct(light_dir, N) < 0 ? point - N * 1e-3 : point + N * 1e-3;
        QVector3D shadow_pt, shadow_N;
        Material tmpmaterial;
        if (scene_intersect(shadow_orig, light_dir, shadow_pt, shadow_N, tmpmaterial, closest) && (shadow_pt - shadow_orig).length() < light_distance)
            continue;

        diffuse_intensity += lights[i]->get_intensity() * std::max(0.f, QVector3D::dotProduct(light_dir, N));
        specular_intensity += powf(std::max(0.f, QVector3D::dotProduct(-reflect(-light_dir, N), dir)), material.get_spec_exp()) * lights[i]->get_intensity();
    }

    return getColor(material, diffuse_intensity, specular_intensity, reflect_color, refract_color);
}

std::shared_ptr<QImage> Picture::drawingFgure()
{
    std::shared_ptr<QImage> image = std::make_shared<QImage>(_width, _height, QImage::Format_RGB32);
    image->fill(_scene.get_backgroundColor());

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            QVector3D screen(x, y, 200);
            QVector3D dir = (screen - _cam.get_position()).normalized();
            _cam.set_direction(dir);
            QColor res = cast_ray(_cam.get_position(), _cam.get_direction(), 0);
            image->setPixelColor(x, y, res);
        }
    }

    std::cout << "done" << std::endl;
    return image;
}
