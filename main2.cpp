#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

using namespace std;

bool exists(string filePath);
bool hasPermissionsToWrite(string directoryPath);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Invalid amount of arguments!" << endl;
        return 1;
    }

    string fileName = argv[1];
    string currentDirectory = "./";
    string hiddenDirectory = ".hidden";

    string darkDirectoryPath = currentDirectory + hiddenDirectory;

    if (!exists(fileName))
    {
        cout << "File '" << fileName << "' does not exist!" << endl;
        return 1;
    }

    if (!exists(darkDirectoryPath))
    {
        cout << "Dark directory does not exist; it will be created" << endl;
        mkdir(darkDirectoryPath.c_str(), 0755);
        cout << "Dark directory '" << darkDirectoryPath << "' has been created" << endl;
    }

    bool canWriteToDarkDirectory = hasPermissionsToWrite(hiddenDirectory);

    if (!(canWriteToDarkDirectory))
    {
        chmod(darkDirectoryPath.c_str(), 0755);
    }

    string toName = darkDirectoryPath + "/" + fileName;
    rename(fileName.c_str(), toName.c_str());

    chmod(darkDirectoryPath.c_str(), 0111);

    cout << "File '" << fileName << "' has been successfully moved to the dark directory" << endl;

    return 0;
}

bool hasPermissionsToWrite(string directoryPath)
{
    struct stat buffer;
    stat(directoryPath.c_str(), &buffer);

    mode_t mode = buffer.st_mode;
    if (
        (mode & S_IRWXU) == S_IRWXU &&
        (mode & S_IRGRP) == S_IRGRP &&
        (mode & S_IXGRP) == S_IXGRP &&
        (mode & S_IROTH) == S_IROTH &&
        (mode & S_IXOTH) == S_IXOTH)
    {
        return true;
    }
    return false;
}

bool exists(string filePath)
{
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}