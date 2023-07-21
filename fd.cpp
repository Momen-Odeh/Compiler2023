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
        open_file_descriptor(FileName);
    } else {
        file = nullptr;
        fp = stdin;
        buffer = new char[buf_size];
    }
}

// Destructor
FileDescriptor::~FileDescriptor() {
    Close();
    if (file)
        delete[] file;
    if (buffer)
        delete[] buffer;
}

// Returns the file name
char* FileDescriptor::GetFileName() {
    return file;
}

// Checks if the file is open without errors
bool FileDescriptor::IsOpen() {
    return fp != nullptr;
}

// Returns the current line in the file
char* FileDescriptor::GetCurrLine() {
    return buffer;
}

// Returns the current line number in the file
int FileDescriptor::GetLineNum() {
    return line_number;
}

// Returns the current character number in the line
int FileDescriptor::GetCharNum() {
    return char_number;
}

// Closes the file descriptor
void FileDescriptor::Close() {
    if (fp != stdin && fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
}

//read line from file and put it in buffer
void FileDescriptor::ReadLine(){
    delete[] buffer;//delete the previous data in buffer
    buffer = new char[buf_size];//reinitialize buffer
    char c;
    line_length=0;
    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (line_length == buf_size - 1) {
            // Resize the buffer by doubling the size
            char* new_buffer = new char[buf_size * 2];
            memcpy(new_buffer, buffer, buf_size);
            delete[] buffer;
            buffer = new_buffer;
            buf_size *= 2;
        }
        buffer[line_length++] = c;
    }
    buffer[line_length] = '\0';
    if(line_length==0) buffer[0]=EOF;
    else line_number++;
}

// Gets the current character in the file
char FileDescriptor::GetChar() {
    char c;
    if(char_number > line_length || line_length==0){
        ReadLine();
        char_number=0;
    }
    c = buffer[char_number++];
    if(c==EOF)char_number=0;
    return c;
}

// Reports the error specifying the current line and character
void FileDescriptor::ReportError(char *msg) {
    cout<< buffer <<endl;
    for(int i=0;i<char_number;i++){
        cout<<' ';
    }
    cout << '^'<<endl;
    cerr << "Error: " << '"' << msg << '"' << " on line " << line_number << " of "<< GetFileName() << endl;
}


void FileDescriptor::UngetChar() {
    if (char_number > 0) {
        char_number--;
    }
}

// Opens the file descriptor
bool FileDescriptor::open_file_descriptor(char *filename) {
    if (fp != nullptr) {
        ReportError("File already open");
        return false;
    }

    fp = fopen(filename, "r");
    if (fp == nullptr) {
        ReportError("Error opening file");
        return false;
    }

    if (buffer)
        delete[] buffer;
    buffer = new char[buf_size];

    return true;
}
