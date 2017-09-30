#include "jsonreader.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string schemaPath = "schema.json";
    std::string jsonPath = "test.json";
    
    JSONReader reader;

    Error err;
    rapidjson::Document sd = reader.readJSON(schemaPath, err);
    if (err)
    {
        std::cerr << "Read schema file error: " << err.reason() << std::endl;
        exit(EXIT_FAILURE);
    }

    rapidjson::SchemaDocument schema(sd); 
    rapidjson::SchemaValidator validator(schema);

    rapidjson::Document json = reader.readJSON(jsonPath, err);
    if (err)
    {
        std::cerr << "Read JSON file error: " << err.reason() << std::endl;
        exit(EXIT_FAILURE);        
    }
    
    if (!json.Accept(validator))
    {
        std::cout << "NO! Input JSON file " << jsonPath << " not match the schema " << "" << std::endl;
    }
    else
    {
        std::cout << "YES! Input JSON file " << jsonPath << " match the schema " << "" << std::endl;        
    }
    validator.Reset();
    
    return 0;
}
