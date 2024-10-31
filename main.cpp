#include <iostream>
#include "reflect/object_factory.h"
#include "work/A.h"
#include "work/B.h"

using namespace duck::reflect;

int main(int argc, char* argv[])
{
    int ret;
    ObjectFactory::instance()->print_object_map();
    ObjectFactory::instance()->print_field_map();
    ObjectFactory::instance()->print_method_map();

    Object* A = ObjectFactory::instance()->create_object("A");
    Object* B = ObjectFactory::instance()->create_object("B");


    std::cout << "create object " << A->name() << " ok!" << std::endl;
    std::cout << "create object " << B->name() << " ok!" << std::endl;

    A->set<std::string>("name_", "kitty");
    A->set<int>("age_", 18);
    B->set<std::string>("name_", "tom");
    B->set<int>("age_", 22);

    std::string name;
    int age = 0;
    A->get<std::string>("name_", name);
    A->get<int>("age_", age);
    std::cout << "A: name_=" << name  << ", age_=" << age << std::endl;

    B->get<std::string>("name_", name);
    B->get<int>("age_", age);
    std::cout << "B: name_=" << name  << ", age_=" << age << std::endl;

    

    // A->call<void>("f1");
    // age = A->call<int>("f2", name);
    ret = A->call<void>("f1");
    ret = A->call<int>(age, "f2", name);
    ret = A->call<int>(age, "f3", name);
    std::cout << "age =" << age << std::endl;

    return 0;
}














