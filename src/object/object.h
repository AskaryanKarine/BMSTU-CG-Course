#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void transform() = 0;
};

#endif // OBJECT_H
