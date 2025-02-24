#ifndef STATIONNOTIFIER_H
#define STATIONNOTIFIER_H

#include <iostream>
#include "../IAlertNotifier/IAlertNotifier.h"

class StationNotifier : public IAlertNotifier
{
public:
    void sendAlert(const std::string &licensePlate, const std::string &reason) override
    {
        std::cout << "ALERT: Blacklisted vehicle detected!" << std::endl;
        std::cout << "License Plate: " << licensePlate << std::endl;
        std::cout << "Reason: " << reason << std::endl;
        std::cout << "Notifying Station: Central Traffic Control" << std::endl;
        std::cout << "Action: Dispatching patrol unit to intercept the vehicle." << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
};

#endif // STATIONNOTIFIER_H