#include "IEntityObserver.h"

IEntityObserver::IEntityObserver(){
    publishcer = new Publishcer();
}

void IEntityObserver::Notify(std::string msg, std::string droneName, std::string robotName){
    std::string str = droneName + msg + robotName;
    publishcer->Notify(str);
}

Publishcer* IEntityObserver::GetPublishcer(){
    return publishcer;
}

