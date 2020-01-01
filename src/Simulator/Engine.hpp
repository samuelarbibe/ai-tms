//
//  Engine.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include "../UI/QsfmlCanvas.hpp"
#include "Map/Map.hpp"
#include "Vehicle.hpp"
#include "Flags.hpp"

using namespace sf;

class Engine : public QSFMLCanvas
{
public:

    Engine(QWidget* Parent, const QPoint& Position, const QSize& Size);

private:

    void OnInit();
    void OnUpdate();
    void OnDraw();

    void input();
    void update(float elapsedTime);

    Map * map;
};


#endif /* Engine_hpp */
