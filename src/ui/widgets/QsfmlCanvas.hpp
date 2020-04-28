//
// Created by Samuel Arbibe on 01/01/2020.
//

#ifndef SIMULATORSFML_QSFMLCANVAS_HPP
#define SIMULATORSFML_QSFMLCANVAS_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include <iostream>
//
using namespace sf;
using namespace std;

class QSFMLCanvas : public QWidget, public RenderWindow
{
  Q_OBJECT

  public :

	QSFMLCanvas(QWidget *Parent,
	            int logicIntervalTime = 500,
	            int drawIntervalTime = 60);
	~QSFMLCanvas() { std::cout << "QSFML Canvas destroyed" << std::endl; }

  private :

	virtual void on_init() {}
	virtual void logic_cycle() {}
	virtual void draw_cycle() {}

	QWidget *parent_;

	QPaintEngine *paintEngine() const override;
	void showEvent(QShowEvent *) override;
	void paintEvent(QPaintEvent *) override;

	bool is_init_;

  private slots:
	void redraw();

  protected:
	QTimer logic_timer_;
	QTimer draw_timer_;
};

#endif //SIMULATORSFML_QSFMLCANVAS_HPP
