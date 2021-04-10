cmake_minimum_required(VERSION 3.1.0)
project (sgl VERSION 2021.04.03 LANGUAGES CXX)

# use C++11 language standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# tell CMake to run moc when necessary
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

# as moc files are generated in the binary dir, tell CMake
# to always look for includes there:
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Link to Qt5 graphical libraries
find_package(Qt5 COMPONENTS Widgets Multimedia Network REQUIRED)

# Configure flags for the C++ compiler
# (In general, many warnings/errors are enabled to tighten compile-time checking.
# A few overly pedantic/confusing errors are turned off to avoid confusion.)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(
	-g
	-Wall
	-Wextra
	-Werror=return-type
	-Werror=uninitialized
	-Wunused-parameter
	-Wmissing-field-initializers
	-Wno-old-style-cast
	-Wno-sign-compare
	-Wno-sign-conversion
)

add_compile_options(
	-DSGL_GRAPHICAL_CONSOLE_NO_TOOLBAR=1
)

# student writes ordinary main() function, but it must be called within a
# wrapper main() that handles library setup/teardown. Rename student's
# to distinguish between the two main() functions and avoid symbol clash
add_compile_options(
	-Dmain=qMain
	-DqMain=studentMain
)

# convenience variables to represent all source / header files to compile
FILE(GLOB LibSources
	lib/*.cpp
)

FILE(GLOB LibHeaders
	lib/*.h
)

# non-sgl-library source / header files specific to this project
FILE(GLOB ProjectSources
	src/*.cpp
)

FILE(GLOB ProjectHeaders
	src/*.h
)

# resource files (images, input files, etc.) for this project
FILE(GLOB ProjectResources
	res/*
)

set(sgl_HDRS
	${LibHeaders}
	${ProjectHeaders}
	# lib/build.h
	# lib/console.h
	# lib/consolestreambuf.h
	# lib/consoletext.h
	# lib/echoinputstreambuf.h
	# lib/forwardingstreambuf.h
	# lib/gbrowserpane.h
	# lib/gbutton.h
	# lib/gcanvas.h
	# lib/gcheckbox.h
	# lib/gchooser.h
	# lib/gclipboard.h
	# lib/gcolorchooser.h
	# lib/gcolor.h
	# lib/gconsolewindow.h
	# lib/gcontainer.h
	# lib/gdiffgui.h
	# lib/gdiffimage.h
	# lib/gdownloader.h
	# lib/gdrawingsurface.h
	# lib/gevent.h
	# lib/geventqueue.h
	# lib/gexceptions.h
	# lib/gfilechooser.h
	# lib/gfontchooser.h
	# lib/gfont.h
	# lib/ginit.h
	# lib/ginteractor.h
	# lib/glabel.h
	# lib/glayout.h
	# lib/gmath.h
	# lib/gobjects.h
	# lib/gobservable.h
	# lib/goptionpane.h
	# lib/gradiobutton.h
	# lib/grid.h
	# lib/gridlocation.h
	# lib/gscrollbar.h
	# lib/gscrollpane.h
	# lib/gslider.h
	# lib/gsound.h
	# lib/gspacer.h
	# lib/gstatic.h
	# lib/gtable.h
	# lib/gtextarea.h
	# lib/gtextfield.h
	# lib/gthread.h
	# lib/gtimer.h
	# lib/gtypes.h
	# lib/gversion.h
	# lib/gwindow.h
	# lib/privatediff.h
	# lib/privatefilelib.h
	# lib/privateregexpr.h
	# lib/privatestrlib.h
	# lib/qtgui.h
	# lib/require.h
)

set(sgl_SRCS
	${LibSources}
	${ProjectSources}
	# lib/console.cpp
	# lib/gbrowserpane.cpp
	# lib/gbutton.cpp
	# lib/gcanvas.cpp
	# lib/gcheckbox.cpp
	# lib/gchooser.cpp
	# lib/gclipboard.cpp
	# lib/gcolorchooser.cpp
	# lib/gcolor.cpp
	# lib/gconsolewindow.cpp
	# lib/gcontainer.cpp
	# lib/gdiffgui.cpp
	# lib/gdiffimage.cpp
	# lib/gdownloader.cpp
	# lib/gdrawingsurface.cpp
	# lib/gevent.cpp
	# lib/geventqueue.cpp
	# lib/gexceptions.cpp
	# lib/gfilechooser.cpp
	# lib/gfontchooser.cpp
	# lib/gfont.cpp
	# lib/ginit.cpp
	# lib/ginteractor.cpp
	# lib/glabel.cpp
	# lib/glayout.cpp
	# lib/gmath.cpp
	# lib/gobjects.cpp
	# lib/gobservable.cpp
	# lib/goptionpane.cpp
	# lib/gradiobutton.cpp
	# lib/gridlocation.cpp
	# lib/gscrollbar.cpp
	# lib/gscrollpane.cpp
	# lib/gslider.cpp
	# lib/gsound.cpp
	# lib/gspacer.cpp
	# lib/gtable.cpp
	# lib/gtextarea.cpp
	# lib/gtextfield.cpp
	# lib/gthread.cpp
	# lib/gtimer.cpp
	# lib/gtypes.cpp
	# lib/gversion.cpp
	# lib/gwindow.cpp
	# lib/librarymain.cpp
	# lib/mainwrapper.cpp
	# lib/privatediff.cpp
	# lib/privatefilelib.cpp
	# lib/privatefilelibunix.cpp
	# lib/privatefilelibwindows.cpp
	# lib/privateregexpr.cpp
	# lib/privatestrlib.cpp
	# lib/qtgui.cpp
	# lib/require.cpp
)

# all libraries to link
set(sgl_LIBS
	-lpthread
)

add_executable(StarterProject
	${sgl_SRCS}
	images.qrc
)

qt5_use_modules(StarterProject
	Widgets
	Multimedia
	Network
)

# copy resource files from res/ folder over to the build destination folder
foreach(resFile ${ProjectResources})
	get_filename_component(resFileName ${resFile} NAME)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/${resFileName} ${CMAKE_CURRENT_BINARY_DIR}/${resFileName} COPYONLY)
endforeach()
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

target_include_directories(StarterProject
	PRIVATE
	lib/
	src/
)

target_link_libraries(StarterProject
	${sgl_LIBS}
)
