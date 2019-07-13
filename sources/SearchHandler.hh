#ifndef SEARCHHANDLER_HH
#define SEARCHHANDLER_HH

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <set>
#include <algorithm>
#include <iterator>
#include <regex>
#include <mutex>

#include "ParsingTextFile.hh"

#define MAX_THEARD 10

class SearchStructure
{
public:
    std::string name;
    int priority;
    SearchStructure(std::string name, int priority)
    {
        this->name = name;
        this->priority = priority;
    }
    // ~SearchStructure() = default;
};

struct CustomCompare
{
    bool operator()(const SearchStructure& lhs, const SearchStructure& rhs)
    {
        if (lhs.priority == rhs.priority)
            return lhs.name < rhs.name;
        else
            return lhs.priority > rhs.priority;
    }
};

class SearchHandler
{
public:
    SearchHandler(std::string keyWord);
    ~SearchHandler() = default;
    void showResult();
    void execute();

private:
    std::vector<std::string> resplit(const std::string &s, std::string rgx_str = "\\s+");

    static void threadWorker(int index);

    std::string keyWord;

    static std::set<SearchStructure, CustomCompare> output;

    static std::thread thWorker[MAX_THEARD];

    static std::vector<std::string> paramKey;

    static std::mutex mutexTheard;

    static std::vector<std::string> tmp;
};

#endif //SEARCHHANDLER_HH
