#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_
#include "Recharger.h"

/**
 * @class BatteryDecorator
 * 
 * @brief Represents the battery that will be wrapped around the drone in a physical system.
 *
 * A BatteryDecorator takes in an entity, in this case it will be a drone which the BatteryDecorator will decorate onto. This class
 * inherits from IEntity so the ability to get speed, position, direction, destination, and details are all available.
 */
class BatteryDecorator : public IEntity{   
  public:
    /**
    * @brief Construct a new BatteryDecorator object
    *
    * @param entity the entity to decorate onto
    */
    BatteryDecorator(IEntity* entity); 

    /**
    * @brief Destructor: destoys a BatteryDecorator object
    * 
    */
    ~BatteryDecorator();

    /**
    * @brief Gets the entity speeds.  A constant function
    *
    * @return the speed of the entity
    */
    float GetSpeed() const {return entity->GetSpeed();}

    /**
    * @brief Gets the position of the entity. A constant function
    *
    * @return the position of the entity
    */
    Vector3 GetPosition() const{return entity->GetPosition();}

    /**
    * @brief Gets the direction of the entity. A constant function
    *
    * @return the direction of the entity
    */
    Vector3 GetDirection() const{return entity->GetDirection();}

    /**
    * @brief Gets the destination of the entity. A constant function
    *
    * @return the destination of the entity
    */
    Vector3 GetDestination() const {return entity->GetDestination();}

    /**
    * @brief Gets the details of the entity. A constant function
    *
    * @return the details of the entity
    */
    JsonObject GetDetails() const {return entity->GetDetails();}

    /**
    * @brief Gets the availability of the entity.
    *
    * @return a bool if the recharger is available or not
    */    
    bool GetAvailability() {return entity->GetAvailability();}

    /**
    * @brief sets the graph
    *
    * @param des_: graph: a const value of a pointer to IGraph
    */
    void SetGraph(const IGraph* graph) {this->graph = graph;}

    /**
    * @brief sets the destination of the entity
    *
    * @param des_: A vec3 of the destination
    */
    void SetDestination(Vector3 des_){ entity->SetDestination(des_);}

    /**
    * @brief sets the strategy name
    *
    * @param strategyName_: a string representing the strategy name
    */
    void SetStrategyName(std::string strategyName_) { entity->SetStrategyName(strategyName_);}

    /**
    * @brief sets the battery percent of the battery wrapped around the entity
    *
    * @param percent: The new updated percent of the battery
    */
    double SetBatteryPercent(double percent){this->batteryPercent = percent;}

    /**
    * @brief Gets the battery of the entity.
    *
    * @return the percent of the battery
    */    
    double GetBatteryPercent(){return this->batteryPercent;}

    /**
    * @brief A virtual functiono that finds the nearest recharger that is available
    *
    * @param scheduler: A vector comprised of IEntity pointers, drone_pos: A Vector3 coordinate depicting the drones position, nearestRecharger: a Recharger pointer to the nearest recharger, to_recharger_strategy: pointer of IStrategy of the strategy used
    */
    virtual void GetNearestRecharger(std::vector<IEntity*> scheduler, Vector3 drone_pos, Recharger*& myNearestRecharger) = 0; 

    /**
    * @brief A virtual functiono that calculates the total percent lost
    *
    * @param scheduler: dt: delta time, scheduler: a vector of IEntity pointers
    *
    * @return a double of the percent lost
    */    
    virtual double TotalPercentLost(double dt, std::vector<IEntity*> scheduler) = 0;  
  
  protected:
    double batteryPercent;
    IEntity* entity;
    Recharger* nearestRecharger; 
    bool toCharger;
    bool notify;
    bool lowPercentNotify;
    bool percentNotify;
    IStrategy* toRechargerStrategy;
    int lostPercentCounter;
    double maxPercent;
};

#endif // BATTERY_DECORATOR_H_
