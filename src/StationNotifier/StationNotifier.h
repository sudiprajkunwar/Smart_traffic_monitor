#ifndef STATIONNOTIFIER_H
#define STATIONNOTIFIER_H

#include <iostream>
#include "../IAlertNotifier/IAlertNotifier.h"

class StationNotifier : public IAlertNotifier
{
public:
    void sendAlert(const std::string &licensePlate, const std::string &reason) override
    {
        std::string alertMessage =
            "ALERT: Blacklisted vehicle detected!\n"
            "License Plate: " +
            licensePlate + "\n"
                           "Reason: " +
            reason + "\n"
                     "Notifying Station: Central Traffic Control\n"
                     "Action: Dispatching patrol unit to intercept the vehicle.\n"
                     "----------------------------------------";

        // Log the alert using the Logger singleton
        Logger::getInstance().log(LogLevel::ERROR, alertMessage);
    }
};

#endif // STATIONNOTIFIER_H