#include "fd.h"
#include <cstring>
#include <iostream>
using namespace std;
// Constructor to open a file
FileDescriptor::FileDescriptor(char *FileName) :
    fp(nullptr),                    // Initialize the FILE pointer to null
    line_number(0),                 // Set the initial line number to 1
    char_number(0),                 // Set the initial character number to 0
    flag(0),                        // Initialize the flag to 0
    buf_size(256),                 // Set the initial buffer size to 1024
    buffer(nullptr),                // Initialize the buffer pointer to null
    file(nullptr),                  // Initialize the file name pointer to null
    line_length(0),                 // Initialize line_length to 0
    flag2(0)                       // Initialize flag2 to 0

{
    if (FileName) {
        file = new char[strlen(FileName) + 1];
        strcpy(file, FileName);
        openFileDescriptor(FileName);
    } else {
        file = nullptr;
        fp = stdin;
        buffer = new char[buf_size];
    }
}

// Destructor
FileDescriptor::~FileDescriptor() {
    close();
    if (file)
        delete[] file;
    if (buffer)
        delete[] buffer;
}

// Returns the file name
char* FileDescriptor::getFileName() {
    return file;
}

// Checks if the file is open without errors
bool FileDescriptor::isOpen() {
    return fp != nullptr;
}

// Returns the current line in the file
char* FileDescriptor::getCurrLine() {
    return buffer;
}

// Returns the current line number in the file
int FileDescriptor::getLineNum() {
    return line_number;
}

// Returns the current character number in the line
int FileDescriptor::getCharNum() {
    return char_number;
}

// Closes the file descriptor
void FileDescriptor::close() {
    if (fp != stdin && fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
}

//read line from file and put it in buffer
void FileDescriptor::readLine(){
    char c;
    line_length=0;
    while ((c = fgetc(fp)) != EOF) {
        if(line_length==0)memset(buffer, 0,buf_size);
        if (line_length == buf_size - 1) {
            // Resize the buffer by doubling the size
            char* new_buffer = new char[buf_size * 2];
            memcpy(new_buffer, buffer, buf_size);
            delete[] buffer;
            buffer = new_buffer;
            buf_size *= 2;
        }
        buffer[line_length++] = c;
        if(c == '\n'){
            break;
        }
    }
    line_number++;
    if(c==EOF){
        buffer[line_length++]=c;
    }
}

// Gets the current character in the file
char FileDescriptor::getChar() {
    char c=EOF;
    if((char_number > line_length-1 || line_length==0)&&(buffer[line_length-1]!=EOF)){
        readLine();
        char_number=0;
    }
    if(char_number<line_length){
        c = buffer[char_number++];
    }
    return c;
}

// Reports the error specifying the current line and character
void FileDescriptor::reportError(char *msg) {
    if(buffer[line_length-1] == EOF || buffer[line_length-1] == '\n'){
        buffer[line_length-1]=0;
    }
    cout<< buffer <<endl;
    for(int i=0;i<char_number-1;i++){
        if(buffer[i]=='\t')cout<<'\t';
        else cout<<' ';
    }
    cout << '^'<<endl;
    cout << "Error: " << '"' << msg << '"' << " on line " << line_number << " of "<< getFileName() << endl;
}


void FileDescriptor::ungetChar() {
    if (char_number > 0) {
        char_number--;
    }
}

// Opens the file descriptor
bool FileDescriptor::openFileDescriptor(char *filename) {
    if (fp != nullptr) {
        reportError("File already open");
        return false;
    }

    fp = fopen(filename, "r");
    if (fp == nullptr) {
        reportError("Error opening file");
        return false;
    }

    if (buffer)
        delete[] buffer;
    buffer = new char[buf_size];

    return true;
}
