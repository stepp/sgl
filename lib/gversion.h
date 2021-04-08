/*
 * File: gversion.h
 * ----------------
 * Version message reports the version/build info of the library.
 */

#ifndef _gversion_h
#define _gversion_h

#include <string>

namespace sgl {
namespace version {
    std::string getLibraryInfoPanelMessage();
    std::string getLibraryVersion();
}
}

#endif // _gversion_h
