#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <memory>
#include <functional>


namespace duck {
namespace reflect {

class ObjectFeild;
class ObjectFactory;

class Object
{
public:
    void set_name(const std::string& value) {
        name_ = value;
    }

    std::string name() {
        return name_;
    }

    template<typename T>
    int set(const std::string& field_name, T value) {
        int ret = 0;
        size_t offset = 0;
        ret = get_field_offset(field_name, offset);
        if (ret < 0) {
            return -1;
        }
        
        unsigned char* wptr = (unsigned char*)(this) + offset;
        *(T*)wptr = value;
        return 0;
    }

    template<typename T>
    int get(const std::string& field_name, T& value) {
        int ret = 0;
        size_t offset = 0;
        ret = get_field_offset(field_name, offset);
        if (ret < 0) {
            return -1;
        }

        unsigned char* rptr = (unsigned char*)(this) + offset;
        value = *(T*)rptr;

        return 0;
    }

    int get_field_offset(const std::string& field_name, size_t& offset);



    // template<typename R, typename ...Args>
    // R call(const std::string& method_name, Args&&... args) {
    //     int ret = -1;
    //     uintptr_t method_ptr;
    //     ret = get_method_ptr(method_name, method_ptr);
    //     if (ret < 0) {
    //         return R();
    //     }

    //     typedef std::function<R(decltype(this), Args...)> func_type;
    //     return (*((func_type *)method_ptr))(this, args...);
        
    // }

    template<typename R, typename ...Args>
    int call(const std::string& method_name, Args&&... args) {
        int ret = -1;
        uintptr_t method_ptr;
        ret = get_method_ptr(method_name, method_ptr);
        if (ret < 0) {
            return -1;
        }

        typedef std::function<void(decltype(this), Args...)> func_type;
        (*((func_type *)method_ptr))(this, args...);
        
        return 0;
    }

    template<typename R, typename ...Args>
    int call(R& result, const std::string& method_name, Args&&... args) {
        int ret = -1;
        uintptr_t method_ptr;
        ret = get_method_ptr(method_name, method_ptr);
        if (ret < 0) {
            return -1;
        }

        typedef std::function<R(decltype(this), Args...)> func_type;
        result = (*((func_type *)method_ptr))(this, args...);
        
        return 0;
    }
    
    int get_method_ptr(const std::string& method_name, uintptr_t& method_ptr);

private:
    std::string name_;
};


class ObjectFeild
{
public:
    ObjectFeild(const std::string& type_name, const std::string& field_name, size_t offset) 
        : type_name_(type_name), field_name_(field_name), offset_(offset) {

    }

    std::string type_name() {
        return type_name_;
    }

    size_t offset() {
        return offset_;
    }

private:
    std::string type_name_;
    std::string field_name_;
    size_t offset_;

};

class ObjectMethod
{
public:
    ObjectMethod(const std::string& method_name, uintptr_t method_ptr) : method_name_(method_name), method_ptr_(method_ptr) {

    } 

    uintptr_t method_ptr() {
        return method_ptr_;
    }

private:
    std::string method_name_;
    uintptr_t method_ptr_;
};

typedef Object* (*create_object_func_ptr)();




class ObjectFactory
{
public:
    static ObjectFactory* instance() {
        static ObjectFactory instance;
        return &instance;
    }

    void print_object_map() {
        std::cout << std::left << std::setw(40) << std::setfill('-')<< "register object list begin"  << std::setfill(' ') << std::endl;  
        std::cout << std::left << std::setw(20) << "object_name" << std::setw(20) <<"func_ptr" << std::endl; 
        for (const auto &pair : object_map_) { 
            std::cout << std::left << std::setw(20) <<  pair.first << std::setw(20) << (void*)pair.second << std::endl;
        } 
        std::cout << std::left << std::setw(40) << std::setfill('-')<< "register object list end"  << std::setfill(' ') << std::endl;  
        std::cout << std::endl;
    }

    void print_field_map() {
        std::cout << std::left << std::setw(40) << std::setfill('-')<< "register field list begin"  << std::setfill(' ') << std::endl;  
        std::cout << std::left << std::setw(20) << "field_name" << std::setw(20) <<"type_name" << std::endl;

        auto object_map = object_field_map_;
        for (const auto &object_pair : object_map) { 
            std::string object_name = object_pair.first;
            auto field_map = object_pair.second;
            for (const auto &field_pair : field_map) {
                std::string field_name = field_pair.first;
                auto field = field_pair.second;
                std::string object_field_name = object_name + "." + field_name;
                std::cout << std::left << std::setw(20) <<  object_field_name << std::setw(20) << field->type_name() << std::endl;
            }

            
        } 
        std::cout << std::left << std::setw(40) << std::setfill('-')<< "register field list end"  << std::setfill(' ') << std::endl;  
        std::cout << std::endl;
    }

