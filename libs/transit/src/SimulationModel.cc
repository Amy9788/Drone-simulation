#include "SimulationModel.h"

#include "DroneFactory.h"
#include "RobotFactory.h"
#include "HumanFactory.h"
#include "HelicopterFactory.h"
#include "RechargerFactory.h"
#include "ConcreteBatteryDecorator.h"
#include "TransitServer.h"

#include <fstream>
#include <iostream>

SimulationModel::SimulationModel(IController& controller)
    : controller(controller) {
  compFactory = new CompositeFactory();
  AddFactory(new DroneFactory());
  AddFactory(new RobotFactory());
  AddFactory(new HumanFactory());
  AddFactory(new HelicopterFactory());
  AddFactory(new RechargerFactory());

  transitServer = new TransitServer(); 
  oldMsg = "";
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (int i = 0; i < entities.size(); i++) {
    delete entities[i];
  }
  for (int i = 0; i < scheduler.size(); i++) {
    delete scheduler[i];
  }
  delete graph;
  delete compFactory;
}

void SimulationModel::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  std::cout << name << ": " << position << std::endl;


  IEntity* myNewEntity = compFactory->CreateEntity(entity);

  if(type.compare("drone") == 0){
    myNewEntity->GetObserver()->GetPublishcer()->Subcribe(transitServer); //setting up the drone notifications  

    myNewEntity->SetGraph(graph);
    myNewEntity = new ConcreteBatteryDecorator(myNewEntity); //Wrapping the drone with the battery decorator

    myNewEntity->GetObserver()->GetPublishcer()->Subcribe(transitServer); //setting up the drone notifications for the wrapped drone
  }


  myNewEntity->SetGraph(graph);

  // Call AddEntity to add it to the view
  controller.AddEntity(*myNewEntity);
  entities.push_back(myNewEntity);

  //Adding recharger objects to scheduler
  if(type.compare("recharger") == 0){
    scheduler.push_back(myNewEntity);
  }
}

//Schedules a trip for an object in the scene
void SimulationModel::ScheduleTrip(JsonObject& details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  for (auto entity : entities) {  //Add the entity to the scheduler
    JsonObject detailsTemp = entity->GetDetails();
    std::string nameTemp = detailsTemp["name"];
    std::string typeTemp = detailsTemp["type"];
    if (name.compare(nameTemp) == 0 && typeTemp.compare("robot") == 0 &&
        entity->GetAvailability()) {
      std::string strategyName = details["search"];
      entity->SetDestination(Vector3(end[0], end[1], end[2]));
      entity->SetStrategyName(strategyName);
      scheduler.push_back(entity);
      break;
    }
  }
  controller.SendEventToView("TripScheduled", details);
}

// Updates the simulation
void SimulationModel::Update(double dt) {
  for (int i = 0; i < entities.size(); i++) {
    entities[i]->Update(dt, scheduler);
    controller.UpdateEntity(*entities[i]);
  }
  std::string curr = transitServer->GetCurrentMsg();
  if(curr != oldMsg){
    std::cout << curr << std::endl;
    oldMsg = curr;

    JsonObject obj;
    obj["event"] = "observe";
    obj["details"] = curr;
    controller.SendEventToView("observe", obj);

  }
}

void SimulationModel::AddFactory(IEntityFactory* factory) {
  compFactory->AddFactory(factory);
}
