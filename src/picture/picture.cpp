#include "picture.h"

#include <iostream>
#include <thread>

#include "lightsource.h"
#include "objloader.h"
#include "polyhedron.h"
#include "sphere.h"

#include <QtMath>

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
    _backgroungColor = Qt::black;
    _cam.set_position(QVector3D(0, 0, 3000));
    _screen = QVector3D(0, 0, 200);

    // LEANSE
    Material m(1, 100, QVector4D(0.2, 0.9, 0.7, 0.5), QColor(188, 21, 79));
    auto mainSph = std::make_shared<Sphere>(QVector3D(480, 350, 100), 150, m);
    _obj.push_back(mainSph);

    // PLANE
    Material m1(1, 100, QVector4D(0.6, 1, 0, 0), QColor(79, 0, 112));
    std::vector<QVector3D> planePoints = {
        QVector3D(-2000, 500, -1000),
        QVector3D(2000, 500, -1000),
        QVector3D(-2000, 500, 500),
        QVector3D(2000, 500, 500),
    };
    std::vector<Polygon> planePolygons = {
        Polygon(std::vector<size_t>({ 0, 1, 2 }), planePoints),
        Polygon(std::vector<size_t>({ 3, 2, 1 }), planePoints),
    };
    auto plane = std::make_shared<Polyhedron>(planePoints, planePolygons, m1);
    _obj.push_back(plane);

    // PREPARE PRIMITIVES
    load_primitives("../src/obj/1.obj");

    // BASE LIGHTS
    auto l = std::make_shared<LightSource>(QVector3D(0, 0, 0));
    //    _lights.push_back(l);
    auto l1 = std::make_shared<LightSource>(QVector3D(700, 400, 200));
    _lights.push_back(l1);
    auto l2 = std::make_shared<LightSource>(QVector3D(500, 500, 500));
    _lights.push_back(l2);
    auto l3 = std::make_shared<LightSource>(QVector3D(1000, 500, 500));
    _lights.push_back(l3);
}

void Picture::set_height(int height)
{
    _height = height;
}

void Picture::set_width(int width)
{
    _width = width;
}

void Picture::set_maxDepth(int max_depth)
{
    _maxDepth = max_depth;
}

void Picture::set_backColor(QColor c)
{
    _backgroungColor = c;
}

QColor Picture::get_backColor()
{
    return _backgroungColor;
}

void Picture::set_mainSphereColor(QColor c)
{
    auto m = _obj[0]->get_material();
    m.set_difColor(c);
    _obj[0]->set_material(m);
}

QColor Picture::get_mainSphereColor()
{
    return _obj[0]->get_material().get_difColor();
}

void Picture::set_refIndex(double r)
{
    auto m = _obj[0]->get_material();
    m.set_refractive_index(r);
    _obj[0]->set_material(m);
}

void Picture::set_transparient(double tr)
{
    auto m = _obj[0]->get_material();
    QVector4D d = m.get_albedo();
    d.setW(tr);
    m.set_albedo(d);
    _obj[0]->set_material(m);
}

void Picture::move_camera(QVector3D offset)
{
    _cam.transform(offset, offset, offset);
    _screen -= offset;
}

QVector3D Picture::get_cam_pos()
{
    return _cam.get_position();
}

