#ifndef JSONREADER_H
#define JSONREADER_H

#include "error.hpp"

#include <rapidjson/filereadstream.h>
#include <rapidjson/schema.h>
  
#include <string>
#include <array>

class JSONReader
{
public:
    JSONReader() = default;

    rapidjson::Document readJSON(const std::string &path, Error &err)
    {
        rapidjson::Document doc;
        
        FILE *fp = fopen(path.c_str(), "r");
        if (fp == nullptr)
        {
            err = Error(std::string("JSON file ") + path + " not found");
        }
        else
        {
            rapidjson::FileReadStream fs(fp, buff_.data(), BUFFER_SIZE);        
            doc.ParseStream(fs);            
            if (doc.HasParseError())
            {
                err = Error(std::string("JSON file ") + path + " is invalid");
            }
            fclose(fp);
        }

        return doc;
    }
    
private:
    static constexpr std::size_t BUFFER_SIZE = 1 * 1024 * 1024;
    std::array<char, BUFFER_SIZE> buff_;
};

#endif /* JSONREADER_H */
