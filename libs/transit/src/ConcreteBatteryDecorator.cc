#include "ConcreteBatteryDecorator.h"

ConcreteBatteryDecorator::~ConcreteBatteryDecorator(){
    //deleting dynamically allocated variables
    delete entity;
    delete nearestRecharger;
    delete toRechargerStrategy;
    delete observer;
}

void ConcreteBatteryDecorator::GetNearestRecharger(std::vector<IEntity *> scheduler, Vector3 myDronePos, Recharger*& myNearestRecharger){
    float minDis = std::numeric_limits<float>::max();
    for (auto recharger : scheduler){
        std::string type = recharger->GetDetails()["type"];

        if (type.compare("recharger") == 0){
            Recharger *tempRecharger = dynamic_cast<Recharger *>(recharger); //If the entity is a recharger it should be dynamically cast to become a recharger object
            
            if (tempRecharger){
                float disToRecharger = myDronePos.Distance(tempRecharger->GetPosition());
                
                if (disToRecharger <= minDis){ //Finding the recharger that has the closest distance to myDronePos
                    minDis = disToRecharger;
                    myNearestRecharger = tempRecharger; //Updates what the nearest recharger is
                }
            } 
        }
    }
}

void ConcreteBatteryDecorator::Update(double dt, std::vector<IEntity *> scheduler)
{   
    std::string entityName = entity->GetDetails()["name"];

    if(this->batteryPercent < 10 && !lowPercentNotify) {
        observer->Notify(" battery is critical and is below 10%", entityName, "");
        this->lowPercentNotify = true;
    }

    if (!toCharger && entity->GetAvailability() && ((this->GetBatteryPercent() - this->TotalPercentLost(dt, scheduler)) < 10)) { //Finds nearest recharger if the drone cannot complete the next trip
        toCharger = true;
        this->GetNearestRecharger(scheduler, this->entity->GetPosition(), this->nearestRecharger);

        if(nearestRecharger) {this->toRechargerStrategy = new BeelineStrategy(this->entity->GetPosition(),nearestRecharger->GetPosition()); }

        while(this->maxPercent - this->TotalPercentLost(dt, scheduler) < 10) {this->maxPercent += 20; } //If the entity cannot complete the trip with 100% battery, the max battery should increase 

        std::string rechargerName = this->nearestRecharger->GetDetails()["name"];
        observer->Notify(" is headed to recharge at ", entityName, rechargerName); //notifying users of the entity's current actions
    }

    
    if (toCharger && this->toRechargerStrategy && entity->GetAvailability()){//The entity is moving towards the recharge station 
        
        std::string rechargerName = this->nearestRecharger->GetDetails()["name"];

        if(entity->GetSpeed() != 0.0 && this->batteryPercent >= 0.001) {
            this->toRechargerStrategy->Move(this->entity, dt); 
            this->batteryPercent -= 0.001;
            this->lostPercentCounter += 1;
        }

        if(toRechargerStrategy->IsCompleted()){ //The entity has reached the recharge station
            bool isCharged = nearestRecharger->ChargeEntity(this, dt, this->maxPercent); //Charging the entity
            
            if(!notify) {
                observer->Notify(" has arrived at ", entityName, rechargerName); //notifying users of the entity's current actions
                notify = true;
            }
            
            if(isCharged){//The battery is now fully charged, so the entity (drone) should now complete its next trip 
                toCharger = false;
                this->toRechargerStrategy = nullptr;
                this->nearestRecharger = nullptr; 
                this->maxPercent = 100;
                
                observer->Notify(" is finished charging its battery", entityName, ""); //notifying users of the entity's current actions
                this->notify = false;
                this->lowPercentNotify = false;
            }
        } 
    }else if(!toCharger) //The entity has enough battery, and will complete the trip as normal
    {   
        entity->Update(dt, scheduler);
        if(batteryPercent >= 0.001 && !entity->GetAvailability()){
            this->batteryPercent -= 0.001;
            this->percentNotify = false;
        } else if(entity->GetAvailability()) {
            if(!percentNotify) {
                observer->Notify( " is available\nCurrent battery percentage(%):", entityName, std::to_string(batteryPercent));
                this->percentNotify = true;
            }  
        }
    }
}

double ConcreteBatteryDecorator::TotalPercentLost(double dt, std::vector<IEntity *> scheduler)
{
    double totalDistance = 0;
    Recharger *destinationRecharger = nullptr;
    IEntity *nearestEntity = nullptr;
    Vector3 destination;
    Vector3 finalDestination;
    Vector3 position;

    //finding the nearest entity
    float minDis = std::numeric_limits<float>::max();
    for (auto entity2 : scheduler){
        if (entity2->GetAvailability()){
            float disToEntity = this->entity->GetPosition().Distance(entity2->GetPosition());
            
            if (disToEntity <= minDis){
                minDis = disToEntity;
                nearestEntity = entity2;
            }
        }
    }
    if (nearestEntity){
        //Finding the distance from entity to the nearest entity
        destination = nearestEntity->GetPosition();
        finalDestination = nearestEntity->GetDestination();
        position = entity->GetPosition();
        totalDistance += position.Distance(destination);

        //Finding distance from nearest entity to the final destination
        std::vector<std::vector<float>> path;
        std::string strat = nearestEntity->GetStrategyName();
        std::vector<float> dest2;
        std::vector<float> finalDest2;

        dest2.push_back(destination.x);
        dest2.push_back(destination.y);
        dest2.push_back(destination.z);
        finalDest2.push_back(finalDestination.x);
        finalDest2.push_back(finalDestination.y);
        finalDest2.push_back(finalDestination.z);

        //based off of the strategy name, the path is found and helper function TotalDistance is called to find the total distance of path
        if (strat.compare("astar") == 0){
            path = graph->GetPath(dest2, finalDest2, AStar::Default());
            totalDistance += this->TotalDistance(path);
        }
        else if (strat.compare("dfs") == 0){
            path = graph->GetPath(dest2, finalDest2, DepthFirstSearch::Default());
            totalDistance += this->TotalDistance(path);
        }
        else if (strat.compare("dijkstra") == 0){
            path = graph->GetPath(dest2, finalDest2, Dijkstra::Default());
            totalDistance += this->TotalDistance(path);
        }
        else{
            totalDistance += finalDestination.Distance(destination);
        }

        //Finding distance from the final destination to its nearest recharge station
        this->GetNearestRecharger(scheduler, finalDestination, destinationRecharger);
        if(destinationRecharger) {
            double rechargerDestinationDistance = finalDestination.Distance(destinationRecharger->GetPosition());
            totalDistance += rechargerDestinationDistance;
            destinationRecharger= nullptr;
        }
        //The total battery percent lost based on the overall calculated distance
        //The calculated TotalPercentLost has a margin of error of 5% 
        double percentLost = totalDistance / (this->entity->GetSpeed() * dt) * 0.001;
        return percentLost;
    }
}

double ConcreteBatteryDecorator::TotalDistance(std::vector<std::vector<float>> path)
{
    
    std::vector<Vector3> path2;
    double distance = 0;
    for (auto vec : path){ //Calculating the distance based off of the travelled nodes in the path
        Vector3 position = {vec[0], vec[1], vec[2]};
        path2.push_back(position);
    }

    for (int i = 0; i < path2.size() - 1; i++){
        distance += path2[i].Distance(path2[i + 1]);
    }

    return distance; //Returning the total distance in the parameter path
}
