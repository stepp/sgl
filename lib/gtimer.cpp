/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 * 
 * @version 2021/04/09
 * - added sgl namespace
 * @version 2019/01/23
 * - added destructor
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unneeded include statements
 */

#include "gtimer.h"
#include <exception>
#include <stdexcept>
#include "gthread.h"
#include "gwindow.h"
#include "require.h"

/* Implementation of the GTimer class */

namespace sgl {

GTimer::GTimer(double milliseconds)
        : _ms(0),
          _id(-1) {
    setDelay(milliseconds);
}

GTimer::~GTimer() {
    stop();
}

double GTimer::getDelay() const {
    return _ms;
}

bool GTimer::isStarted() const {
    return _id >= 0;
}

void GTimer::restart() {
    stop();
    start();
}

void GTimer::setDelay(double ms) {
    require::positive(ms, "GTimer::setDelay", "delay (ms)");
    _ms = ms;
    if (isStarted()) {
        restart();
    }
}

void GTimer::start() {
    _Internal_QMainWindow* lastWindow = static_cast<_Internal_QMainWindow*>(GWindow::getLastWindow());
    if (!lastWindow) {
        throw std::runtime_error("GTimer::start: You must create at least one GWindow before starting a GTimer.");
        return;
    }
    GThread::runOnQtGuiThreadAsync([this, lastWindow]() {
        _id = lastWindow->timerStart(_ms);
    });
}

void GTimer::stop() {
    _Internal_QMainWindow* lastWindow = static_cast<_Internal_QMainWindow*>(GWindow::getLastWindow());
    if (!lastWindow) {
        throw std::runtime_error("GTimer::constructor: You must create at least one GWindow before stopping a GTimer.");
        return;
    }
    GThread::runOnQtGuiThreadAsync([this, lastWindow]() {
        lastWindow->timerStop(_id);
        _id = -1;
    });
}

} // namespace sgl
