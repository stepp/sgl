/*
 * File: welcome.cpp
 * -----------------
 * Sample program used to confirm SGL install.
 */

#include <iostream>
#include <vector>
#include "console.h"
#include "gwindow.h"
using namespace std;

void welcomeAlert(string name)
{
    GWindow* window = new GWindow(300, 250);
    window->setTitle("SGL Starter Project");
    window->setLocation(300, 150);
    window->setExitOnClose(true);
    window->setBackground("White");
    window->clear();
    window->setColor("black");
    window->drawString("Welcome " + name + "!", 75, 200);
    window->drawImage("res/edmonds.png", 75, 25);
    window->setVisible(true);
}

int main()
{
    cout << "What is your name? ";
    string name;
    getline(cin, name);
    welcomeAlert(name);
    return 0;
}
