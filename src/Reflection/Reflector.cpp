#include "Reflection/Reflector.h"

namespace TTB {

    Reflector& Reflector::instance() {

        static Reflector instance;
        return instance;
    }

    std::unique_ptr<IObject> Reflector::create(const char *className) {

        auto it = m_classRegistry.find(className);
        if (it != m_classRegistry.end()) {
            return it->second();
        }
        return nullptr;
    }

    bool Reflector::registerClass(const char* className, std::function<std::unique_ptr<IObject>()>&& creator) {

        m_classRegistry[className] = std::move(creator);
        return true;
    }
}
