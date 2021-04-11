/*
 * File: privatefilelib.cpp
 * ------------------------
 * This file implements the privatefilelib.h interface.
 * Platform-dependent functions are handled through filelib_* functions
 * with different implementations for Windows and Mac/Linux.
 * This functionality is considered "private" and not to be used by students.
 *
 * @version 2021/04/09
 * - moved to private SGL namespace
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2016/11/20
 * - small bug fix in readEntireStream method (failed for non-text files)
 * @version 2016/11/12
 * - added fileSize, readEntireStream
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * - moved appendSpace function to simpio
 * @version 2015/04/12
 * - added promptUserForFile overload without stream parameter
 * @version 2014/10/19
 * - alphabetized function declarations
 * - converted many funcs to take const string& rather than string for efficiency
 * - added listDirectory overload that returns a Vector
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "privatefilelib.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <exception>
#include <stdexcept>
#include "privatestrlib.h"

namespace sgl {
namespace priv {
namespace filelib {

static std::string expandPathname(const std::string& filename) {
    return platform::filelib_expandPathname(filename);
}

bool fileExists(const std::string& filename) {
    return platform::filelib_fileExists(::sgl::priv::strlib::trim(filename));
}

std::string getAbsolutePath(const std::string& path) {
    return platform::filelib_getAbsolutePath(path);
}

std::string getDirectoryPathSeparator() {
    return platform::filelib_getDirectoryPathSeparator();
}

std::string getExtension(const std::string& filename) {
    int dot = -1;
    int len = filename.length();
    for (int i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '.') dot = i;
        if (ch == '/' || ch == '\\') dot = -1;
    }
    if (dot == -1) {
        return "";
    } else {
        return filename.substr(dot);
    }
}

std::string getHead(const std::string& filename) {
    size_t slash = std::string::npos;
    size_t len = filename.length();
    for (size_t i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '/' || ch == '\\') slash = i;
    }
    if (slash == std::string::npos) {
        return "";
    } else if (slash == 0) {
        return "/";
    } else {
        return filename.substr(0, slash);
    }
}

std::string getTempDirectory() {
    return platform::filelib_getTempDirectory();
}

bool isDirectory(const std::string& filename) {
    return platform::filelib_isDirectory(expandPathname(filename));
}

std::vector<std::string> listDirectory(const std::string& path) {
    std::vector<std::string> vec;
    platform::filelib_listDirectory(expandPathname(path), vec);
    return vec;
}

std::string readEntireStream(std::istream& input) {
    std::ostringstream output;
    while (true) {
        int ch = input.get();
        if (input.fail()) {
            break;
        }
        output.put(ch);
    }
    return output.str();
}

void readEntireFile(std::istream& is, std::vector<std::string>& lines) {
    lines.clear();
    while (true) {
        std::string line;
        getline(is, line);
        if (is.fail()) {
            break;
        }
        lines.push_back(line);
    }
}

std::string readEntireFile(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        throw std::runtime_error(std::string("input file not found or cannot be opened: ") + filename);
    }
    return readEntireStream(input);
}

bool writeEntireFile(const std::string& filename,
                     const std::string& text,
                     bool append) {
    std::ofstream output;
    if (append) {
        output.open(filename.c_str(), std::ios_base::out | std::ios_base::app);
    } else {
        output.open(filename.c_str());
    }
    if (output.fail()) {
        return false;
    }
    output << text;
    output.close();
    return !output.fail();
}

} // namespace filelib
} // namespace priv
} // namespace sgl


// BEGIN PLATFORM-SPECIFIC CODE

/*
 * This code contains Unix implementations of privatefilelib.h primitives.
 *
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2018/10/23
 * - added getAbsolutePath
 */

// define all of the following only on non-Windows OS
// (see filelibwindows.cpp for Windows versions)
#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdint.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <stdexcept>
#include <string>

