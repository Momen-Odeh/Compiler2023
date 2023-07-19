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
    char *GetFileName();

    // Checks if the file is open without errors
    bool IsOpen();

    // Returns the current line in the file
    char *GetCurrLine();

    // Returns the current line number in the file
    int GetLineNum();

    // Returns the current character number in the line
    int GetCharNum();

    // Closes the file descriptor
    void Close();

    // Gets the current character in the file
    char GetChar();

    // Reports the error specifying the current line and character
    void ReportError(char *msg);

    //read line and store it in buffer
    void ReadLine();

    void UngetChar(char c);

    // Opens the file descriptor
    bool open_file_descriptor(char *filename);
};

#endif // FD_H
