#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

bool hasPermissionsToWrite(string directoryName);
void changePermissions(string directoryName, fs::perms permissions);

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

    string darkDirectoryPath = currentDirectory.append(hiddenDirectory);

    if (!fs::exists(fileName))
    {
        cout << "File '" << fileName << "' does not exist!" << endl;
        return 1;
    }

    if (!fs::exists(darkDirectoryPath))
    {
        cout << "Dark directory does not exist; it will be created" << endl;
        fs::create_directories(darkDirectoryPath);
        cout << "Dark directory '" << darkDirectoryPath << "' has been created" << endl;
    }

    bool canWriteToDarkDirectory = hasPermissionsToWrite(hiddenDirectory);

    if (!(canWriteToDarkDirectory))
    {
        changePermissions(
            hiddenDirectory,
            fs::perms::owner_all | fs::perms::group_read | fs::perms::group_write | fs::perms::others_read | fs::perms::others_write);
    }

    string path = darkDirectoryPath.append("/").append(fileName);
    fs::rename(fileName, path);

    changePermissions(
        hiddenDirectory,
        fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec);

    cout << "File '" << fileName << "' has been successfully moved to the dark directory" << endl;

    return 0;
}

bool hasPermissionsToWrite(string directoryName)
{
    auto permissions = fs::status(directoryName).permissions();

    bool ownerAll = fs::perms::none == (fs::perms::owner_all & permissions) ? false : true;

    bool groupWrite = fs::perms::none == (fs::perms::group_write & permissions) ? false : true;
    bool groupRead = fs::perms::none == (fs::perms::group_read & permissions) ? false : true;

    bool otherWrite = fs::perms::none == (fs::perms::others_write & permissions) ? false : true;
    bool otherRead = fs::perms::none == (fs::perms::others_write & permissions) ? false : true;

    return ownerAll && groupWrite && groupRead && otherWrite && otherRead;
}

void changePermissions(string directoryName, fs::perms permissions)
{
    fs::permissions(
        directoryName,
        permissions,
        fs::perm_options::replace);
}