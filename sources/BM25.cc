#include "BM25.hh"
#include <math.h> 

BM25::BM25(ParsingTextFile* p)
{
    this->parser = p;
    N = (float)p->getLineOfFile();

    auto hashTable = parser->getHashTable();

    uint32_t sumWordInData = 0;
    for (auto& itr : hashTable)
    {
        sumWordInData += itr.second.second;
    }
    avgdl = (float)sumWordInData / N;

    std::cout << IDF("tai") << std::endl;
}

double BM25::IDF(std::string word)
{
    std::map<std::string, std::vector<uint32_t>*>::iterator itr;
    auto mapIndex = parser->getMapInvertedIndex();
    itr = mapIndex.find(word);
    uint32_t n = 0;
    if (itr != mapIndex.end())
    {
        auto vec = *(itr->second);
        n = vec.size();
    }
    double tmp = (N - n + 0.5) / (n + 0.5);
    return log(tmp);
}

double BM25::TF(std::string word, uint32_t uniqueID)
{

}
