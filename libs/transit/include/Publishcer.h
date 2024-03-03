#ifndef PUBLISHCER_H_
#define PUBLISHCER_H_

#include "IListener.h"
#include <vector>
#include <string>

class Publishcer {
 public:
   /**
    * @brief default constructor
    */
   Publishcer() {}

   /**
    * @brief Adds given listeners
    * 
    * @param listenerObj - IListener to be added.
    */
   void Subcribe(IListener* listenerObj);

   /**
    * @brief remove given listeners
    * 
    * @param listenerObj - IListener to be removed.
    */
   void UnSubcribe(IListener* listenerObj);

   /**
    * @brief notify the subriber
    * 
    * @param msg std::string message need to be notified
    */  
   void Notify(std::string msg);

  /**
    * @brief Gets listeners vector that contain IListener objects
    * 
    * @return Gets listeners vector
    */
   std::vector<IListener*> GetListeners() const;

  private:
   std::vector<IListener*> listeners;
};

#endif // PUBLISHCER_H_
