###############################################################################
# Project file for SGL student program
#
# @version 2021/04/03
# @author Marty Stepp
###############################################################################

SGL_VERSION = 2021.04.09

TEMPLATE    =   app
QT          +=  core gui widgets multimedia network
CONFIG      +=  silent debug         # quiet build and debug symbols always
CONFIG      -=  depend_includepath   # library headers not changing, don't add depend

###############################################################################
#       Find/use installed version of SGL lib and headers                     #
###############################################################################

# Library installed into per-user writable data location from QtStandardPaths
win32|win64     { QTP_EXE = qtpaths.exe } else { QTP_EXE = qtpaths }
USER_DATA_DIR   =   $$system($$[QT_INSTALL_BINS]/$$QTP_EXE --writable-path GenericDataLocation)

SGL_DIR         =   $${USER_DATA_DIR}/sgl
STATIC_LIB      =   $$system_path($${SGL_DIR}/lib/libsgl.a)

# Confirm presence of lib before build using extra target as prereq
check_lib.target    =  "$${STATIC_LIB}"
check_lib.commands  =  $(error SGL graphics library not found. See http://github.com/stepp/sgl for library install instructions)
#QMAKE_EXTRA_TARGETS +=  check_lib
#PRE_TARGETDEPS       +=  $${check_lib.target}

# link against .a, add library headers to search path
# requires libpthread, add link here
# LIBS            +=  -lsgl -lpthread
LIBS            +=  -lpthread
QMAKE_LFLAGS    =   -L$$shell_quote($${SGL_DIR}/lib)
# put PWD first in search list to allow local copy to shadow if needed
# INCLUDEPATH     +=  $$PWD "$${SGL_DIR}/include"
exists($$PWD/lib/*) {
    INCLUDEPATH *= $$PWD/lib/
}

###############################################################################
#       Configure project with custom settings                                #
###############################################################################

# remove spaces from target executable for better Windows compatibility
TARGET      =   $$replace(TARGET, " ", _)

# set DESTDIR to project root dir, this is where executable/app will deploy and run
DESTDIR     =   $$PWD

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
DEFINES     +=  main=qMain
DEFINES     +=  qMain=studentMain

DEFINES     += SGL_GRAPHICAL_CONSOLE_NO_TOOLBAR=1

###############################################################################
#       Gather files to list in Qt Creator project browser                    #
###############################################################################

# honeypot to trick Qt Creator to allow glob-all to coexist with user-added files
# Qt looks for first 'SOURCES *=' line and lists user-added .cpp/h files there.
# Afterward we glob-add files to SOURCES ourselves. Operator *= will unique
# entries, so no worries about duplicates
SOURCES         *=  ""
HEADERS         *=  ""

# Gather any .cpp or .h files within the project folder (student/starter code).
# Second argument true makes search recursive
SOURCES         *=  $$files(*.cpp, true)
HEADERS         *=  $$files(*.h, true)
# HEADERS         -=  $$files(lib/*.h, true)   # so moc will skip them

# Gather resource files (image/sound/etc) from res dir, list under "Other files"
OTHER_FILES     *=  $$files(res/*, true)
# Gather text files from root dir or anywhere recursively
OTHER_FILES     *=  $$files(*.txt, true)

###############################################################################
#       Configure compiler, compile flags                                     #
###############################################################################

# Configure flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off to avoid confusion.)

CONFIG          +=  sdk_no_version_check   # removes spurious warnings on Mac OS X

# MinGW compiler lags, be conservative and use C++11 on all platforms
# rather than special case
CONFIG          +=  c++11

# WARN_ON has -Wall -Wextra, add/remove a few specific warnings
QMAKE_CXXFLAGS_WARN_ON      +=  -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON      +=  -Werror=uninitialized
QMAKE_CXXFLAGS_WARN_ON      +=  -Wunused-parameter
QMAKE_CXXFLAGS_WARN_ON      +=  -Wmissing-field-initializers
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-old-style-cast
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-sign-compare
QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-sign-conversion
!win32 {
    QMAKE_CXXFLAGS_WARN_ON      +=  -Wno-unused-const-variable
}

*-clang { # warning flags specific to clang
    QMAKE_CXXFLAGS_WARN_ON  +=  -Wempty-init-stmt
    QMAKE_CXXFLAGS_WARN_ON  +=  -Wignored-qualifiers
}

*-g++ {   # warning flags specific to g++
    QMAKE_CXXFLAGS_WARN_ON  +=  -Wlogical-op
}

###############################################################################
#       Detect/report errors in project structure                             #
###############################################################################

# error if project opened from within a ZIP archive (common mistake on Windows)
win32|win64 {
    contains(PWD, .*\.zip.*) | contains(PWD, .*\.ZIP.*) {
        message( "*******************************************************************" )
        message( "*** ERROR: You are trying to open this project from within a ZIP archive." )
        message( "*** You must first extract the files then open in Qt Creator." )
        message( "*** In File Explorer open the ZIP and choose to Extract All." )
        message( "*******************************************************************" )
        error( Exiting. Extract project from ZIP first.)
    }
}

# error if name of directory has chars that may cause trouble for qmake/make/shell
PROJECT_DIR = $$basename(PWD)
FOUND  = $$PROJECT_DIR
FOUND ~= s|[a-z A-Z 0-9 _.+-]||   # yes, spaces ok, limited punctuation, $ % & are dicey
!isEmpty(FOUND) {
    message( "*******************************************************************" )
    message( "*** ERROR: The name of your project directory has disallowed characters." )
    message( "*** The allowed characters are letters, numbers, and simple punctuation." )
    message( "*** Your directory is named $$PROJECT_DIR which contains the" )
    message( "*** disallowed characters: $$FOUND" )
    message( "*** Please rename to a simple name such as Assignment_1 that contains" )
    message( "*** no disallowed characters." )
    message( "*******************************************************************" )
    error(Exiting. Rename project directory to remove disallowed characters. )
}
