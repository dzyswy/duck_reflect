#include "reflect/object_factory.h"




using namespace duck::reflect;


int Object::get_field_offset(const std::string& field_name, size_t& offset) 
{
    auto& object_map = ObjectFactory::instance()->object_field_map_;
    auto it_obj = object_map.find(name_);
    if (it_obj == object_map.end()) {
        std::cout << "Can't find Object: " << name_ << std::endl;
        return -1;
    }

    auto& field_map = it_obj->second;
    auto it_fld = field_map.find(field_name);
    if (it_fld == field_map.end()) {
        std::cout << "Can't find Object field: " << field_name << std::endl;
        return -1;
    }

    std::shared_ptr<ObjectFeild> field = it_fld->second;

    offset = field->offset();
    return 0;
}


int Object::get_method_ptr(const std::string& method_name, uintptr_t& method_ptr)
{
    auto& object_map = ObjectFactory::instance()->object_method_map_;
    auto it_obj = object_map.find(name_);
    if (it_obj == object_map.end()) {
        std::cout << "Can't find Object: " << name_ << std::endl;
        return -1;
    }

    auto& method_map = it_obj->second;
    auto it_method = method_map.find(method_name);
    if (it_method == method_map.end()) {
        std::cout << "Can't find Object method: " << method_name << std::endl;
        return -1;
    }

    std::shared_ptr<ObjectMethod> method = it_method->second; 
    method_ptr = method->method_ptr();

    return 0;
}






