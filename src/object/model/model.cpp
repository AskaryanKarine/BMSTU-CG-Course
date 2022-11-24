#include "model.h"

Material Model::get_material()
{
    return _mat;
}

void Model::set_material(Material m)
{
    _mat = m;
}
