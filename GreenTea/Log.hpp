#ifndef  LOG_HPP
#define  LOG_HPP
#include <cstdarg>
#include <iostream>
#include <mutex>
#define CLOG(title,bool,...) __log__(std::cerr,bool,#title,__FILE__,__LINE__,__VA_ARGS__) 
#define DLOG(title,...) __log__(std::cerr,0,#title,__FILE__,__LINE__,__VA_ARGS__) 

#ifdef _WIN32 
#define localtime_s(tm, time) localtime_s((tm), (time))
#else 
#define localtime_s(tm, time) localtime_r((time), (tm))
#endif

template <typename StreamType>
__inline__ void __log__(StreamType& ostream, bool showFileLine, 
                   const char* title, const char* file, int line,
                   const char* format, ...) {
    static std::mutex mtx;
    std::lock_guard lg(mtx);
    time_t now = time(nullptr);
    char timeStr[20] = {};
    tm tm_;
    localtime_s (&tm_,&now);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm_);
    
    char message[1024] = {};
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    ostream << timeStr << " --" << title << "-- ";
    if (showFileLine) {
        ostream << file << "," << line << " ";
    }
    ostream << message << std::endl;
}
#endif