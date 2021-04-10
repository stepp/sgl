/*
 * Test file for verifying the Stanford C++ lib functionality.
 */

#include <iostream>
#include <fstream>
#include <random>
#include "gbutton.h"
#include "gcanvas.h"
#include "gcolor.h"
#include "gevent.h"
#include "gfilechooser.h"
#include "glabel.h"
#include "gobjects.h"
#include "gtimer.h"
#include "gwindow.h"

using namespace std;
using namespace sgl;

void testQwindowDrawing();

int main() {
    testQwindowDrawing();
    return 0;
}

void grayscale(GCanvas* image) {
    // convert image to grayscale
    image->setAutoRepaint(false);
    for (int x = 0; x < image->getWidth(); x++) {
        for (int y = 0; y < image->getHeight(); y++) {
            int rgb = image->getRGB(x, y);
            int r = GCanvas::getRed(rgb);
            int g = GCanvas::getGreen(rgb);
            int b = GCanvas::getBlue(rgb);
            int avg = (r + g + b) / 3;
            rgb = GCanvas::createRgbPixel(avg, avg, avg);
            image->setRGB(x, y, rgb);
        }
    }
    image->repaint();
}

void testQwindowDrawing() {
    static const bool TEST_BACKGROUND = true;
    static const bool TEST_FOREGROUND = true;
    static const bool TEST_LAYOUT_WIDGETS = true;

    static GWindow* window = nullptr;
    static GOval* ball = nullptr;
    static GButton* button = nullptr;
    static GButton* button2 = nullptr;
    static GButton* button3 = nullptr;
    static GButton* button4 = nullptr;
    static GButton* button5 = nullptr;
    static GButton* clearButton = nullptr;
    static GButton* saveButton = nullptr;
    static GButton* scaleButton = nullptr;
    static GButton* rotateButton = nullptr;
    static GButton* setSizeButton = nullptr;
    static GLabel* label = nullptr;
    static GLabel* label2 = nullptr;
    static double dx = 5;
    static double dy = 3;
    static GImage* image = nullptr;
//    static GImage* image2 = nullptr;
//    static GImage* image3 = nullptr;

    window = new GWindow(900, 500);
    // window->setCanvasSize(900, 500);
    window->setTitle("QtGui Drawing Window");
    // window->setResizable(false);
    // window->setBackground("yellow");
    window->setExitOnClose(true);
    window->center();

    if (TEST_LAYOUT_WIDGETS) {
        label = new GLabel("QtGui <b>AWESOME</b> <i>cool</i> window");
        label2 = new GLabel("????????????");
        window->addToRegion(label2, "North");
        window->addToRegion(label, "North");

        window->setRegionAlignment("North", "Left");
        window->setRegionAlignment("South", "Right");

        button = new GButton("Tick");
        button2 = new GButton("Pixels");
        button3 = new GButton("Repaint");
        button4 = new GButton("Clear PX");
        button5 = new GButton("BG");
        button5->setBackground("#77ff00ff");
        button5->setColor("#77000000");
        clearButton = new GButton("Clear");
        saveButton = new GButton("Save");
        scaleButton = new GButton("Scale");
        rotateButton = new GButton("Rotate");
        setSizeButton = new GButton("setSize");

        window->addToRegion(button, "South");
        window->addToRegion(button2, "South");
        window->addToRegion(button3, "South");
        window->addToRegion(button4, "South");
        window->addToRegion(button5, "South");
        window->addToRegion(saveButton, "South");
        window->addToRegion(clearButton, "South");
        window->addToRegion(scaleButton, "South");
        window->addToRegion(rotateButton, "South");
        window->addToRegion(setSizeButton, "South");
    }

//        window->setBackground("yellow");
//        window->setColor("red");
//        window->setFillColor("red");

    // drawing directly onto window
//    window->setColor("blue");
//    window->setFillColor("yellow");
//    window->fillOval(20, 120, 40, 60);
//    window->setColor("red");
//    window->setFillColor("green");
//    window->setLineWidth(3);
//    window->fillRect(10, 30, 120, 70);
//    window->drawLine(100, 100, 200, 150);

    // pixely stuff

    // background layer
    if (TEST_BACKGROUND) {
        window->setColor("black");
        window->drawRect(400, 200, 50, 50);

        window->setFillColor("red");
        window->setLineWidth(5);   // BUG: affects later shapes
        window->setLineStyle(GObject::LINE_DASH);
        window->drawRect(20, 220, 100, 100);
        window->fillArc(20, 220, 100, 100, 45, 120);

        window->setFillColor("purple");
        window->setFont("Monospaced-bold-16");
        window->fillPolygon({200, 200, 250, 300, 150, 300});
    }

    // foreground layer
    static GImage* gimage = nullptr;
//    static GImage* gimage2 = nullptr;
    if (TEST_FOREGROUND) {
        ball = new GOval(20, 20, 50, 50);
        ball->setFillColor("#aaff0033");
        ball->setLineWidth(2);
        ball->scale(1.5);
        ball->setLineStyle(GObject::LINE_DOT);
        window->add(ball);

        window->add(new GRect(400, 100, 50, 50));

        window->setColor("green");
        GText* gtext = new GText("Hello, Qt!", 200, 80);
        gtext->setColor("green");
        gtext->rotate(10);
        window->add(gtext);

        GText* gtext2 = new GText("Bye, Felicia!", 220, 100);
        gtext2->setColor("red");
        gtext2->scale(1.5);
        window->add(gtext2);

//        gimage = new GImage("triangle-icon.png", 200, 40);
//        gimage->setOpacity(0.6);
//        window->add(gimage);

//        std::ifstream input("triangle-icon.png", ios::binary);
//        if (!input) throw new std::runtime_error("Oops - file is missing?");
//        gimage2 = new GImage(input, 300, 40);
//        gimage2->setOpacity(0.6);
//        window->add(gimage2);

        window->setColor("blue");
        GText* gtext3 = new GText("Third string", 240, 120);
        gtext3->setColor("blue");
        window->add(gtext3);

        GCanvas* canvas = new GCanvas(200, 100);   // , 0x99ff2299);
        canvas->setColor("red");
        canvas->setFillColor("green");
        canvas->fillRect(20, 20, 50, 30);
        image = canvas->toGImage();
        window->add(image, 300, 100);

//        GCanvas* canvas2 = new GCanvas("triangle-icon.png");
//        image2 = canvas2->toGImage();
//        window->add(image2, 500, 100);

//        std::ifstream input2("triangle-icon.png", ios::binary);
//        if (!input2) throw new std::runtime_error("Oops: file is missing?");
//        GCanvas* canvas3 = new GCanvas(input2);
//        image3 = canvas3->toGImage();
//        image3->setLocation(600, 100);
//        window->add(image3);
    }

    if (TEST_LAYOUT_WIDGETS) {
        std::function<void()> tickFunc = []() {
            window->pause(20);
            ball->move(dx, dy);
            double cw = window->getCanvasWidth();
            double ch = window->getCanvasHeight();
            if (ball->getX() < 0 || ball->getRightX() >= cw) {
                dx = -dx;
            }
            if (ball->getY() < 0 || ball->getBottomY() >= ch) {
                dy = -dy;
            }
            // cout << "ball loc is now: " << ball->getLocation() << endl;
        };
        button->setActionListener(tickFunc);

        button2->setActionListener([]() {
            // int** pixels = window->getPixelsArray();
            vector<vector<int>> pixels = window->getPixels();
            int randy = rand() % 256;
            for (int x = 0, w = window->getCanvasWidth(); x < w; x++) {
                for (int y = 0, h = window->getCanvasHeight(); y < h; y++) {
                    int r = (x * randy) % 256;
                    int g = (y * randy) % 256;
                    int b = ((x+y) * randy) % 256;
                    // window->setPixel(x, y, r, g, b);
                    pixels[x][y] = GColor::convertRGBToRGB(r, g, b);
                }
            }
            window->setPixels(pixels);
            // delete[] pixels;
        });

        button3->setActionListener([]() {
            window->repaint();
        });

        button4->setActionListener([]() {
            window->clearCanvasPixels();
        });

        button5->setActionListener([]() {
            window->setBackground("yellow");
        });

        clearButton->setActionListener([]() {
            window->clear();
        });

        saveButton->setActionListener([]() {
            string filename = GFileChooser::showSaveDialog();
            window->saveCanvasPixels(filename);
        });

        scaleButton->setActionListener([]() {
            if (gimage) {
                gimage->scale(1.2);
            }
        });

        rotateButton->setActionListener([]() {
            if (gimage) {
                gimage->rotate(1);
            }
        });

        setSizeButton->setActionListener([]() {
            if (gimage) {
                gimage->setSize(gimage->getWidth() * 1.1, gimage->getHeight() * 1.1);
            }
        });
    }

    if (TEST_LAYOUT_WIDGETS && TEST_BACKGROUND) {
        window->setMouseListener([](GEvent event) {
            if (event.getType() == MOUSE_DRAGGED) {
                window->setColor("blue");
                window->setFillColor("blue");
                window->setLineStyle(GObject::LINE_SOLID);
                window->setLineWidth(1);
                window->fillOval(event.getX() - 5, event.getY() - 5, 10, 10);
            } else if (event.getType() == MOUSE_MOVED) {
                std::string px = "";
                if (window->inCanvasBounds(event.getX(), event.getY())) {
                    px = window->getPixelString(event.getX(), event.getY());
                }
                label->setText(event.getLocation().toString() + " " + px);
                cout << "mouse moved: " << event.getLocation().toString() << endl;
            }
        });
    }

    if (TEST_FOREGROUND) {
        window->setKeyListener([](GEvent event) {
            // cout << "key! event=" << event << endl;
            double balldx = 0;
            double balldy = 0;
            if (event.getKeyCode() == GEvent::LEFT_ARROW_KEY) {
                balldx = -2;
            } else if (event.getKeyCode() == GEvent::RIGHT_ARROW_KEY) {
                balldx = 2;
            } else if (event.getKeyCode() == GEvent::UP_ARROW_KEY) {
                balldy = -2;
            } else if (event.getKeyCode() == GEvent::DOWN_ARROW_KEY) {
                balldy = 2;
            } else if (event.getKeyCode() == 'f') {
                ball->sendToFront();
            }
            ball->move(balldx, balldy);
        });
    }

//    window->setWindowHandler([](GEvent /* event */) {
//        // cout << "window! event=" << event << endl;
//    });

//    for (int i = 0; i < 1000; i++) {
//        tickFunc();
//    }

//    GTimer timer(2);
//    timer.start();
//    while (true) {
//        GEvent event = waitForEvent(MOUSE_EVENT | TIMER_EVENT);
//        int x = randomInteger(0, 800);
//        int y = randomInteger(0, 400);
//        GOval* oval = new GOval(x, y, 10, 20);
//        window->add(oval);
//    }
}
