/*
 * File: gversion.cpp
 * ------------------
 */

#include "gversion.h"

#include <QString>
#include <QStandardPaths>

#ifndef SGL_VERSION
#define SGL_VERSION "20210406"
#endif

#ifdef SGL_SHARED_INSTALL
static std::string installed_location()
{
    QString qs = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    return qs.toStdString() + "/sgl";
}
#endif // SGL_SHARED_INSTALL

namespace sgl {
namespace version {

    std::string getLibraryInfoPanelMessage()
    {
        return "<html><p>" "Simple Graphics Library (SGL) version " SGL_VERSION "<br><br>"
#ifdef SGL_SHARED_INSTALL
            "<small>Installed into " + installed_location() + "<br><br>"
#endif // SGL_SHARED_INSTALL
            "Library written and maintained by Marty Stepp.<br>"
            "&copy; 2021.<br><br>"
            "This library is a fork of the Stanford C++ Library,<br>"
            "which is maintained by Julie Zelenski and others.<br>"
            "Many thanks to Julie and the team at Stanford."
        ;
    }

    std::string getLibraryVersion()
    {
        return std::string(SGL_VERSION);
    }
} // namespace version
} // namespace sgl
