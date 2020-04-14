//
// Created by Samuel Arbibe on 01/01/2020.
//

#include "../../sim/simulator/Settings.hpp"
#include "QsfmlCanvas.hpp"

#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif

QSFMLCanvas::QSFMLCanvas(QWidget *Parent,
                         int intervalTime,
                         int drawIntervalTime) :
	QWidget(Parent),
	RenderWindow(VideoMode(0, 0),
	             "Sim",
	             Style::Default,
	             ContextSettings(0, 0, Settings::AntiAliasing)) {
	// Setup some states to allow direct rendering into the widget
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);


	// Set strong focus to enable keyboard events to be received
	setFocusPolicy(Qt::StrongFocus);

	// Setup the widget geometry
	// setup for high DPI devices, adjust sfml widget size to actual size

	parent_ = Parent;

	Settings::SFMLRatio =
		parent_->devicePixelRatio(); // save ratio for later use
	resize(parent_->size());

	// Setup the logic timer
	logic_timer_.setTimerType(Qt::PreciseTimer);
	logic_timer_.setInterval(intervalTime);

	// Setup the draw timer
	draw_timer_.setInterval(drawIntervalTime);

	is_init_ = false;
}

void QSFMLCanvas::showEvent(QShowEvent *) {
	if (!is_init_)
	{
		// Under X11, we need to flush the commands sent to the server to ensure that
		// SFML will get an updated view of the windows
#ifdef Q_WS_X11
		XFlush(QX11Info::display());
#endif

		// Create the SFML window with the widget handle
		RenderWindow::create((sf::WindowHandle) winId());

		// Let the derived class do its specific stuff
		on_init();

		// Setup the timer to trigger a refresh at specified framerate
		connect(&logic_timer_, SIGNAL(timeout()), this, SLOT(repaint()));
		logic_timer_.start();

		// Setup the draw timer to trigger a redraw at specified framerate
		//connect(&draw_timer_, SIGNAL(timeout()), this, SLOT(redraw()));
		connect(&draw_timer_, SIGNAL(timeout()), this, SLOT(redraw()));
		draw_timer_.start();

		is_init_ = true;
	}
}

QPaintEngine *QSFMLCanvas::paintEngine() const {
	return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent *) {
	// Let the derived class do its specific stuff
	logic_cycle();
}

void QSFMLCanvas::redraw() {
	draw_cycle();
}