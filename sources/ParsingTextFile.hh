#ifndef PARSINGTEXTFILE_HH
#define PARSINGTEXTFILE_HH

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator> 
#include <map> 
#include <regex>
#include <queue>
#include <mutex>
#include <unordered_map>
#include <condition_variable>
#include <thread>
#include <set>

constexpr char sign_a[] = "á|à|ạ|ả|ã|â|ấ|ầ|ậ|ẩ|ẫ|ă|ắ|ằ|ặ|ẳ|ẵ";
constexpr char sign_A[] = "Á|À|Ạ|Ả|Ã|Â|Ấ|Ầ|Ậ|Ẩ|Ẫ|Ă|Ắ|Ằ|Ặ|Ẳ|Ẵ";
constexpr char sign_e[] = "é|è|ẹ|ẻ|ẽ|ê|ế|ề|ệ|ể|ễ";
constexpr char sign_E[] = "É|È|Ẹ|Ẻ|Ẽ|Ê|Ế|Ề|Ệ|Ể|Ễ";
constexpr char sign_o[] = "ó|ò|ọ|ỏ|õ|ô|ố|ồ|ộ|ổ|ỗ|ơ|ớ|ờ|ợ|ở|ỡ";
constexpr char sign_O[] = "Ó|Ò|Ọ|Ỏ|Õ|Ô|Ố|Ồ|Ộ|Ổ|Ỗ|Ơ|Ớ|Ờ|Ợ|Ở|Ỡ";
constexpr char sign_u[] = "ú|ù|ụ|ủ|ũ|ư|ứ|ừ|ự|ử|ữ";
constexpr char sign_U[] = "Ú|Ù|Ụ|Ủ|Ũ|Ư|Ứ|Ừ|Ự|Ử|Ữ";
constexpr char sign_i[] = "í|ì|ị|ỉ|ĩ";
constexpr char sign_I[] = "Í|Ì|Ị|Ỉ|Ĩ";
constexpr char sign_d[] = "đ";
constexpr char sign_D[] = "Đ";
constexpr char sign_y[] = "ý|ỳ|ỵ|ỷ|ỹ";
constexpr char sign_Y[] = "Ý|Ỳ|Ỵ|Ỷ|Ỹ";

#define MAX_THEARD 8

class ParsingTextFile
{
public:

    /**
     * Constructor
     */
    ParsingTextFile(std::string pathFile);

    /**
     * Disable copy constructor
     */
    ParsingTextFile(const ParsingTextFile&) = delete;

    /**
     * Default destuctor
     */
    ~ParsingTextFile();

    /**
     * Build block
     */
    void build();

    /**
     * Get Map inverted Index
     */
    std::map<std::string, std::vector<uint32_t>*>& getMapInvertedIndex()
    {
        return mapInvertedIndex;
    }

    /**
     * Get Map Hash table
     */
    std::unordered_map<uint32_t, std::pair<std::string, uint32_t>>& getHashTable()
    {
        return hashTable;
    }

    /**
     * Convert to unsigned string
     */
    std::string convertUnsignedString(const std::string& input);

    uint32_t getLineOfFile();

    float getNumberWordOfD()
    {
        return (float)mapInvertedIndex.size();
    }

    static std::vector<std::string> sign;

private:

    static void worker(ParsingTextFile* p);
    static std::map<std::string, std::string> convertTable;
    std::ifstream file;
    std::string pathFile;
    static std::map<std::string, std::vector<uint32_t>*> mapInvertedIndex;
    static std::unordered_map<uint32_t, std::pair<std::string, uint32_t>> hashTable;
    static std::queue <std::pair<uint32_t, std::string>*> container;
    static std::mutex mContainer;
    static std::mutex mMapInvertedIndex;
    static std::condition_variable condNotify;
    static std::thread thWorker[MAX_THEARD];
    static bool isRuning;
};
#endif //PARSINGTEXTFILE_HH