namespace sgl {
namespace priv {
namespace filelib {
namespace platform {

void filelib_createDirectory(const std::string& path) {
    std::string pathStr = path;
    if (::sgl::priv::strlib::endsWith(path, "/")) {
        pathStr = path.substr(0, path.length() - 1);
    }
    if (mkdir(pathStr.c_str(), 0777) != 0) {
        if (errno == EEXIST && filelib_isDirectory(pathStr)) {
            return;
        }
        std::string msg = "createDirectory: ";
        std::string err = std::string(strerror(errno));
        throw std::runtime_error(msg + err);
    }
}

void filelib_deleteFile(const std::string& path) {
    remove(path.c_str());
}

std::string filelib_expandPathname(const std::string& filename) {
    if (filename == "") {
        return "";
    }
    int len = filename.length();
    std::string expanded = filename;
    if (expanded[0] == '~') {
        int spos = 1;
        while (spos < len && expanded[spos] != '\\' && expanded[spos] != '/') {
            spos++;
        }
        char *homedir = nullptr;
        if (spos == 1) {
            homedir = getenv("HOME");
            if (!homedir) {
                homedir = getpwuid(getuid())->pw_dir;
            }
        } else {
            struct passwd *pw = getpwnam(expanded.substr(1, spos - 1).c_str());
            if (!pw) {
                throw std::runtime_error("expandPathname: No such user");
            } else {
                homedir = pw->pw_dir;
            }
        }
        expanded = std::string(homedir) + expanded.substr(spos);
        len = expanded.length();
    }
    for (int i = 0; i < len; i++) {
        if (expanded[i] == '\\') {
            expanded[i] = '/';
        }
    }
    return expanded;
}

bool filelib_fileExists(const std::string& filename) {
    if (filename.empty()) {
        return false;
    }
    struct stat fileInfo;
    return stat(filename.c_str(), &fileInfo) == 0;
}

std::string filelib_getAbsolutePath(const std::string& path) {
    char realpathOut[4096];
    realpath(path.c_str(), realpathOut);
    std::string absPath(realpathOut);
    return absPath;
}

std::string filelib_getCurrentDirectory() {
    char currentDirBuf[4096] = {'\0'};
    char* cwd = getcwd(currentDirBuf, 4096 - 1);
    std::string result;
    if (cwd) {
        result = std::string(cwd);
    } else {
        throw std::runtime_error("getCurrentDirectory: " + std::string(strerror(errno)));
    }
    return result;
}

std::string filelib_getDirectoryPathSeparator() {
    return "/";
}

std::string filelib_getSearchPathSeparator() {
    return ":";
}

// http://stackoverflow.com/questions/8087805/
// how-to-get-system-or-user-temp-folder-in-unix-and-windows
std::string filelib_getTempDirectory() {
    char* dir = getenv("TMPDIR");
    if (!dir) dir = getenv("TMP");
    if (!dir) dir = getenv("TEMP");
    if (!dir) dir = getenv("TEMPDIR");
    if (!dir) return "/tmp";
    return dir;
}

bool filelib_isDirectory(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISDIR(fileInfo.st_mode) != 0;
}

bool filelib_isFile(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISREG(fileInfo.st_mode) != 0;
}

bool filelib_isSymbolicLink(const std::string& filename) {
    struct stat fileInfo;
    if (lstat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISLNK(fileInfo.st_mode) != 0;
}

void filelib_listDirectory(const std::string& path, std::vector<std::string>& list) {
    DIR* dir = opendir(path.empty() ? "." : path.c_str());
    if (!dir) {
        throw std::runtime_error(std::string("listDirectory: Can't open \"") + path + "\"");
    }
    list.clear();
    while (true) {
        struct dirent* ep = readdir(dir);
        if (!ep) {
            break;
        }
        std::string name = std::string(ep->d_name);
        if (name != "." && name != "..") {
            list.push_back(name);
        }
    }
    closedir(dir);
    std::sort(list.begin(), list.end());
}

void filelib_setCurrentDirectory(const std::string& path) {
    if (chdir(path.c_str()) != 0) {
        std::string msg = "setCurrentDirectory: ";
        std::string err = std::string(strerror(errno));
        throw std::runtime_error(msg + err);
    }
}

} // namespace filelib
} // namespace priv
} // namespace sgl
} // namespace platform

#endif // _WIN32



/*
 * This code contains Windows implementations of privatefilelib.h primitives.
 *
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2018/10/23
 * - added getAbsolutePath
 */

// define all of the following only on Windows OS
// (see filelibunix.cpp for non-Windows versions)
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#undef MOUSE_EVENT
#undef KEY_EVENT
#undef MOUSE_MOVED
#undef HELP_KEY
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <stdexcept>
#include <string>

namespace sgl {
namespace priv {
namespace filelib {
namespace platform {

void filelib_createDirectory(const std::string& path) {
    std::string pathStr = path;
    if (::sgl::priv::strlib::endsWith(path, "\\")) {
        pathStr = path.substr(0, path.length() - 1);
    }
    if (CreateDirectoryA(path.c_str(), nullptr) == 0) {
        if (GetLastError() == ERROR_ALREADY_EXISTS && filelib_isDirectory(pathStr)) {
            return;
        }
        throw std::runtime_error("createDirectory: Can't create " + path);
    }
}

void filelib_deleteFile(const std::string& path) {
    if(filelib_isDirectory(path)) {
        RemoveDirectoryA(path.c_str());
    } else {
        DeleteFileA(path.c_str());
    }
}

std::string filelib_expandPathname(const std::string& filename) {
    if (filename.empty()) {
        return "";
    }
    std::string filenameStr = filename;
    for (int i = 0, len = filename.length(); i < len; i++) {
        if (filenameStr[i] == '/') {
            filenameStr[i] = '\\';
        }
    }
    return filenameStr;
}

bool filelib_fileExists(const std::string& filename) {
    if (filename.empty()) {
        return false;
    }
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

std::string filelib_getAbsolutePath(const std::string& path) {
    char realpathOut[4096];
    if (_fullpath(realpathOut, path.c_str(), 4095)) {
        std::string absPath(realpathOut);
        return absPath;
    } else {
        return path;
    }
}

std::string filelib_getCurrentDirectory() {
    char path[MAX_PATH + 1];
    int n = GetCurrentDirectoryA(MAX_PATH + 1, path);
    return std::string(path, n);
}

std::string filelib_getDirectoryPathSeparator() {
    return "\\";
}

std::string filelib_getSearchPathSeparator() {
    return ";";
}

std::string filelib_getTempDirectory() {
    char path[MAX_PATH + 1];
    int n = GetTempPathA(MAX_PATH + 1, path);
    return std::string(path, n);
}

bool filelib_isDirectory(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/gg258117(v=vs.85).aspx
bool filelib_isFile(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool filelib_isSymbolicLink(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

void filelib_listDirectory(const std::string& path, std::vector<std::string> & list) {
    std::string pathStr = path;
    if (pathStr == "") {
        pathStr = ".";
    }
    std::string pattern = pathStr + "\\*.*";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("listDirectory: Can't list directory \"" + pathStr + "\"");
    }
    list.clear();
    while (true) {
        std::string name = std::string(fd.cFileName);
        if (name != "." && name != "..") {
            list.push_back(name);
        }
        if (!FindNextFileA(h, &fd)) {
            break;
        }
    }
    FindClose(h);
    std::sort(list.begin(), list.end());
}

void filelib_setCurrentDirectory(const std::string& path) {
    if (!filelib_isDirectory(path) || !SetCurrentDirectoryA(path.c_str())) {
        throw std::runtime_error("setCurrentDirectory: Can't change to " + path);
    }
}

} // namespace filelib
} // namespace priv
} // namespace sgl
} // namespace platform

#endif // _WIN32

// END PLATFORM-SPECIFIC CODE
