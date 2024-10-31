#pragma once


#include "reflect/object_factory.h"


using namespace duck::reflect;

namespace duck {
namespace work {

class A : public Object
{
public:
    void f1() {
        std::cout << "f1" << std::endl;
    }

    int f2(const std::string& name) {
        std::cout << "f2: name = " << name << std::endl; 
        return 0;
    }

public:
    std::string name_;
    int age_;
    
};

REGISTER_OBJECT(A);
REGISTER_OBJECT_FIELD(A, string, name_);
REGISTER_OBJECT_FIELD(A, int, age_);
REGISTER_OBJECT_METHOD(A, f1, void);
REGISTER_OBJECT_METHOD(A, f2, int, const std::string&);

}//namespace reflect
}//namespace work





