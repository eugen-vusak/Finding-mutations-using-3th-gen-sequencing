/**
 * Src: https://github.com/SFML/SFML/wiki/Source:-Settings-Parser
 */

#include "config/SettingsJsonParser.hpp"

#include <locale>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
SettingsJsonParser::SettingsJsonParser() : m_isChanged(false)
{
    m_data.clear();     //added
}


SettingsJsonParser::~SettingsJsonParser()
{
    //saveToFile();
}


bool SettingsJsonParser::loadFromFile(const std::string& filename)
{
    //m_data.clear();
    m_filename = filename;
    return read();
}


bool SettingsJsonParser::read()
{
    std::ifstream in(m_filename);

    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    cJSON* body_from_file = cJSON_Parse(contents.c_str());
    cJSON* body_line;

    cJSON_ArrayForEach(body_line, body_from_file) {

        char* c_str_body_line = cJSON_Print(body_line);
        std::string str_body_line {c_str_body_line};
        free(c_str_body_line);

        if (cJSON_IsString(body_line)) { // remove quotes
            str_body_line.erase(std::remove(str_body_line.begin(), str_body_line.end(), '"'), str_body_line.end());
        }

        //printf("%s %s\n", body_line->string, str_body_line.c_str());

        std::pair<std::string, std::string> keyValuePair = std::make_pair(body_line->string, str_body_line.c_str());

        if (!keyValuePair.first.empty())
        {
            // if the line is not empty or a comment save it to the map
            m_data[keyValuePair.first] = keyValuePair.second;
        }
    }

    cJSON_Delete(body_from_file);
    cJSON_Delete(body_line);

    m_isChanged = false;
    return true;
}


bool SettingsJsonParser::write(cJSON* body, std::string filename) {
    char* body_char = body->valuestring;
    std::ofstream stream;
    stream.open(filename);
    if (!stream) {
        printf("Opening file failed!\n");
        return false;
    }
    stream << body_char << std::endl;
    stream.close();

    return true;
}


void SettingsJsonParser::print() const
{
    for (auto& element : m_data)
        std::cout << element.first << "=" << element.second << std::endl;

    std::cout << std::endl << "Size: " << m_data.size() << std::endl;
}


bool SettingsJsonParser::isChanged() const
{
    return m_isChanged;
}
