#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <functional>
#include <utility>

namespace GreenTea {

    template<typename  KeyType, typename ValueType, typename CompareType = std::less<KeyType>>
    class factory
    {
        std::map<KeyType, std::shared_ptr<ValueType>,CompareType>  obj_array;
        std::mutex mtx;
    public:
        template<typename type = ValueType,typename... ArgTypes>
        std::shared_ptr<ValueType> create(KeyType key,ArgTypes && ... args)
        {        
            auto ptr = std::make_shared<type>(std::forward(args)...);
            std::unique_lock lock(mtx);
            auto it =  obj_array.emplace(key,ptr);
            if(!it.second) 
            {throw  std::runtime_error("fail create"); }
            return it.first->second;
        }

        std::shared_ptr<ValueType> insert(KeyType key,std::shared_ptr<ValueType> ptr)
        {
            std::unique_lock lock(mtx);
            return obj_array.emplace(key,ptr).first->second;
        }

        size_t count(KeyType key)
        {
            std::unique_lock lock(mtx);
            return obj_array.count(key);
        }

        std::shared_ptr<ValueType> at(KeyType key)
        {
            std::unique_lock lock(mtx);
            return obj_array.find(key)->second;
        }

        std::shared_ptr<ValueType> remove(KeyType key) {
            std::unique_lock lock(mtx);
            auto it = obj_array.find(key); 
            if (it != obj_array.end())  {
                auto obj = it->second;
                obj_array.erase(it); 
                return obj;
            }
            return nullptr;
        }
    };

}
#endif