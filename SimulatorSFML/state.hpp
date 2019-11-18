//
//  state.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef state_hpp
#define state_hpp

#include <stdio.h>

namespace Sim {
    class state{
        
    public:
        virtual void Init() = 0;
        
        virtual void HandleInput() = 0;
        virtual void Update(float dt) = 0;
        virtual void Draw(float dt) = 0; // time difference between frames, for cap'ing FPS
        
        virtual void Pause() = 0; // called when game is paused
        virtual void Resume() = 0; // called when game is resumed
    };
}


#endif /* state_hpp */