std::tuple<bool, double, int, QVector3D> Picture::scene_intersect(const QVector3D& orig, const QVector3D& dir)
{
    double dist = std::numeric_limits<double>::max();
    QVector3D norm(0, 0, 0);
    int closest = -1;
    double t = 0;
    std::tuple<bool, double, int, QVector3D> r(false, t, closest, norm);
    for (size_t i = 0; i < _obj.size(); i++) {
        auto intersectRes = _obj[i]->rayIntersect(orig, dir);
        double dist_i = std::get<1>(intersectRes);
        if (std::get<0>(intersectRes) && dist_i < dist) {
            dist = dist_i;
            closest = i;
            norm = std::get<2>(intersectRes);
        }
    }
    if (closest == -1)
        return r;

    t = dist;
    return std::tuple<bool, double, int, QVector3D>(true, t, closest, norm);
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
    return k < 0 ? QVector3D(1, 0, 0) : I * eta + n * (eta * cosi - sqrt(k));
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

QColor Picture::cast_ray(const QVector3D& orig, const QVector3D& dir, int depth)
{
    auto sceneRes = scene_intersect(orig, dir);

    if (depth > _maxDepth || !std::get<0>(sceneRes)) {
        return (_backgroungColor);
    }
    int closest = std::get<2>(sceneRes);
    double t = std::get<1>(sceneRes);
    QVector3D N = std::get<3>(sceneRes);
    auto material = _obj[closest]->get_material();

    QVector3D point = orig + dir * t;

    QVector3D reflect_dir = reflect(dir, N).normalized();
    QColor reflect_color = cast_ray(point, reflect_dir, depth + 1);

    QVector3D refract_dir = refract(dir, N, material.get_refractive_index()).normalized();
    QColor refract_color = cast_ray(point, refract_dir, depth + 1);

    double diffuse_intensity = 0, specular_intensity = 0;
    for (size_t i = 0; i < _lights.size(); i++) {
        QVector3D light_dir = (point - _lights[i]->get_position()).normalized();
        auto intersectRes = _obj[closest]->rayIntersect(_lights[i]->get_position(), light_dir);
        auto lightRes = scene_intersect(_lights[i]->get_position(), light_dir);
        t = std::get<1>(intersectRes);
        double t1 = std::get<1>(lightRes);

        if (std::get<0>(lightRes) && t1 < t)
            continue;

        diffuse_intensity += _lights[i]->get_intensity() * std::max(0.f, QVector3D::dotProduct(N, (-1) * light_dir));
        specular_intensity += powf(std::max(0.f, QVector3D::dotProduct(reflect(light_dir, N).normalized(), dir * (-1))), material.get_spec_exp()) * _lights[i]->get_intensity();
    }

    return getColor(material, diffuse_intensity, specular_intensity, reflect_color, refract_color);
}

void Picture::load_primitives(std::string filename)
{
    Material m(1, 50, QVector4D(0.6, 1, 0, 0), QColor(255, 69, 0));
    auto Sph = std::make_shared<Sphere>(QVector3D(480, 350, -500), 150, m);
    _primitives.push_back(Sph);

    objl::Loader loader;

    bool loaded = loader.LoadFile(filename);

    if (loaded) {
        size_t minVertPos = 0;
        size_t minPolPos = 0;

        for (size_t i = 0; i < loader.LoadedMeshes.size(); i++) {
            objl::Mesh curMesh = loader.LoadedMeshes[i];

            Polyhedron p = Polyhedron();
            std::vector<QVector3D> points;
            std::vector<Polygon> polygons;

            for (size_t j = minVertPos; j < curMesh.Positions.size(); j++) {
                QVector3D point = QVector3D(curMesh.Positions[j].X, curMesh.Positions[j].Y, curMesh.Positions[j].Z);
                points.push_back(point);
            }

            minVertPos = curMesh.Positions.size();

            for (size_t j = minPolPos; j < curMesh.Indexes.size(); j += 3) {
                Polygon pol = Polygon({ curMesh.Indexes[j] - 1, curMesh.Indexes[j + 1] - 1, curMesh.Indexes[j + 2] - 1 }, points);
                polygons.push_back(pol);
            }

            minPolPos = curMesh.Indexes.size();

            p.set_points(points);
            p.set_polygons(polygons);
            p.set_material(m);

            _primitives.push_back(std::make_shared<Polyhedron>(p));
        }
    }
}

std::shared_ptr<QImage> Picture::drawingFigure()
{
    std::shared_ptr<QImage> image = std::make_shared<QImage>(_width, _height, QImage::Format_RGB32);
    image->fill(_backgroungColor);

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            QVector3D screen(x, y, 0);
            screen += _screen;
            QVector3D dir = (screen - _cam.get_position()).normalized();
            QColor res = cast_ray(_cam.get_position(), dir, 0);
            image->setPixelColor(x, y, res);
        }
    }
    std::cout << "done" << std::endl;
    return image;
}

