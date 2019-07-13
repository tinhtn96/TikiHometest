#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SearchHandler.hh"

int main () {

    SearchHandler handler("what's your favourite animal");
    handler.execute();
    handler.showResult();

    // std::string test = "bo quan ao mo vai hinh voi truot patin bieber bi03005b - xanh (size 5)";
    // if (test.find("nghe") != std::string::npos)
    // {
    //     std::cout << "cadsfcvsd";
    // }
    return 0;
}