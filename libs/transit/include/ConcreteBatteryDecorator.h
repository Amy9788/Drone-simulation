#ifndef CONCRETE_BATTERY_DECORATOR_
#define CONCRETE_BATTERY_DECORATOR_

#include "BatteryDecorator.h"
#include "routing/astar.h"
#include "routing/depth_first_search.h"
#include "routing/dijkstra.h"

/**
 * @class ConcreteBatteryDecorator
 * 
 * @brief Inherits from BatteryDecorator. Represents the battery that will be wrapped around the drone in a physical system.
 *
 * A ConcreteBatteryDecorator takes in an entity, in this case it will be a drone which the BatteryDecorator will decorate onto. This class
 * inherits from IEntity and BatteryDecorator so the ability to get speed, position, direction, destination, and details are all available.
 */
class ConcreteBatteryDecorator : public BatteryDecorator {
  public:
    /**
    * @brief Construct a new ConcreteBatteryDecorator object
    *
    * @param entity the entity to decorate onto
    */    
    ConcreteBatteryDecorator(IEntity* entity) : BatteryDecorator(entity) {};

    /**
    * @brief Destructor: destoys a ConcreteBatteryDecorator object
    */    
    ~ConcreteBatteryDecorator();

    /**
    * @brief updates the position of an entity
    *
    * @param scheduler: A vector comprised of IEntity pointers, dronePos: A Vector3 coordinate depicting the drones position, nearestRecharger: a Recharger pointer to the nearest recharger, toRechargerStrategy: pointer of IStrategy of the strategy used
    */    
    void Update(double dt, std::vector<IEntity*> scheduler) override;

    /**
    * @brief A virtual functiono that finds the nearest recharger that is available
    *
    * @param scheduler: A vector comprised of IEntity pointers, dronePos: A Vector3 coordinate depicting the drones position, nearestRecharger: a Recharger pointer to the nearest recharger, toRechargerStrategy: pointer of IStrategy of the strategy used
    */    
    void GetNearestRecharger(std::vector<IEntity*> scheduler, Vector3 myDronePos, Recharger*& myNearestRecharger) override;

    /**
    * @brief A functiono that calculates the total percent lost
    *
    * @param scheduler: dt: delta time, scheduler: a vector of IEntity pointers
    *
    * @return a double of the percent lost
    */
    double TotalPercentLost(double dt, std::vector<IEntity*> scheduler) override;

    /**
    * @brief A function that calculates the total distance of a path
    *
    * @param scheduler: path: a vector of vector floats
    *
    * @return a double of the total distance
    */    
    double TotalDistance(std::vector<std::vector<float>> path);

};
#endif //CONCRETE_BATTERY_DECORATOR_