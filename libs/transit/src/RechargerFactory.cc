#include "RechargerFactory.h"
#include "Recharger.h"

IEntity* RechargerFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("recharger") == 0) {
    std::cout << "Recharger Created" << std::endl;
    return new Recharger(entity);
  }
  return nullptr;
}
