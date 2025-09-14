#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <algorithm>
#include <functional>
#include <mutex>
#include <vector>

namespace GreenTea {

template <typename... TypeArgs>
class signal {
public:
    template<typename  T>
    int connect(T &&slot)
    {
        std::unique_lock ul(mtx);
        current_index++;
        slot_infos.push_back(std::forward<solt_info>({current_index,slot}));
        return current_index;
    }
    
    void disconnect(int index)
    {
        std::unique_lock ul(mtx);
        auto pred = [index](const solt_info&  solt_info)
        {
            return solt_info.index == index;
        };
        auto it = std::find_if(slot_infos.begin(),slot_infos.end(),pred);
        if(it != slot_infos.end())
        slot_infos.erase(it);
    }

    void emit(TypeArgs... args) {
        decltype(slot_infos) tmp;
        {
            std::unique_lock ul(mtx);
            tmp = slot_infos;
        }
        for (auto& slot_info : tmp) slot_info.slot(args...);
    }
    
    operator bool ()
    {
        std::unique_lock ul(mtx);
        return !slot_infos.empty();
    }
private:
    std::mutex mtx;

    struct solt_info 
    {
        int index = 0;
        std::function<void(TypeArgs...)> slot;
    };

    std::vector<solt_info> slot_infos;
    int current_index = -1;
};


template <>
class signal<void> {
public:
    template<typename  T>
    int connect(T &&slot)
    {
        std::unique_lock ul(mtx);
        current_index++;
        slot_infos.push_back(std::forward<solt_info>({current_index,slot}));
        return current_index;
    }
    
    void disconnect(int index)
    {
        std::unique_lock ul(mtx);
        auto pred = [index](const solt_info&  solt_info)
        {
            return solt_info.index == index;
        };
        auto it = std::find_if(slot_infos.begin(),slot_infos.end(),pred);
        if(it != slot_infos.end())
        slot_infos.erase(it);
    }

    void emit() {
        std::unique_lock ul(mtx);
        for (auto& slot_info : slot_infos) slot_info.slot(); 
    }
    
    operator bool ()
    {
        std::unique_lock ul(mtx);
        return !slot_infos.empty();
    }
private:
    std::mutex mtx;
    
    struct solt_info 
    {
        int index = 0;
        std::function<void()> slot;
    };

    std::vector<solt_info> slot_infos;
    int current_index = -1;
};

}
#endif