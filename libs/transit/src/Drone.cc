#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>

Drone::Drone(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};
  observer = new IEntityObserver();
  speed = obj["speed"];
  notify = false;
  available = true;
}

Drone::~Drone() {
  // Delete dynamically allocated variables
  delete graph;
  delete nearestEntity;
  delete toRobot;
  delete toFinalDestination;
  delete observer;
}

void Drone::GetNearestEntity(std::vector<IEntity*> scheduler) {
  float minDis = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    if (entity->GetAvailability()) {
      float disToEntity = this->position.Distance(entity->GetPosition());
      if (disToEntity <= minDis) {
        minDis = disToEntity;
        nearestEntity = entity;
      }
    }
  }

  if (nearestEntity) {
      // set availability to the nearest entity
    nearestEntity->SetAvailability(false);
    available = false;
    pickedUp = false;

    destination = nearestEntity->GetPosition();
    Vector3 finalDestination = nearestEntity->GetDestination();

    toRobot = new BeelineStrategy(position, destination);

    std::string strat = nearestEntity->GetStrategyName();
    if (strat == "astar")
      toFinalDestination =
        new JumpDecorator(new AstarStrategy(destination, finalDestination, graph));
    else if (strat == "dfs")
      toFinalDestination =
        new SpinDecorator(new JumpDecorator(new DfsStrategy(destination, finalDestination, graph)));
    else if (strat == "dijkstra")
      toFinalDestination =
        new JumpDecorator(new SpinDecorator(new DijkstraStrategy(destination, finalDestination, graph)));
    else
      toFinalDestination = new BeelineStrategy(destination, finalDestination);
  }
}

void Drone::Update(double dt, std::vector<IEntity*> scheduler) {

  std::string robotName = GetNearestDetails();
  std::string droneName = GetDetails()["name"];
  if (available)
    GetNearestEntity(scheduler);

  if (toRobot) {
    toRobot->Move(this, dt);
    if(!notify){
      observer->Notify(" has taken on a trip and is on the way", droneName, robotName);
      notify = true;
    }

    if (toRobot->IsCompleted()) {
      delete toRobot;
      toRobot = nullptr;
      pickedUp = true;
      notify = false;
      observer->Notify(" has picked up ", droneName, robotName);
   
    }
  } else if (toFinalDestination) {
    toFinalDestination->Move(this, dt);
    if(!notify){
      observer->Notify(" is delivering ", droneName, robotName);
      notify = true;
    }

    if (nearestEntity && pickedUp) {
      nearestEntity->SetPosition(position);
      nearestEntity->SetDirection(direction);
    }

    if (toFinalDestination->IsCompleted()) {
      delete toFinalDestination;
      toFinalDestination = nullptr;
      nearestEntity = nullptr;
      available = true;
      pickedUp = false;
      notify = false;
    }
  }
}

void Drone::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

void Drone::Jump(double height) {
  if (goUp) {
    position.y += height;
    jumpHeight += height;
    if (jumpHeight > 5) {
      goUp = false;
    }
  } else {
    position.y -= height;
    jumpHeight -= height;
    if (jumpHeight < 0) {
      goUp = true;
    }
  }
}

std::string Drone::GetNearestDetails() const{ 
    if(nearestEntity){
      return nearestEntity->GetDetails()["name"];
    }
    return "";
}
