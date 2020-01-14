#ifndef SETTINGS_JSON_PARSER_H
#define SETTINGS_JSON_PARSER_H

#include <cJSON.h>

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cstdint>

class SettingsJsonParser
{
public:
    SettingsJsonParser();
    ~SettingsJsonParser();

    bool loadFromFile(const std::string& filename);
    bool write(cJSON* body, std::string filename);

    bool isChanged() const;

    template<typename T>
    void get_to(const std::string& key, T & value) const;
    template<typename T>
    void get_to(const std::string& key, std::vector<T> &value) const;

    template<typename T>
    T get(const std::string& key) const;
    template<typename T>
    T get(const std::string& key, const T& def) const;

    template<typename T>
    void set(const std::string &key, const T value);

    // template<typename T>
    // void set(const std::string& key, const std::vector<T> value);

    void print() const;

private:

    //return the string in the type of T
    template<typename T>
    T convertToType(const std::string &input) const;
    //return string of type T
    template<typename T>
    std::string convertToStr(const T input) const;

    bool read();
    bool write() const;
    std::pair<std::string, std::string> parseLine(const std::string &line) const;

    bool m_isChanged;
    std::string m_filename;
    std::map<std::string, std::string> m_data;
    const std::locale m_locale;
};


template<typename T>
inline std::string SettingsJsonParser::convertToStr(T) const {
    throw "Unsupported type supplied, either change types, or write a correct conversion function for the template type.";
}

template<>
inline std::string SettingsJsonParser::convertToStr<std::string>(std::string value) const {
    return value;
}

template<>
inline std::string SettingsJsonParser::convertToStr<const char*>(const char* value) const {
    return std::string(value);
}

template<>
inline std::string SettingsJsonParser::convertToStr<bool>(bool value) const {
    return (value) ? "true" : "false";
}

template<>
inline std::string SettingsJsonParser::convertToStr<char>(char value) const {
    std::string tmp = "";
    tmp = value;
    return tmp;
}

template<>
inline std::string SettingsJsonParser::convertToStr<int>(int value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string SettingsJsonParser::convertToStr<float>(float value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string SettingsJsonParser::convertToStr<short>(short value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string SettingsJsonParser::convertToStr<double>(double value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string SettingsJsonParser::convertToStr<uint8_t>(uint8_t value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
inline std::string SettingsJsonParser::convertToStr<uint32_t>(uint32_t value) const {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
inline T SettingsJsonParser::convertToType(const std::string &) const {
    throw "Unconvertable type encountered, please use a different type, or define the handle case in SettingsParser.hpp";
}

template<>
inline int SettingsJsonParser::convertToType<int>(const std::string &input) const {
    int value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

template<>
inline double SettingsJsonParser::convertToType<double>(const std::string &input) const {
    double value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

template<>
inline float SettingsJsonParser::convertToType<float>(const std::string &input) const {
    float value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

template<>
inline short SettingsJsonParser::convertToType<short>(const std::string &input) const {
    short value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

template<>
inline uint8_t SettingsJsonParser::convertToType<uint8_t>(const std::string &input) const {
    short value;
    std::stringstream ss(input);
    ss >> value;

    return static_cast<uint8_t>(value);
}

template<>
inline uint32_t SettingsJsonParser::convertToType<uint32_t>(const std::string &input) const {
    uint32_t value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

template<>
inline bool SettingsJsonParser::convertToType<bool>(const std::string &input) const {
    return input == "true" ? true : false;
}

template<>
inline char SettingsJsonParser::convertToType<char>(const std::string &input) const {
    return input[0];
}

template<>
inline std::string SettingsJsonParser::convertToType<std::string>(const std::string &input) const {
    return input;
}

template<typename T>
inline void SettingsJsonParser::get_to(const std::string& key, T &value) const {
    auto it = m_data.find(key);

    if (it != m_data.end()) {
        value = convertToType<T>(it->second);
    }
}

template<typename T>
inline T SettingsJsonParser::get(const std::string& key, const T& def) const {
    T var = def;
    get_to(key, var);
    return var;
}

template<typename T>
inline T SettingsJsonParser::get(const std::string& key) const {
    T var;
    get_to(key, var);
    return var;
}

template<>
inline std::string SettingsJsonParser::get(const std::string& key) const {
    std::string var;
    get_to(key, var);
    return var;
}

/**
 * This method tries to read the value of a key into a vector. The values have to be
 * seperated by comma. The vector is cleared before it is filled.
 */
template<typename T>
inline void SettingsJsonParser::get_to(const std::string& key, std::vector<T> &value) const {
    auto it = m_data.find(key);
    if (it == m_data.end()) {
        return;
    }

    cJSON* array_body = cJSON_Parse(it->second.c_str());

    for (int i = 0; i < cJSON_GetArraySize(array_body); i++) {
        cJSON* item = cJSON_GetArrayItem(array_body, i);

        char* object_string = cJSON_Print(item);
        value.push_back(convertToType<T>(std::string(object_string)));
        free(object_string);
    }

    cJSON_Delete(array_body);
}

template<typename T>
inline void SettingsJsonParser::set(const std::string& key, const T value) {
    // the [] operator replaces the value if the key is found, if not it creates a new element
    m_data[key] = convertToStr<T>(value);
    m_isChanged = true;
}

// template<typename T>
// inline void SettingsJsonParser::set(const std::string &key, const std::vector<T> value) {
//     // transform the vector into a string that seperates the elements with a comma
//     std::string valueAsString;
//     for (size_t i = 0; i < value.size() - 1; ++i) {
//         valueAsString += convertToStr<T>(value.at(i)) + ",";
//     }
//     valueAsString += convertToStr<T>(value.back());

//     // the [] operator replaces the value if the key is found, if not it creates a new element
//     m_data[key] = valueAsString;
//     m_isChanged = true;
// }

#endif // SETTINGS_JSON_PARSER_H
