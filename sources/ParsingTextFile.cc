#include "ParsingTextFile.hh"
#include <unistd.h>

std::map<std::string, std::string> ParsingTextFile::convertTable = {
    {"a", sign_a},
    {"A", sign_A},
    {"e", sign_e},
    {"E", sign_E},
    {"o", sign_o},
    {"O", sign_O},
    {"u", sign_u},
    {"U", sign_U},
    {"i", sign_i},
    {"I", sign_I},
    {"d", sign_d},
    {"D", sign_D},
    {"y", sign_y},
    {"Y", sign_Y}
};

std::vector<std::string> ParsingTextFile::sign = {
    "á", "à", "ạ", "ả", "ã", "â", "ấ", "ầ", "ậ", "ẩ", "ẫ", "ă", "ắ", "ằ", "ặ", "ẳ", "ẵ",
    "Á", "À", "Ạ", "Ả", "Ã", "Â", "Ấ", "Ầ", "Ậ", "Ẩ", "Ẫ", "Ă", "Ắ", "Ằ", "Ặ", "Ẳ", "Ẵ",
    "é", "è", "ẹ", "ẻ", "ẽ", "ê", "ế", "ề", "ệ", "ể", "ễ",
    "É", "È", "Ẹ", "Ẻ", "Ẽ", "Ê", "Ế", "Ề", "Ệ", "Ể", "Ễ",
    "ó", "ò", "ọ", "ỏ", "õ", "ô", "ố", "ồ", "ộ", "ổ", "ỗ", "ơ", "ớ", "ờ", "ợ", "ở", "ỡ",
    "Ó", "Ò", "Ọ", "Ỏ", "Õ", "Ô", "Ố", "Ồ", "Ộ", "Ổ", "Ỗ", "Ơ", "Ớ", "Ờ", "Ợ", "Ở", "Ỡ",
    "ú", "ù", "ụ", "ủ", "ũ", "ư", "ứ", "ừ", "ự", "ử", "ữ",
    "Ú", "Ù", "Ụ", "Ủ", "Ũ", "Ư", "Ứ", "Ừ", "Ự", "Ử", "Ữ",
    "í", "ì", "ị", "ỉ", "ĩ",
    "Í", "Ì", "Ị", "Ỉ", "Ĩ",
    "đ",
    "Đ",
    "ý", "ỳ", "ỵ", "ỷ", "ỹ",
    "Ý", "Ỳ", "Ỵ", "Ỷ", "Ỹ"
};

std::map<std::string, std::vector<uint32_t>*> ParsingTextFile::mapInvertedIndex;
std::unordered_map<uint32_t, std::pair<std::string, uint32_t>> ParsingTextFile::hashTable;
std::queue <std::pair<uint32_t, std::string>*> ParsingTextFile::container;
std::mutex ParsingTextFile::mContainer;
std::mutex ParsingTextFile::mMapInvertedIndex;
bool ParsingTextFile::isRuning = false;

std::condition_variable ParsingTextFile::condNotify;
std::thread ParsingTextFile::thWorker[MAX_THEARD];

ParsingTextFile::ParsingTextFile(std::string pathFile)
{
    this->pathFile = pathFile;
    file = std::ifstream(pathFile);
}

ParsingTextFile::~ParsingTextFile()
{
    file.close();
}

void ParsingTextFile::worker(ParsingTextFile* p)
{
    while(1)
    {
        std::unique_lock<std::mutex> lck(mContainer);

        while (container.empty())
        {
            if (isRuning)
            {
                return;
            }
            condNotify.wait(lck);
        }
        std::pair<uint32_t, std::string>* pairData = container.front();
        container.pop();
        lck.unlock();

        std::pair<std::string, uint32_t> pairHashTable;
        pairHashTable.first = pairData->second;
        std::regex rgx ("\\s+");
        std::string unsignedString = p->convertUnsignedString(pairData->second);
        std::sregex_token_iterator iter(unsignedString.begin(), unsignedString.end(), rgx, -1);
        std::sregex_token_iterator end;
        std::map<std::string, std::vector<uint32_t>*>::iterator itr;
        uint32_t countWord = 0;
        while (iter != end)  
        {
            countWord++;

            std::unique_lock<std::mutex> l(mMapInvertedIndex);

            itr = mapInvertedIndex.find(*iter);
            if (itr != mapInvertedIndex.end())
            {
                itr->second->push_back(pairData->first);
            }
            else
            {
                std::vector<uint32_t>* vec = new std::vector<uint32_t>;
                vec->push_back(pairData->first);
                mapInvertedIndex.emplace(*iter, vec);
            }
            l.unlock();
            ++iter;
        }
        pairHashTable.second = countWord;
        
        lck.lock();
        hashTable[(uint32_t)pairData->first] = pairHashTable;
        lck.unlock();

        delete pairData;
    }
}

void ParsingTextFile::build()
{
    uint32_t count = 0;
    std::string rawdata;

    for (int i = 0; i < MAX_THEARD; ++i)
    {
        thWorker[i] = std::thread(worker, this);
    }

    while(file.good())
    {
        count++;
        std::getline(file, rawdata);
        std::unique_lock<std::mutex> lock(mContainer);
        std::pair<uint32_t, std::string>* p = new std::pair<uint32_t, std::string>;
        p->first = count;
        p->second = rawdata;

        container.push(p);
        condNotify.notify_all();
        lock.unlock();
    }
    isRuning = true;
    file.close();

    for (int i = 0; i < MAX_THEARD; ++i)
    {
        thWorker[i].join();
    }
}

std::string ParsingTextFile::convertUnsignedString(const std::string& input)
{
    std::string output = input;
    for (auto& itr : convertTable)
    {
        output = std::regex_replace(output, std::regex(itr.second), itr.first);
    }
    return output;
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