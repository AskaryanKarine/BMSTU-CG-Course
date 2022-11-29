#ifndef MATERIAL_H
#define MATERIAL_H

#include <QColor>
#include <QVector4D>

class Material {
public:
    Material();
    Material(double r, double spec, QVector4D alb, QColor difcol);

    double get_refractive_index();
    void set_refractive_index(double r);

    double get_spec_exp();
    void set_spec_exp(double spec);

    QVector4D get_albedo();
    void set_albedo(QVector4D alb);

    QColor get_difColor();
    void set_difColor(QColor difCol);

    QColor get_specColor();
    void set_specColor(QColor specCol);

private:
    double _refractive_index, _specular_exponent;
    QVector4D _albedo;
    QColor _difColor;
    QColor _specColor;
};

#endif // MATERIAL_H
