#include "material.h"

Material::Material()
{
}

Material::Material(double r, double spec, QVector4D alb, QColor difcol)
    : _refractive_index(r)
    , _specular_exponent(spec)
    , _albedo(alb)
    , _difColor(difcol)
    , _specColor(1, 1, 1)
{
}

double Material::get_refractive_index()
{
    return _refractive_index;
}

void Material::set_refractive_index(double r)
{
    _refractive_index = r;
}

double Material::get_spec_exp()
{
    return _specular_exponent;
}

void Material::set_spec_exp(double spec)
{
    _specular_exponent = spec;
}

QVector4D Material::get_albedo()
{
    return _albedo;
}

void Material::set_albedo(QVector4D alb)
{
    _albedo = alb;
}

QColor Material::get_difColor()
{
    return _difColor;
}

void Material::set_difColor(QColor difCol)
{
    _difColor = difCol;
}

QColor Material::get_specColor()
{
    return _specColor;
}

void Material::set_specColor(QColor specCol)
{
    _specColor = specCol;
}
