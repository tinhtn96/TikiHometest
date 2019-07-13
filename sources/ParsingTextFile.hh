#ifndef PARSINGTEXTFILE_HH
#define PARSINGTEXTFILE_HH

#include <iostream>
#include <fstream>
#include <vector>

typedef struct
{
    uint32_t begin;
    uint32_t end;
}BLOCK_TEXT_FILE;

class ParsingTextFile
{
public:
    /**
     * Constructor
     */
    ParsingTextFile(std::string pathFile, uint32_t numberOfBlock, uint32_t orderBlock);

    /**
     * Disable copy constructor
     */
    // ParsingTextFile(const ParsingTextFile&) = delete;

    /**
     * Default destuctor
     */
    ~ParsingTextFile();

    /**
     * Get line of file 
     */
    uint32_t getLineOfFile();

    /**
     * Build block
     */
    void buildBlock();

    /**
     * Build block
     */
    std::string getData(bool flag = false);

private:
    std::string pathFile;
    uint32_t numberOfBlock;
    uint32_t orderBlock;
    static std::vector<BLOCK_TEXT_FILE> block;
    std::ifstream file;
};
#endif //PARSINGTEXTFILE_HH
