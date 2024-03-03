#include "BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity* entity){
    this->entity = entity;
    this-> toCharger = false;
    this->notify = false;
    this->lowPercentNotify = false;
    this->percentNotify = false;
    this->batteryPercent = 100;
    this->lostPercentCounter = 0;
    this->maxPercent = 100;
    this-> nearestRecharger = nullptr;
    this->toRechargerStrategy = nullptr;
    this->observer = new IEntityObserver();

}
BatteryDecorator::~BatteryDecorator() {
    //deletes dynamically allocated variables
    delete entity;
    delete nearestRecharger;
    delete toRechargerStrategy;
    delete observer;
}
