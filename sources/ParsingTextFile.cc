#include "ParsingTextFile.hh"

std::vector<BLOCK_TEXT_FILE> ParsingTextFile::block;

ParsingTextFile::ParsingTextFile(std::string pathFile, uint32_t numberOfBlock, uint32_t orderBlock)
{
    this->pathFile = pathFile;
    this->numberOfBlock = numberOfBlock;
    this->orderBlock = orderBlock;
    file = std::ifstream(pathFile);
    if (orderBlock != 0)
    {
        file.seekg(block[orderBlock].begin);
    }
}

ParsingTextFile::~ParsingTextFile()
{
    file.close();
}


uint32_t ParsingTextFile::getLineOfFile()
{
    FILE *fp;
    char result[10];
    const std::string command = "wc -l " + pathFile + " | cut -d \" \" -f1";
    fp = popen(command.c_str(), "r");

    if (fp == nullptr)
    {
        std::cout << "Can not execute the command: " << command << std::endl;
        return 0;
    }

    fgets(result, 10, fp);
    return std::atoi(result);
}

void ParsingTextFile::buildBlock()
{
    uint32_t numLine = getLineOfFile();
    std::ifstream::streampos linebegins;
    uint32_t count = 0;
    uint32_t begin = 0;
    std::string s;

    while(file.good())
    {
        count++;
        linebegins = file.tellg();
        std::getline(file, s);

        if (count == numLine/numberOfBlock)
        {
            BLOCK_TEXT_FILE tmp = {begin, (uint32_t)linebegins};
            block.push_back(tmp);
            begin = linebegins;
            count = 0;
        }
    }
    file.seekg(block[orderBlock].begin);
}

std::string ParsingTextFile::getData(bool flag)
{
    std::string s;
    std::getline(file, s);
    if (file.tellg() > block[orderBlock].end)
    {
        return "";
    }
    return s;
}
