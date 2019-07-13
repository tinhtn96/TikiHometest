#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <iterator> 
#include <map> 
#include "SearchHandler.hh"

#define PATH_DATA_FILE  "./../data/product_names.txt"
#define PATH_KEY_FILE   "./../data/100_query.txt"

std::vector<std::string> getKey(std::string key)
{
    std::ifstream file = std::ifstream(key);
    std::string rawKey;
    std::vector<std::string> vecKey;
    while(file.good())
    {
        std::getline(file, rawKey);
        vecKey.push_back(rawKey);
    }

    file.close();
    return vecKey;
}

int main () {

    ParsingTextFile* parser = new ParsingTextFile(PATH_DATA_FILE);
    SearchHandler handler(parser);
    parser->build();

    std::vector<std::string>&& keys = getKey(PATH_KEY_FILE);

    for (auto& itr : keys)
    {
        std::cout << "************* Key is: " << itr << std::endl;
        handler.search(itr);
    }
    
    delete parser;
    return 0;
}