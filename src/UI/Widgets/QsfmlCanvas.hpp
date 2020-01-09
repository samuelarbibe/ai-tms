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

    virtual ~QSFMLCanvas(){std::cout << "QSFML Canvas destroyed" << std::endl;}

private :

    virtual void OnInit(){}

    virtual void OnUpdate(){}

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);

    bool   myInitialized;

protected:
    QTimer myTimer;
};


#endif //SIMULATORSFML_QSFMLCANVAS_HPP
