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

class QSFMLCanvas : public QWidget, public RenderWindow
{
public :

    QSFMLCanvas(QWidget* Parent, unsigned int FrameTime = 0);
    ~QSFMLCanvas(){std::cout << "QSFML Canvas destroyed" << std::endl;}
    void ResizeEvent(QResizeEvent* event);

private :

    virtual void on_init(){}
    virtual void on_update(){}

    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);

    bool   is_init_;
protected:
    QTimer timer_;
};


#endif //SIMULATORSFML_QSFMLCANVAS_HPP
