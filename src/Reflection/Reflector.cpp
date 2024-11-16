#include "Reflection/Reflector.h"
#include <iostream>

namespace TTB {

    Reflector& Reflector::instance() {

        static Reflector instance;
        return instance;
    }

    IObject* Reflector::create(const std::string& className) {

        for (const auto& it : m_classRegistry) {
            if (it.first == className)
                return it.second();
        }
        return nullptr;
    }

    bool Reflector::registerClass(const char* className, std::function<IObject*()>&& creator) {

        m_classRegistry[className] = std::move(creator);
        return true;
    }
}
