/*
 * File: privatefilelib.h
 * ----------------------
 * This file exports a standardized set of tools for working with
 * files.  The library offers at least some portability across the
 * file systems used in the three supported platforms: Mac OSX,
 * Windows, and Linux.  Directory and search paths are allowed to
 * contain separators in any of the supported styles, which usually
 * makes it possible to use the same code on different platforms.
 * This functionality is considered "private" and not to be used by students.
 *
 * @version 2021/04/09
 * - moved to private SGL namespace
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2018/10/23
 * - added getAbsolutePath
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2016/11/12
 * - added fileSize, readEntireStream
 * @version 2015/04/12
 * - added promptUserForFile overload without stream parameter
 * @version 2014/10/19
 * - alphabetized function declarations
 * - converted many funcs to take const string& rather than string for efficiency
 * - added listDirectory overload that returns a Vector
 */


#ifndef _private_filelib_h
#define _private_filelib_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace sgl {
namespace priv {
namespace filelib {

/**
 * Returns <code>true</code> if the specified file exists.
 */
bool fileExists(const std::string& filename);

/**
 * Returns an absolute filename for the given file or directory.
 * This converts from, for example, "temp/foo.txt" to "/Users/jsmith12/Documents/temp/foo.txt".
 */
std::string getAbsolutePath(const std::string& path);

/**
 * Returns the standard directory path separator used on this platform.
 */
std::string getDirectoryPathSeparator();

/**
 * Returns the extension of <code>filename</code>.  The extension
 * consists of the separating dot and all subsequent characters.
 * If no dot exists in the final component, <code>getExtension</code>
 * returns the empty string.  These semantics ensure that concatenating
 * the root and the extension always returns the original filename.
 */
std::string getExtension(const std::string& filename);

/**
 * Returns all but the last component of a path name.  The components
 * of the path name can be separated by any of the directory path
 * separators (forward or reverse slashes).  The special cases are
 * illustrated by the following examples:
 *
 *<pre>
 *    getHead("a/b")  = "a"     getTail("a/b")   = "b"
 *    getHead("a")    = ""      getTail("a")     = "a"
 *    getHead("/a")   = "/"     getTail("/a")    = "a"
 *    getHead("/")    = "/"     getTail("/")     = ""
 *</pre>
 */
std::string getHead(const std::string& filename);

/**
 * Returns the operating system's "temp" folder.
 * Can be useful if your program needs to store temporary files.
 */
std::string getTempDirectory();

/**
 * Returns <code>true</code> if the specified file is a directory.
 */
bool isDirectory(const std::string& filename);

/**
 * Adds an alphabetized list of the files in the specified directory
 * to a vector that is returned.  This list excludes the
 * names <code>.</code> and <code>..</code> entries.
 */
std::vector<std::string> listDirectory(const std::string& path);

/**
 * Reads the entire contents of the specified input stream into the
 * string vector <code>lines</code>.  The client is responsible for
 * opening and closing the stream.
 */
void readEntireFile(std::istream& is, std::vector<std::string>& lines);

/**
 * An overload of readEntireFile that just returns the whole file as a very
 * long single string, rather than a vector of lines.
 * @throw ErrorException if the file is not found or cannot be read.
 */
std::string readEntireFile(const std::string& filename);

/**
 * Reads the contents of the given stream until its end and returns them as
 * a string.
 */
std::string readEntireStream(std::istream& input);

/**
 * Opens the given file and writes the given text into it.
 * Normally this function replaces any previous contents of the file, but
 * if the optional 'append' parameter is passed, the given text is added
 * at the end of the file, retaining any previous contents.
 * Returns true if the write was successful and false if the file was unable
 * to be opened for writing.
 */
bool writeEntireFile(const std::string& filename,
                     const std::string& text,
                     bool append = false);

/**
 * Platform-dependent functions that differ by operating system.
 * @private
 */
namespace platform {
    void filelib_createDirectory(const std::string& path);
    void filelib_deleteFile(const std::string& path);
    std::string filelib_expandPathname(const std::string& filename);
    bool filelib_fileExists(const std::string& filename);
    std::string filelib_getAbsolutePath(const std::string& path);
    std::string filelib_getCurrentDirectory();
    std::string filelib_getDirectoryPathSeparator();
    std::string filelib_getSearchPathSeparator();
    std::string filelib_getTempDirectory();
    bool filelib_isDirectory(const std::string& filename);
    bool filelib_isFile(const std::string& filename);
    bool filelib_isSymbolicLink(const std::string& filename);
    void filelib_listDirectory(const std::string& path, std::vector<std::string>& list);
    void filelib_setCurrentDirectory(const std::string& path);
} // namespace platform

} // namespace filelib
} // namespace priv
} // namespace sgl

#endif // _private_filelib_h
