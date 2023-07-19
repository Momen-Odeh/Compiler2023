#include <iostream>
#include "fd.h"

//int main() {
//    FileDescriptor fd("example.txt");

//    if (!fd.IsOpen()) {
//        std::cerr << "Error opening the file." << std::endl;
//        return 1;
//    }

//    char *fileName = fd.GetFileName();
//    std::cout << "Reading file: " << fileName << std::endl;

//    char line;
//    while ((line = fd.GetChar()) != EOF) {
//        std::cout << "Line " << fd.GetLineNum()<< ": "  <<fd.GetCharNum() << ": " << line << std::endl;
//    }

//    std::cout << "Last line number: " << fd.GetLineNum() << std::endl;
//    std::cout << "Last character number: " << fd.GetCharNum() << std::endl;

//    fd.Close();

//    return 0;
//}
