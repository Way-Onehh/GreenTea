#ifndef MARCO_H
#define MARCO_H

#define FIVERULE(class)    class() = default;\
    class(const class &) = default;\
    class(class&&) = default;\
    virtual ~class() = default;\
    class& operator = (const class &) = default;\
    class& operator = (class&&) = default;\

#define THREERULE(class)    class() = default;\
    class(const class &) = default;\
    virtual ~class() = default;\
    class& operator = (const class &) = default;

#define DWF(type,name)   int set##name(type);

#define IWF(type,name,class,value)\
int class::set##name(type v){\
    value = v;\
    return 0 ;\
}

#define DRF(type,name)   \
type get##name();

#define IRF(type,name,class,value)   \
type class::get##name()\
{\
    return value;\
}

#define DWRF(type,name)  \
DWF(type,name) DRF(type,name)

#define IWRF(type,name,class,value)\
IWF(type,name,class,value) IRF(type,name,class,value)
#endif