#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <list>
#include <map>
#include <any>
#include <memory>
#include <string>

#include <GreenTea/Marco.hpp>

namespace GreenTea {
    class Object;
    using ObjectPointer = std::shared_ptr<Object>;
    using ObjectList = std::list<ObjectPointer>;
    using PropertyMap = std::map<std::string,std::any>;

    class Object
    {
    private:
        struct 
        {
            int index = -1;
            int current_index = -1;
            ObjectPointer parent = nullptr;
            ObjectList children;
            PropertyMap property;
        }
        data;
    public:
        FIVERULE(Object)
        Object(ObjectPointer parent);

        DWRF(ObjectPointer,Parent);
        DWRF(int,Index);
        DRF(ObjectList,Children);
        DRF(PropertyMap,Propertys);

        int addChild(ObjectPointer);

        void removeChild(int index);

        ObjectPointer ChlidAt(int index);

        template<typename PropertyType>
        int addProperty(const std::string & name,PropertyType value)
        {
            data.property.emplace(name,value);
            return 0;
        }

        template<typename PropertyType>
        PropertyType getProperty(const std::string & name)
        {
            return std::any_cast<PropertyType>(data.property.at(name));
        }
    };
}

#endif