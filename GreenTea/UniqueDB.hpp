#ifndef DBLITE_HPP
#define DBLITE_HPP

#include <string>
#include <vector>
#include <map>

namespace GreenTea {
class UniqueDB {
private:
    std::string db_file;
    std::map<std::string, std::string> data;
    bool modified = false;
 
    std::string serialize() const ;
 
    void deserialize(const std::string& content) ;
    void load();
    void save();
public:
    UniqueDB(const std::string& filename) ;

    ~UniqueDB();

    void put(const std::string& key, const std::string& value) ;

    void remove(const std::string& key);

    std::string get(const std::string& key) const;

    std::vector<std::string> keys() const;

    void commit();
};

}
#endif