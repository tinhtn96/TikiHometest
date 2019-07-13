#ifndef BM25_HH
#define BM25_HH

#include "ParsingTextFile.hh"

constexpr float k_1 = 1.2;

class BM25
{
public:
    BM25(ParsingTextFile* p);
    double IDF(std::string word);
    double TF(std::string word, uint32_t uniqueID);
    ~BM25() = default;

private:
    ParsingTextFile* parser;

    float lengthD;
    float avgdl;
    float numWordOfD;
    float N; // number of name (line) 50.000

};

#endif //BM25_HH
