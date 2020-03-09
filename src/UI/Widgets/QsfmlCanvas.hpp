//
// Created by Samuel Arbibe on 01/01/2020.
//

#ifndef SIMULATORSFML_QSFMLCANVAS_HPP
#define SIMULATORSFML_QSFMLCANVAS_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include <iostream>

using namespace sf;
using namespace std;

class QSFMLCanvas : public QWidget, public RenderWindow
{
  public :

	QSFMLCanvas(QWidget *Parent, int FrameTime = 0);
	~QSFMLCanvas() { std::cout << "QSFML Canvas destroyed" << std::endl; }

  private :

	virtual void on_init() {}
	virtual void cycle() {}

	QWidget *parent_;

	QPaintEngine *paintEngine() const override;
	void showEvent(QShowEvent *) override;
	void paintEvent(QPaintEvent *) override;

	bool is_init_;

  protected:
	QTimer timer_;
};

#endif //SIMULATORSFML_QSFMLCANVAS_HPP
