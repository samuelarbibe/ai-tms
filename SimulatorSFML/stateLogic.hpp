//
//  stateLogic.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef stateLogic_hpp
#define stateLogic_hpp

#include <stdio.h>
#include <memory>
#include <stack>

#include "state.hpp"

namespace Sim {
    typedef std::unique_ptr<state> stateRefrence;

    class stateLogic{
    public:
        stateLogic(){}
        ~stateLogic(){}
    
        void AddState(stateRefrence newState, bool isReplacing = true); // are you replacing the current state?
        void RemoveState(); // manually remove the top state
        
        void ProcessStateChanges(); // apply states
        
        stateRefrence &GetActiveState(); // get the active state VALUE
        
    private:
        std::stack<stateRefrence> m_states;
        stateRefrence m_newState;
        
        //these flags will declare what to do with the state stack
        bool m_isRemoving;
        bool m_isAdding;
        bool m_isReplacing;
        
    };
}


#endif /* stateLogic_hpp */
