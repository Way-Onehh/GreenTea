#include <GreenTea/Object.hpp>
#include <algorithm>
#include <stdexcept>

namespace GreenTea {
    Object::Object(ObjectPointer parent)
    {
        data.parent = parent;
    }

    IWRF(ObjectPointer,Parent,Object,data.parent)
    IWRF(int,Index,Object,data.index)
    IRF(ObjectList,Children,Object,data.children)
    IRF(PropertyMap,Propertys,Object,data.property)

    int Object::addChild(ObjectPointer v)
    {
        data.current_index++;
        v->setIndex(data.current_index);
        data.children.push_back(v);
        return data.current_index;
    }

    void Object::removeChild(int index)
    {
        auto it = std::find_if(data.children.begin(),data.children.end(),[index](ObjectPointer obj)
        {
            return obj->getIndex() == index;
        });
        if(it == data.children.end()) throw std::runtime_error("Overflow");
        data.children.erase(it);
    }

    ObjectPointer Object::ChlidAt(int index)
    {
        auto it = std::find_if(data.children.begin(),data.children.end(),[index](ObjectPointer obj)
        {
            return obj->getIndex() == index;
        });
        if(it == data.children.end()) return nullptr;
        return *it;
    }
}