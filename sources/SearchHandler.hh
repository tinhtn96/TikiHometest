#ifndef SEARCHHANDLER_HH
#define SEARCHHANDLER_HH

#include <iostream>
#include "ParsingTextFile.hh"

class SearchHandler
{
public:
    /**
     * Constructor
     */
    SearchHandler(ParsingTextFile* p);

    /**
     * Default destuctor
     */
    ~SearchHandler() = default;

    /**
     *  Search
     */
    std::vector<std::string> search(std::string key);

private:

    /**
     *  Intersection
     */

    std::vector<uint32_t> intersection(std::vector<uint32_t>& first, std::vector<uint32_t>& second);

    /**
     *  instance for parsing
     */
    ParsingTextFile* parsing;
};

#endif //SEARCHHANDLER_HH
