#ifndef RECHARGER_H_
#define RECHARGER_H_

#include "SimulationModel.h"

/**
 * @class Recharger
 * 
 * @brief Represents a recharge station in a physical system. 
 * 
 **/
class Recharger : public IEntity {
 public:
  /**
   * @brief Rechargers are created with a name
   * 
   * @param obj JSON object containing the recharger's information
   */
  Recharger(JsonObject& obj);

  /**
   * @brief Destroy the Recharger object
   */
  ~Recharger();

  /**
   * @brief Gets the speed of the recharger
   * 
   * @return The float speed of the recharger
   */
  float GetSpeed() const { return speed; } 

  /**
   * @brief Gets the position of the recharger
   * 
   * @return The Vector3 position of the recharger
   * 
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the destination of the recharger
   * 
   * @return The destination of the recharger which is a Vector3 object
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the direction of the recharger
   * 
   * @return The Vetor3 direction of the recharger
   */
  Vector3 GetDirection() const { return direction; } 

  /**
   * @brief Gets the details information of the recharger
   * 
   * @return The details information of the recharger
   */
  JsonObject GetDetails() const { return details; }

  /**
     * @brief Gets the availability of the recharger
     * 
     * @return The boolean representing whether a recharger is available to charge an entity
     */
  bool GetAvailability() const{return available; }

  /**
     * @brief charges an entity's battery 
     * 
     * @param myEntity The entity that will be charged 
     * @param dt Delta time ( how much the entity will charge )
     * 
     * @return The boolean representing whether the entity has been charged to its max percentage or not
     */
  bool ChargeEntity(IEntity* myEntity, double dt, double maxPercent); 

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
  bool available; 
};

#endif //RECHARGER_H_
