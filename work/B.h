#pragma once


#include "reflect/object_factory.h"


using namespace duck::reflect;

namespace duck {
namespace work {

class B : public Object
{
public:
    void f1() {
        std::cout << "f1" << std::endl;
    }

    int f2(const std::string& name) {
        std::cout << name << std::endl;
        return 0;
    }

public:
    std::string name_;
    int age_;
    
};

REGISTER_OBJECT(B);
REGISTER_OBJECT_FIELD(B, string, name_);
REGISTER_OBJECT_FIELD(B, int, age_);
REGISTER_OBJECT_METHOD(B, f1, void);
REGISTER_OBJECT_METHOD(B, f2, int, const std::string&);

}//namespace reflect
}//namespace work






