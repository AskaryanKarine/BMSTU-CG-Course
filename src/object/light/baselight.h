#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "object.h"

class BaseLight : public Object {
public:
    BaseLight() = default;
    virtual ~BaseLight() = default;

    // позиция, интесивность (везде 1)
};

#endif // BASELIGHT_H
