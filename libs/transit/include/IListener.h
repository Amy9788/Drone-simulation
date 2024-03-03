#ifndef I_LISTENER_H_
#define I_LISTENER_H_

#include <string>
//-------------------- IListener Interface ----------------------------

class IListener{
 public:
   /**
    * 
    * @brief To update the message for subcriber
    * @param msg std::string contains the message
    **/ 
   virtual void Update(std::string msg) = 0;
};
#endif // I_LISTENER_H_