std::shared_ptr<QImage> Picture::drawingFigure(int nThr)
{
    std::shared_ptr<QImage> image = std::make_shared<QImage>(_width, _height, QImage::Format_RGB32);
    image->fill(_backgroungColor);

    std::vector<std::thread> thrs(nThr);

    int one_thr = _height / nThr;
    for (int i = 0; i < nThr; i++) {
        int start_y = i * one_thr;
        int end_y = (i + 1) * one_thr;
        if (i == nThr - 1 && _height % nThr != 0)
            end_y += _height % nThr;
        thrs[i] = std::thread(&Picture::drawThr, this, start_y, end_y, std::ref(image));
    }

    for (int i = 0; i < nThr; i++)
        thrs[i].join();

    std::cout << "done" << std::endl;
    return image;
}

void Picture::drawThr(int start, int end, std::shared_ptr<QImage>& img)
{
    for (int y = start; y < end; y++) {
        for (int x = 0; x < _width; x++) {
            QVector3D screen(x, y, 0);
            screen += _screen;
            QVector3D dir = (screen - _cam.get_position()).normalized();
            QColor res = cast_ray(_cam.get_position(), dir, 0);
            img->setPixelColor(x, y, res);
        }
    }
}

void Picture::added_model(int type_index, QColor color)
{
    auto tM = _primitives[type_index]->clone();
    auto m = tM->get_material();
    m.set_difColor(color);
    tM->set_material(m);
    _obj.push_back(tM);
}

void Picture::remove_model(int id)
{
    _obj.erase(_obj.begin() + id);
}

void Picture::transform_model(int id, QVector3D move, QVector3D scale, QVector3D rotate)
{
    _obj[id]->transform(move, scale, rotate);
}

QVector3D Picture::get_pos_model(int id)
{
    if (get_count_models() > 2)
        return _obj[id]->get_center();
    return QVector3D(qSqrt(-1), qSqrt(-1), qSqrt(-1));
}

void Picture::added_light()
{
    auto l = std::make_shared<LightSource>(QVector3D(0, 0, 0));
    _lights.push_back(l);
}

void Picture::remove_light(int id)
{
    _lights.erase(_lights.begin() + id);
}

void Picture::trasform_light(int id, QVector3D move)
{
    _lights[id]->transform(move, move, move);
}

QVector3D Picture::get_pos_light(int id)
{
    if (get_count_light() > 0 && id > 0)
        return _lights[id]->get_position();
    return QVector3D(qSqrt(-1), qSqrt(-1), qSqrt(-1));
}

int Picture::get_count_light()
{
    return _lights.size();
}

int Picture::get_count_models()
{
    return _obj.size();
}

void Picture::change_fig_color(int id, QColor fc)
{
    auto m = _obj[id]->get_material();
    m.set_difColor(fc);
    _obj[id]->set_material(m);
}

void Picture::load_all()
{
    for (size_t i = 0; i < _obj.size() - 2; i++)
        remove_model(i + 2);

    for (int i = 0; i < 5; i++)
        added_model(i, QColor(255, 69, 0));

    transform_model(2, QVector3D(-400, 50, 0), QVector3D(0.5, 1, 1), QVector3D(0, 0, 0));
    transform_model(3, QVector3D(-200, 00, -400), QVector3D(1, 1, 1), QVector3D(0, 0, 0));
    transform_model(4, QVector3D(100, 0, -400), QVector3D(1, 1, 1), QVector3D(0, 50, 0));
    transform_model(5, QVector3D(400, 50, -300), QVector3D(1, 1, 1), QVector3D(0, 15, 0));
    transform_model(6, QVector3D(600, 0, -300), QVector3D(1, 1, 1), QVector3D(0, 70, 0));

    _lights.clear();
    auto l = std::make_shared<LightSource>(QVector3D(0, 0, 0));
    _lights.push_back(l);
    auto l1 = std::make_shared<LightSource>(QVector3D(700, 400, 200));
    _lights.push_back(l1);
    auto l2 = std::make_shared<LightSource>(QVector3D(500, 500, 500));
    _lights.push_back(l2);
    auto l3 = std::make_shared<LightSource>(QVector3D(1000, 500, 500));
    _lights.push_back(l3);
    auto l4 = std::make_shared<LightSource>(QVector3D(1000, 0, 0));
    _lights.push_back(l4);
}
