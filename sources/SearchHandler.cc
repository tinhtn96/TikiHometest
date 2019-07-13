#include "SearchHandler.hh"
#include <unistd.h>

std::vector<std::string> SearchHandler::paramKey;
std::mutex SearchHandler::mutexTheard;
std::set<SearchStructure, CustomCompare> SearchHandler::output;
std::thread SearchHandler::thWorker[MAX_THEARD];

SearchHandler::SearchHandler(std::string keyWord)
{
    this->keyWord = keyWord;

    paramKey = resplit(keyWord);
}

std::vector<std::string> SearchHandler::resplit(const std::string &s, std::string rgx_str)
{
    std::vector<std::string> elems;
    std::regex rgx (rgx_str);
    std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
    std::sregex_token_iterator end;

    while (iter != end)  {
        elems.push_back(*iter);
        ++iter;
    }
    return elems;
}

void SearchHandler::threadWorker(int index)
{
    if (index != 0)
    {
        sleep(1);
    }

    ParsingTextFile parsingFile("./../data/product_names.txt", MAX_THEARD, index);
    if (index == 0)
    {
        parsingFile.buildBlock();
    }

    
    std::string data = parsingFile.getData();
    while(!data.empty())
    {
        int prioriy = -1;

        for (auto& itr : paramKey)
        {
            if (data.find(itr) != std::string::npos)
            {
                prioriy++;
            }
        }
        
        if (prioriy >= 0)
        {
            std::unique_lock<std::mutex> lock(mutexTheard);
            output.insert(SearchStructure(data, prioriy));
            lock.unlock();
        }

        data = parsingFile.getData();
    }

}

void SearchHandler::execute()
{
    for (int i = 0; i < MAX_THEARD; i++)
    {
        thWorker[i] = std::thread(threadWorker, i);
    }

    for (int i = 0; i < MAX_THEARD; i++)
    {
        thWorker[i].join();
    }
}

void SearchHandler::showResult()
{
    for (auto& itr : output)
    {
        std::cout << itr.name << std::endl;
    }
}