//
//  stateLogic.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "stateLogic.hpp"


namespace Sim {

    void stateLogic::AddState(stateRefrence newState, bool isReplacing){
        
        this->m_isAdding = true;
        this->m_isReplacing = isReplacing;
        
        this->m_newState = std::move(newState);
        
    }

    void stateLogic::RemoveState(){
         
        this->m_isRemoving = true;
    }

    void stateLogic::ProcessStateChanges(){
        
        if(this->m_isRemoving && !this->m_states.empty()){ // remove state
            
            this->m_states.pop();
            
            if(!this->m_states.empty()){ // if not empty, and there was a previous state
                this->m_states.top()->Resume(); // resume with the previous state
            }
            
            this->m_isRemoving = false; // stop removing
        }
        
        if(this->m_isAdding){ // add / replace
            
            if(!this->m_states.empty()){
                
                if(this->m_isReplacing){ // if we want to add and replace last state
                    this->m_states.pop();
                }else{
                    this->m_states.top()->Pause(); // pause the current state
                }
            }
            
            this->m_states.push(std::move(this->m_newState));
            this->m_states.top()->Init(); // initiallise the new state
            
            this->m_isAdding = false; // stop adding
        }
    }

    stateRefrence & stateLogic::GetActiveState(){
        return this->m_states.top();
    }
}
