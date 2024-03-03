#ifndef TRANSIT_SERVER_H_
#define TRANSIT_SERVER_H_

#include "IListener.h"
#include "util/json.h"
#include "IController.h"

class TransitServer: public IListener{
 public:
   /**
    * @brief default constructor
    */
   TransitServer();

   /**
    * @brief send the message to the texfile
    * 
    * @param msg std::string message need to be send
    */ 
   void Update(std::string msg);

   /**
    * @brief Gets the message
    */ 
   std::string GetCurrentMsg();
    
  private:
   std::string currMsg;
};

#endif // TRANSIT_SERVER_H_
