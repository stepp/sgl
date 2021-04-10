/*
 * File: gsound.cpp
 * ----------------
 * Implementation of the GSound class.
 * 
 * @version 2021/04/09
 * - added sgl namespace
 * @version 2021/04/03
 * - removed dependencies
 * - renamed to GSound
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unused include statements
 */

#include "gsound.h"
#include <exception>
#include <stdexcept>
#include <QUrl>
#include "gthread.h"
#include "require.h"
#include "privatefilelib.h"

namespace sgl {

/*static*/ QMediaPlayer* GSound::_qmediaPlayer = nullptr;

/*static*/ long GSound::getDuration() {
    initialize();
    return _qmediaPlayer->duration();
}

/*static*/ int GSound::getVolume() {
    initialize();
    return _qmediaPlayer->volume();
}

/*static*/ void GSound::initialize() {
    if (!_qmediaPlayer) {
        GThread::runOnQtGuiThread([]() {
            if (!_qmediaPlayer) {
                _qmediaPlayer = new QMediaPlayer;
            }
        });
    }
}

/*static*/ void GSound::pause() {
    initialize();
    _qmediaPlayer->pause();
}

/*static*/ void GSound::playSound(const std::string& filename) {
    initialize();
    std::string absPath = sgl::priv::filelib::getAbsolutePath(filename);
    if (!sgl::priv::filelib::fileExists(absPath)) {
        throw std::runtime_error("GSound::playSound: file not found: " + filename);
    }

    GThread::runOnQtGuiThreadAsync([absPath]() {
        _qmediaPlayer->setMedia(QUrl::fromLocalFile(QString::fromStdString(absPath)));
        _qmediaPlayer->play();
    });
}

/*static*/ void GSound::stop() {
    initialize();
    _qmediaPlayer->stop();
}

/*static*/ void GSound::setVolume(int volume) {
    initialize();
    require::inRange(volume, 0, 100, "GSound::setVolume", "volume");
    _qmediaPlayer->setVolume(volume);
}

GSound::GSound(const std::string& filename)
        : _filename(filename) {
    initialize();
}

GSound::~GSound() {
    // TODO
}

void GSound::play() {
    playSound(_filename);
}

} // namespace sgl
