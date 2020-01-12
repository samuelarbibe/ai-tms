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
#include "../UI/Widgets/QsfmlCanvas.hpp"
#include "Map/Map.hpp"
#include "Vehicle.hpp"
#include "Flags.hpp"

using namespace sf;

class Engine : public QSFMLCanvas
{
public:

    Engine(QWidget* Parent) : QSFMLCanvas(Parent, 1000.f/MAX_FPS){}
    Map * map;

private:

    void OnInit();
    void OnUpdate();
    void OnDraw();

    void input();
    void update(float elapsedTime);


};


#endif /* Engine_hpp */
