# Simple Graphics Library (SGL) for C++

This library is a basic 2D graphical library for C++.
It is built upon and depends upon Qt, a popular cross-platform graphical framework.
It is made for students and teachers, and it is designed to be simpler to use and understand than Qt for basic graphical programs such as those found in a typical CS1 / CS2 course.
It is cross-platform and works on Windows, Mac, and Linux.
The design and feel of the provided classes is generally similar to that of Java's AWT/Swing graphical library, using C++ syntax and idioms.

This library is a fork of the Stanford C++ Library, which was originally developed by Stanford Professor Eric Roberts, with the assistance of Julie Zelenski, Keith Schwarz, and other Stanford colleagues.  Many thanks to these Stanford instructors for their excellent work.


# Support

We are sorry to say that we cannot offer any user support for these libraries at this time.
You are welcome to use the libraries, but we cannot help with compiler issues or answer other technical questions.

We are willing to accept pull requests if you find bugs or would like to add functionality.
We recommend contacting us first if you plan to add code to this project.


# Dependencies

To use this library, you need the following:

* a working C++ compiler
* the Qt library
* an editor or IDE

We have personally tested and recommend **Qt Creator** and **Visual Studio Code**.
We have also built and run the libraries from the command line using the tools `qmake` and `cmake`.
Other editors may work but we cannot provide any assistance with installation or support.


# Installation

We offer brief installation instructions for various operating systems here:

1. Install System Libraries.
 * **Windows:** None; skip.
 * **Mac OS X:** You must install **Xcode** because it includes a C++ compiler for Mac systems.
   - For modern Macs, you can find Xcode in the App Store.
   - After installing Xcode, you must **launch Xcode once**.
   - When Xcode runs, it will pop up a license agreement; click **Agree**, then type your account password if prompted. Xcode will say that it is "Installing components..."  Once this is done, you may exit Xcode and do not need to run it again.
 * **Linux:** You need the utilities `make`, `cmake`, `g++`, and `gdb`.  Install them using your distribution's package manager. For example, on Ubuntu or other Debian-based systems, type this in a terminal:

    `sudo apt-get update && sudo apt-get upgrade`
    `sudo apt-get -y install build-essential cmake openssl libssl-dev libssl1.0 libgl1-mesa-dev libqt5x11extras5`


2. Install Qt library.
 * (This is needed because our graphics library is based on Qt, which is a cross-platform graphical framework.)
 * Go to https://www.qt.io/download-qt-installer.
 * Choose the **Qt Online Installer** for your operating system and click **Download**.
 * Go to your Downloads directory, find the downloaded file, and double-click it to run the Qt installer (or mount the dmg image on Mac). (Its name will be something like **qt-unified-...**.)
 * The installer will ask you to create a free Qt account to complete the install.
 * Choose a **custom installation.**
 * For most options you can just click the default, but on each OS you must make a few customizations:
   - **Windows:** In the custom installation screen, expand the **Qt** menu, then the latest version of Qt, and then check the box for the **MinGW** compiler. (There are 32-bit and 64-bit MinGW compilers; which is correct depends on your computer's processor and operating system. You can press Ctrl+Break in Windows to see information about your OS and whether it is 32-bit or 64-bit.)
   - **Mac OS X:** In the custom installation screen, expand the **Qt** menu, then the latest version of Qt, and then check the box for **macOS** (and uncheck any other boxes).
   - **Linux:** In the custom installation screen, expand the **Qt** menu, then the latest version of Qt, and then check the box for **Desktop gcc** (and uncheck any others).
 * Continue through the screens of the installer, clicking **Next** or **Agree** as appropriate, until it is complete.
   It will take some time for Qt to download and install itself.

3. Install an Editor. (Optional)
 * (This is needed so you have a way to edit and run code.)
 * Qt includes an editor named **Qt Creator**, and you can use that editor with our library if you like.
 * But if you prefer a different editor, you will need to download and install it.
 * One recommended editor is [Visual Studio Code](https://code.visualstudio.com/), a free editor from Microsoft.
 * If you install VS Code, you should also install these two extensions:
   ** **C/C++** (ms-vscode.cpptools)
   ** **CMake Tools** (ms-vscode.cmake-tools)

4. Build and Run a Project!
 * Download this repository from GitHub.
 * Place your code into a project containing our library (we recommend putting it in `lib/`).
 * Build the library and your code using your IDE. Both Qt Creator and Visual Studio Code should be able to build/run our library using the provided `.pro` and `CMakeLists.txt` files respectively.
 * Run the project!


# API Documentation

The following documentation lists each class, function, and other member of the library.
You can consult these docs to see how to use the various library classes.

[SGL C++ Library API Documentation](doc/html/annotated.html)


# Example Code

The following example program uses SGL to create a window with an image and some text and display it on the screen:

```
// welcome.cpp - demonstrates SGL graphics library
#include <iostream>
#include "console.h"   // pops up a graphical console
#include "gwindow.h"   // for GWindow class
using namespace std;

int main()
{
    cout << "What is your name? ";
    string name;
    getline(cin, name);

    GWindow* window = new GWindow(300, 250);
    window->setTitle("SGL Starter Project");
    window->setLocation(300, 150);
    window->setExitOnClose(true);
    window->setBackground("white");
    window->clear();
    window->setColor("black");
    window->drawString("Welcome " + name + "!", 75, 200);
    window->drawImage("example.png", 75, 25);
    window->setVisible(true);

    return 0;
}
```


# Contact Us

If you have questions about this library, please contact the author, Marty Stepp, at **mstepp AT gmail DOT com**.
Thank you and good luck!
