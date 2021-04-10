/*
 * File: console.cpp
 * -----------------
 * This file implements the console .h interface.
 *
 * @author Marty Stepp
 * @version 2019/04/12
 * - added pause() implementation (empty) in headless mode
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/10/01
 * - bug fix for graphical console popping up even if not included
 * @version 2018/08/23
 * - renamed to console .cpp/h to replace Java version
 * - separated out gconsolewindow.h/cpp
 * @version 2018/07/15
 * - initial version, based on io/console .cpp
 */

#include "consoletext.h"
#include <cstdio>
#include "gconsolewindow.h"
#include "gexceptions.h"
#include "gthread.h"
#include "gstatic.h"

// Default can be overridden with explicit call to setConsoleBlah
static const int DEFAULT_X = -1, DEFAULT_Y = -1; // will center
static const int DEFAULT_WIDTH = 900, DEFAULT_HEIGHT = 500;
static const bool DEFAULT_ECHO = true, DEFAULT_EXIT_ON_CLOSE = true;

#ifdef SGL_HEADLESS_MODE

void clearConsole() {
    // empty
}

bool getConsoleClearEnabled() {
    return true;
}

/* GWindow::CloseOperation */ int getConsoleCloseOperation() {
    return 0;
}

bool getConsoleEcho() {
    return true;
}

bool getConsoleEnabled() {
    return true;
}

bool getConsoleEventOnClose() {
    return true;
}

bool getConsoleExitProgramOnClose() {
    return true;
}

std::string getConsoleFont() {
    return "";
}

double getConsoleHeight() {
    return 0;
}

bool getConsoleLocationSaved() {
    return false;
}

bool getConsolePrintExceptions() {
    return gexceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return false;
}

double getConsoleWidth() {
    return 0;
}

std::string getConsoleWindowTitle() {
    return "";
}

void pause(double /*milliseconds*/) {
    // empty
}

void setConsoleClearEnabled(bool /*value*/) {
    // empty
}

void setConsoleCloseOperation(int /*op*/) {
    // empty
}

void setConsoleEcho(bool /*echo*/) {
    // empty
}

void setConsoleEnabled(bool /*enabled*/) {
    // empty
}

void setConsoleErrorColor(const std::string& /*color*/) {
    // empty
}

void setConsoleEventOnClose(bool /*eventOnClose*/) {
    // empty
}

void setConsoleExitProgramOnClose(bool /*exitOnClose*/) {
    // empty
}

void setConsoleFont(const std::string& /*font*/) {
    // empty
}

void setConsoleLocation(double /*x*/, double /*y*/) {
    // empty
}

void setConsoleLocationSaved(bool /*value*/) {
    // empty
}

void setConsoleOutputColor(const std::string& /*color*/) {
    // empty
}

void setConsoleSettingsLocked(bool /*value*/) {
    // empty
}

void setConsoleSize(double /*width*/, double /*height*/) {
    // empty
}

void setConsoleWindowTitle(const std::string& /*title*/) {
    // empty
}

void shutdownConsole() {
    // empty
}

#else // SGL_HEADLESS_MODE

void clearConsole() {
    ::sgl::GConsoleWindow::instance()->clearConsole();
}

bool getConsoleClearEnabled() {
    return ::sgl::GConsoleWindow::instance()->isClearEnabled();
}

/* GWindow::CloseOperation */ int getConsoleCloseOperation() {
    return ::sgl::GConsoleWindow::instance()->getCloseOperation();
}

bool getConsoleEcho() {
    return ::sgl::GConsoleWindow::instance()->isEcho();
}

bool getConsoleEnabled() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return false;
#else
    return ::sgl::GConsoleWindow::consoleEnabled();
#endif
}

bool getConsoleEventOnClose() {
    return true;
}

bool getConsoleExitProgramOnClose() {
    return ::sgl::GConsoleWindow::instance()->getCloseOperation() == ::sgl::GWindow::CLOSE_EXIT;
}

