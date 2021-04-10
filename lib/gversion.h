/*
 * File: gversion.h
 * ----------------
 * Version message reports the version/build info of the library.
 *
 * @version 2021/04/09
 * - added sgl namespace
 */

#ifndef _gversion_h
#define _gversion_h

#include <string>

namespace sgl {
    std::string getLibraryInfoPanelMessage();
    std::string getLibraryVersion();
}

#endif // _gversion_h
