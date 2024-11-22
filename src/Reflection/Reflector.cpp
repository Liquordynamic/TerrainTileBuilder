#include "Reflection/Reflector.h"
#include <iostream>

namespace TTB {

    Reflector& Reflector::instance() {

        static Reflector instance;
        return instance;
    }

    IObject* Reflector::create(const std::string& className) const {

        for (const auto&[fst, snd] : m_classRegistry) {
            if (fst == className)
                return snd();
        }
        return nullptr;
    }

    bool Reflector::registerClass(const char* className, std::function<IObject*()>&& creator) {

        m_classRegistry[className] = std::move(creator);
        return true;
    }
}