std::string getConsoleFont() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return ::sgl::GConsoleWindow::getDefaultFont();
#else
    return ::sgl::GConsoleWindow::instance()->getFont();
#endif
}

double getConsoleHeight() {
    return ::sgl::GConsoleWindow::instance()->getHeight();
}

::sgl::GPoint getConsoleLocation() {
    return ::sgl::GConsoleWindow::instance()->getLocation();
}

bool getConsoleLocationSaved() {
    return ::sgl::GConsoleWindow::instance()->isLocationSaved();
}

bool getConsolePrintExceptions() {
    return ::sgl::exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return ::sgl::GConsoleWindow::isInitialized()
            && ::sgl::GConsoleWindow::instance()->isLocked();
}

::sgl::GDimension getConsoleSize() {
    return ::sgl::GConsoleWindow::instance()->getSize();
}

double getConsoleWidth() {
    return ::sgl::GConsoleWindow::instance()->getWidth();
}

::sgl::GConsoleWindow* getConsoleWindow() {
    return ::sgl::GConsoleWindow::instance();
}

std::string getConsoleWindowTitle() {
    return ::sgl::GConsoleWindow::instance()->getTitle();
}

void setConsoleClearEnabled(bool value) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setClearEnabled(value);
}

void setConsoleCloseOperation(/*GWindow::CloseOperation*/ int op) {
    ::sgl::GWindow::CloseOperation gwcop = static_cast<::sgl::GWindow::CloseOperation>(op);
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setCloseOperation(gwcop);
}

void setConsoleEcho(bool echo) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setEcho(echo);
}

void setConsoleEnabled(bool enabled) {
    ::sgl::GConsoleWindow::setConsoleEnabled(enabled);
}

void setConsoleErrorColor(const std::string& color) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setErrorColor(color);
}

void setConsoleEventOnClose(bool /*eventOnClose*/) {
    // empty
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setExitOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setFont(font);
}

void setConsoleLocation(double x, double y) {
    if (getConsoleSettingsLocked()) { return; }
    if ((int) x == -1 && (int) y == -1) {
        ::sgl::GConsoleWindow::instance()->center();
    } else {
        ::sgl::GConsoleWindow::instance()->setLocation(x, y);
    }
}

void setConsoleLocationSaved(bool value) {
    ::sgl::GConsoleWindow::instance()->setLocationSaved(value);
}

void setConsoleOutputColor(const std::string& color) {
    ::sgl::GConsoleWindow::instance()->setOutputColor(color);
}

void setConsoleSettingsLocked(bool value) {
    ::sgl::GConsoleWindow::instance()->setLocked(value);
}

void setConsoleSize(double width, double height) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setConsoleSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (getConsoleSettingsLocked()) { return; }
    ::sgl::GConsoleWindow::instance()->setTitle(title);
}

void shutdownConsole() {
    if (getConsoleEnabled() && !::sgl::GConsoleWindow::instance()->isLocked()) {
        ::sgl::GConsoleWindow::instance()->shutdown("Shutdown");
    }
}

#endif // SGL_HEADLESS_MODE

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * to default values
 */
void setConsolePropertiesQt() {
#if defined(SPL_CONSOLE_FONTSIZE)
    std::string fontStr = std::string("Monospaced-Bold-") + std::to_string(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

    setConsoleSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setConsoleLocation(DEFAULT_X, DEFAULT_Y);
 #if defined(SPL_CONSOLE_LOCATION_SAVED)
    setConsoleLocationSaved(true);
 #endif
    setConsoleEcho(DEFAULT_ECHO);
    setConsoleExitProgramOnClose(DEFAULT_EXIT_ON_CLOSE);
}

void initializeQtGraphicalConsole() {
#ifndef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    // ensure that console is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    static std::ios_base::Init ios_base_init;

#ifndef SGL_HEADLESS_MODE
    if (::sgl::GConsoleWindow::consoleEnabled()) {
        ::sgl::GConsoleWindow::instance();   // ensure that console window is ready
        ::setConsolePropertiesQt();
    }
#endif // SGL_HEADLESS_MODE

#endif // __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
}
