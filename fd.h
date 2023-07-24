#ifndef FD_H
#define FD_H
#define SET 1
#define UNSET 0

#include <cstdio>

class FileDescriptor {
private:
    FILE *fp;
    int line_number; /* line number in the file */
    int char_number; /* character number in the line */
    int flag; /* to prevents two ungets in a row */
    int buf_size; /* stores the buffer size */
    char *buffer; /* buffer to store a line */
    char *file; /* file name, allocate memory for this */
    int line_length; /*to define the current length of buffer*/
    int flag2;

public:

    // Constructor to open a file
    FileDescriptor(char *FileName = nullptr);

    // Destructor
    ~FileDescriptor();

    // Returns the file name
    char *getFileName();

    // Checks if the file is open without errors
    bool isOpen();

    // Returns the current line in the file
    char *getCurrLine();

    // Returns the current line number in the file
    int getLineNum();

    // Returns the current character number in the line
    int getCharNum();

    // Closes the file descriptor
    void close();

    // Gets the current character in the file
    char getChar();

    // Reports the error specifying the current line and character
    void reportError(char *msg);

    //read line and store it in buffer
    void readLine();

    void ungetChar();

    // Opens the file descriptor
    bool openFileDescriptor(char *filename);
};

#endif // FD_H
