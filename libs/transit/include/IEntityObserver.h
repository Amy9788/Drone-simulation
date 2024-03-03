#ifndef I_ENTITY_OBSERVER_H
#define I_ENTITY_OBSERVER_H

#include "Publishcer.h"

class IEntityObserver{
 public:
  /**
   * @brief default constructor
   */
  IEntityObserver();

  /**
   * @brief combine the message and sent it to the publishcer (publishcer object)
   * 
   * @param msg std::string containing the message 
   * @param droneName std::string containing drone's name
   * @param robotName std::string containing robot's name 
   */
  void Notify(std::string msg, std::string droneName, std::string robotName);

  /**
   * @brief Gets publishcer object
   * 
   * @return the publishcer object
   */
  Publishcer* GetPublishcer();

 private:
  Publishcer* publishcer;
};

#endif //I_ENTITY_OBSERVER_H_
