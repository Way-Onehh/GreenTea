#include <GreenTea/UniqueDB.hpp>
#include <sstream>
#include <fstream>

namespace GreenTea {
    std::string UniqueDB::serialize() const {
        std::ostringstream oss;
        for (const auto& pair : data) {
            oss << pair.first  << "\t" << pair.second  << "\n";
        }
        return oss.str(); 
    }

    void UniqueDB::deserialize(const std::string& content) {
        std::istringstream iss(content);
        std::string line;
        while (std::getline(iss, line)) {
            size_t tab_pos = line.find('\t'); 
            if (tab_pos != std::string::npos) {
                std::string key = line.substr(0,  tab_pos);
                std::string value = line.substr(tab_pos  + 1);
                data[key] = value;
            }
        }
    }

    UniqueDB::UniqueDB(const std::string& filename) : db_file(filename) {
        load();
    }

    UniqueDB::~UniqueDB() {
        if (modified) {
            save();
        }
    }

    void UniqueDB::load() {
        std::ifstream file(db_file,std::ios::ate);
        if (file.is_open())  {
            const auto file_size = file.tellg(); 
            file.seekg(0); 
            std::string content;
            content.resize(file_size);
            file.read(&content[0],  file_size);
            deserialize(content);
            file.close(); 
        }
    }

    void UniqueDB::save() {
        std::ofstream file(db_file);
        if (file.is_open())  {
            file << serialize();
            file.close(); 
            modified = false;
        }
    }

    void UniqueDB::put(const std::string& key, const std::string& value) {
        data[key] = value;
        modified = true;
    }

    std::string UniqueDB::get(const std::string& key) const {
        auto it = data.find(key); 
        if (it != data.end())  {
            return it->second;
        }
        return "";
    }

    void UniqueDB::remove(const std::string& key) {
        if (data.erase(key)  > 0) {
            modified = true;
        }
    }

    std::vector<std::string> UniqueDB::keys() const {
        std::vector<std::string> result;
        for (const auto& pair : data) {
            result.push_back(pair.first); 
        }
        return result;
    }

    void UniqueDB::commit() {
        if (modified) {
            save();
        }
    }
}