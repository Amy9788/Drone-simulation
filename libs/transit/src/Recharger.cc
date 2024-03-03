#define _USE_MATH_DEFINES
#include "Recharger.h"
#include "ConcreteBatteryDecorator.h"

#include <cmath>
#include <limits>


Recharger::Recharger(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  direction = {1,0,0}; 
  destination = {pos[0], pos[1], pos[2]};
  speed = 0.0; 
  available = false;
}

Recharger::~Recharger() {
  // Delete dynamically allocated variables
  delete graph;
}

bool Recharger::ChargeEntity(IEntity* entity, double dt, double maxPercent) {
  BatteryDecorator* myEntity = dynamic_cast<BatteryDecorator*>(entity);
  bool chargingComplete = false;

  if(myEntity) { 
      if((myEntity->GetBatteryPercent() + dt) <= maxPercent){ //If the battery can be increased by dt and not reach its max capacity
        myEntity->SetBatteryPercent(myEntity->GetBatteryPercent() + dt); //increasing battery percentage of entity
      }else {
        chargingComplete = true; //battery is at or near max capacity and is done charging
      }
  } else {
    std::cout << "recharger is null" << std::endl;
  }
  
  return chargingComplete;
} 
