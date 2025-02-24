#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H

#include "../IAlertNotifier/IAlertNotifier.h"
#include <unordered_map>
#include <string>

class AlertSystem
{
private:
    std::unordered_map<std::string, std::string> blacklistedVehicles;
    IAlertNotifier *notifier;
    double speedLimit; // Speed limit in pixels per second

public:
    AlertSystem(IAlertNotifier *notifier, double speedLimit = 50.0)
        : notifier(notifier), speedLimit(speedLimit)
    {
        // Initialize blacklisted vehicles
        blacklistedVehicles = {
            {"ABC123", "Stolen vehicle"},
            {"XYZ789", "Over-speeding history"},
            {"DEF456", "Unpaid fines"}};
    }

    void checkForBlacklistedVehicle(const std::string &licensePlate)
    {
        if (blacklistedVehicles.find(licensePlate) != blacklistedVehicles.end())
        {
            std::string reason = blacklistedVehicles[licensePlate];
            notifier->sendAlert(licensePlate, reason);
        }
    }

    void checkForOverspeedingVehicle(const std::string &licensePlate, double speed)

    {
        std::cout << "licensePlate: " << licensePlate << "\n";
        if (speed > speedLimit)
        {
            std::string reason = "Overspeeding: " + std::to_string(speed) + " pixels per second";
            notifier->sendAlert(licensePlate, reason);
        }
    }
};

#endif // ALERTSYSTEM_H