#ifndef HELICOPTER_H_
#define HELICOPTER_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @class Helicopter
 * 
 * @brief Represents a helicopter in a physical system. 
 **/
class Helicopter : public IEntity {
 public:
  /**
   * @brief Helicopter are created with a name
   * 
   * @param obj JSON object containing the helicoptor's information
   */
  Helicopter(JsonObject& obj);

  /**
   * @brief Destroy the Helicopter object
   */
  ~Helicopter();

  /**
   * @brief Gets the speed of the helicoptor
   * 
   * @return The speed of the helicoptor
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the helicoptor
   * 
   * @return The position of the helicoptor
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the helicoptor
   * 
   * @return The direction of the helicoptor
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the helicoptor
   * 
   * @return The destination of the helicoptor
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the helicoptor
   * 
   * @return The details information of the helicoptor
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Updates the helicoptor's position
   * 
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the helicoptor
   * 
   * @param pos_ The new position of the helicoptor
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the helicoptor
   * 
   * @param dir_ The new direction of the helicoptor
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the helicoptor
   * 
   * @param des_ The new destination of the helicoptor
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Generates a new randomized destination for the helicopter
   */
  void CreateNewDestination();

  /**
   * @brief Rotates the helicoptor
   * 
   * @param angle The angle by which the helicoptor should be rotated
   */
  void Rotate(double angle);

  private:
    JsonObject details;
    Vector3 position;
    Vector3 direction;
    Vector3 destination;
    float speed;
    IStrategy* toDestination = nullptr;
};

#endif // HELICOPTER_H_
