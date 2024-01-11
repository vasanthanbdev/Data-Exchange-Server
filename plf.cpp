#include <iostream> 
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <ctime>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;


// formating the permission
string printPermissions(mode_t mode) {
    string perms[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    string perm = (S_ISDIR(mode) ? "d" : "-") 
                  + perms[(mode >> 6) & 7] 
                  + perms[(mode >> 3) & 7] 
                  + perms[mode & 7];
    return perm;
}

// formating the size
string formatSize(double size) {
    string suffix[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    while (size >= 1024 && i < 4) {
        size /= 1024;
        i++;
    }

    stringstream ss;
    ss << setprecision(2) << size << suffix[i];
    return ss.str();
}

// formating the time
string formatTime(time_t time) {
    tm *Time = localtime(&time);

    stringstream ss;
    ss << setfill('0') 
       << setw(2) << Time->tm_mday << "-"
       << setw(2) << Time->tm_mon + 1 << "-"
       << setw(4) << Time->tm_year + 1900 << "::"
       << setw(2) << Time->tm_hour << ":"
       << setw(2) << Time->tm_min << ":"
       << setw(2) << Time->tm_sec;

    return ss.str();
}


void listFiles(const char* directory) {
    DIR* dir = opendir(directory);

    if (dir == nullptr) {
        cerr << "Error openning the directory" << endl;
        return;    
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (string(entry->d_name) != "." && string(entry->d_name) != ".." && entry->d_name[0] != '.') {
            string filepath = string(directory) + "/" + string(entry->d_name);
            
            struct stat fileStat;
            if (stat(filepath.c_str(), &fileStat) == 0) {
                struct passwd *pw = getpwuid(fileStat.st_uid);

                cout << entry->d_name << " "
                     << printPermissions(fileStat.st_mode) << " "
                     << pw->pw_name << " "
                      << formatTime(fileStat.st_ctime) << " " 
                     << formatSize(fileStat.st_size) << " "
                     << formatTime(fileStat.st_mtime) << " " << endl;
            }  
            else {
                cerr << "Error getting file information" << endl;
                return;
            }
        }
    }
    closedir(dir);
}

// help menu
void helpMenu(char* command) {
    cout << "plf - Menu" << endl;
    cout << "--------------" << endl;
    cout << "Usage: " << command << " [directory]" << endl;
    cout << "Usage: " << command << " [-h]" << endl;
    cout << "------------------------" << endl;
}


int main(int argc, char** argv) {
    const char* directory;
    if (argc == 1) {
        directory = ".";
    }
    else if (argc == 2) {
        if (string(argv[1]) == "-h") {
            helpMenu(argv[0]);
            return 0;
        }
        
        directory = argv[1];
    }
    else {
        cerr << "Usage: " << argv[0] << " [directory]" << endl;
        return 1;
    }


    listFiles(directory);

    return 0;
}