#include "Publishcer.h"

void Publishcer::Subcribe(IListener* listenerObj){
    listeners.push_back(listenerObj);
}


void Publishcer::UnSubcribe(IListener* listenerObj){
    for (auto i = listeners.begin(); i != listeners.end(); i++){
        if (*i == listenerObj) {
            listeners.erase(i);
            return;
        }
    }
}

void Publishcer::Notify(std::string msg){
    for(int i =0; i< listeners.size(); i++){
        listeners[i]->Update(msg);
    }

}

std::vector<IListener*> Publishcer::GetListeners() const {
    return listeners;
}
