#include "SearchHandler.hh"

SearchHandler::SearchHandler(ParsingTextFile* p)
{
    parsing = p;
}

std::vector<std::string> SearchHandler::search(std::string key)
{
    std::string&& unsignedKey = parsing->convertUnsignedString(key);
    std::regex rgx ("\\s+");
    std::sregex_token_iterator iter(unsignedKey.begin(), unsignedKey.end(), rgx, -1);
    std::sregex_token_iterator end;
    std::map<std::string, std::vector<uint32_t>*>::iterator itr;
    
    std::vector<uint32_t> second;
    std::vector<uint32_t> matched;

    auto mapIndex = parsing->getMapInvertedIndex();
    auto hashTable = parsing->getHashTable();

    for (; iter != end; iter++)
    {
        itr = mapIndex.find(*iter);
        if (itr != mapIndex.end())
        {
            second = *(itr->second);
        }

        matched = intersection(matched, second);
    }

    std::vector<std::string> result;
    std::unordered_map<uint32_t, std::pair<std::string, uint32_t>>::iterator got;

    for (auto& itr : matched)
    {
        got = hashTable.find(itr);
        if (got != hashTable.end())
        {
            result.push_back(got->second.first);
            std::cout << got->second.first << std::endl;
        }
    }

    return result;
}

std::vector<uint32_t> SearchHandler::intersection(std::vector<uint32_t>& first, std::vector<uint32_t>& second)
{
    if (first.empty())
    {
        return second;
    }

    std::vector<uint32_t> match(first.size());
    std::vector<uint32_t>::iterator it;

    std::sort(first.begin(), first.end());
    std::sort(second.begin(), second.end());

    it = std::set_intersection(first.begin(), first.end(),
                                second.begin(), second.end(), match.begin()); 

    match.resize(it - match.begin());
    return match;
}
