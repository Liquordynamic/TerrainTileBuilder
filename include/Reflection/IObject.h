//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_IOBJECT_H
#define TERRAINTILEBUILDER_IOBJECT_H

class IObject {

public:
    virtual const char* getDisplayName() { return "Interface Object"; }

    IObject() = default;

    virtual ~IObject() = default;
};

#endif //TERRAINTILEBUILDER_IOBJECT_H
