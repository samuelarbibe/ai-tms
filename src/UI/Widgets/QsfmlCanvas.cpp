//
// Created by Samuel Arbibe on 01/01/2020.
//

#include <src/Simulator/Settings.hpp>
#include "QsfmlCanvas.hpp"

#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, unsigned int FrameTime) :
        QWidget       (Parent),
        myInitialized (false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the widget geometry
    // setup for high DPI devices, adjust sfml widget size to actual size

    Settings::SFMLRatio = Parent->devicePixelRatio(); // save ratio for later use
    resize(Parent->size());

    // Setup the timer
    myTimer.setInterval(FrameTime);
}

void QSFMLCanvas::resizeEvent(QResizeEvent* event)
{
    setSize(sf::Vector2u(QWidget::width(), QWidget::height()));
    std::cout << "resize called" << std::endl;
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
#ifdef Q_WS_X11
        XFlush(QX11Info::display());
#endif

        // Create the SFML window with the widget handle
        RenderWindow::create((sf::WindowHandle)winId());

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
    OnUpdate();

    // Display on screen
    display();
}