    void print_method_map() {
        std::cout << std::left << std::setw(40) << std::setfill('-')<< "register method list begin"  << std::setfill(' ') << std::endl;  
        std::cout << std::left << std::setw(20) << "method_name" << std::setw(20) <<"method_ptr" << std::endl;

        auto object_map = object_method_map_;
        for (const auto &object_pair : object_map) { 
            std::string object_name = object_pair.first;
            auto method_map = object_pair.second;
            for (const auto &method_pair : method_map) {
                std::string method_name = method_pair.first;
                auto method = method_pair.second;
                std::string object_method_name = object_name + "." + method_name;
                std::cout << std::left << std::setw(20) <<  object_method_name << std::setw(20) << method->method_ptr() << std::endl;
            }

            
        } 
        std::cout << std::left << std::setw(40) << std::setfill('-')<< "register method list end"  << std::setfill(' ') << std::endl;  
        std::cout << std::endl;
    }

    Object* create_object(const std::string& name) {
        auto it = object_map_.find(name);
        if (it == object_map_.end()) {
            std::cout << "Can't find Object: " << name << std::endl;
            return nullptr;
        } 
        return it->second();
    }


    void register_object(const std::string& name, create_object_func_ptr func) {
        object_map_[name] = func;
    }

    void register_object_field(const std::string& object_name, const std::string& type_name, const std::string& field_name, size_t offset) {
        
        std::shared_ptr<ObjectFeild> field = std::make_shared<ObjectFeild>(type_name, field_name, offset);
        object_field_map_[object_name][field_name] = field;
    }

    void register_object_method(const std::string& object_name, const std::string& method_name, uintptr_t method_ptr) {

        std::shared_ptr<ObjectMethod> method = std::make_shared<ObjectMethod>(method_name, method_ptr);
        object_method_map_[object_name][method_name] = method;
    }


public:
    std::map<std::string, create_object_func_ptr> object_map_;
    std::map<std::string, std::map<std::string, std::shared_ptr<ObjectFeild> > > object_field_map_;
    std::map<std::string, std::map<std::string, std::shared_ptr<ObjectMethod> > > object_method_map_;

private:
    ObjectFactory() {}
    ~ObjectFactory() {}
    ObjectFactory(const ObjectFactory &) = delete;
    ObjectFactory & operator = (const ObjectFactory &) = delete;

};


class RegisterObjectHelper
{
public:
    RegisterObjectHelper(const std::string& name, create_object_func_ptr func) {
        ObjectFactory* factory = ObjectFactory::instance();
        factory->register_object(name, func);
    }
};

class RegisterObjectFieldHelper
{
public:
    RegisterObjectFieldHelper(const std::string& object_name, const std::string& type_name, const std::string& field_name, size_t offset) {
        ObjectFactory* factory = ObjectFactory::instance();
        factory->register_object_field(object_name, type_name, field_name, offset);
    }
};

class RegisterObjectMethodHelper
{
public:
    RegisterObjectMethodHelper(const std::string& object_name, const std::string& method_name, uintptr_t method_ptr) {
        ObjectFactory* factory = ObjectFactory::instance();
        factory->register_object_method(object_name, method_name, method_ptr);
    }
};

#define REGISTER_OBJECT(name)               \
    Object* create_object##name() {         \
        name* obj = new name();             \
        obj->set_name(#name);               \
        return obj;                         \
    }                                       \
    RegisterObjectHelper helper_obj_##name(#name, create_object##name)

#define REGISTER_OBJECT_FIELD(object, type, field)    \
    object reg_##object##field;                        \
    RegisterObjectFieldHelper helper_##object##field(#object, #type, #field, (size_t)(&(reg_##object##field.field)) - (size_t)(&reg_##object##field))
    

#define REGISTER_OBJECT_METHOD(object, method, ret, ...)                                    \
    std::function<ret(object*, ##__VA_ARGS__)> func_##object##method = &object::method;     \
    RegisterObjectMethodHelper helper_##object##method(#object, #method, (uintptr_t)&(func_##object##method))


}//namespace reflect
}//namespace duck











