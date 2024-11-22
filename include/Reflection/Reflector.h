//
// Created by Yucheng Soku on 2024/10/21.
//

#ifndef TERRAINTILEBUILDER_REFLECTOR_H
#define TERRAINTILEBUILDER_REFLECTOR_H

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

#include "Reflection/IObject.h"

namespace TTB {

    class Reflector {

    private:
        std::unordered_map<const char*, std::function<IObject*()>> m_classRegistry;

    public:

        static Reflector& instance();
        Reflector(const Reflector&) = delete;
        Reflector& operator= (const Reflector&) = delete;

        IObject* create(const std::string& className) const;
        bool registerClass(const char* className, std::function<IObject*()>&& creator);

    private:

        Reflector() = default;
        ~Reflector() = default;
    };

}

#define REGISTER_CLASS(ClassName) \
        static bool _##ClassName##_registered = \
            TTB::Reflector::instance().registerClass(#ClassName, []() { return new ClassName(); });

#endif //TERRAINTILEBUILDER_REFLECTOR_H
