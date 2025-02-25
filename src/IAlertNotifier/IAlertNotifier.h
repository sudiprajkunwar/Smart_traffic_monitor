#ifndef IALERTNOTIFIER_H
#define IALERTNOTIFIER_H

#include <string>

class IAlertNotifier
{
public:
    virtual void sendAlert(const std::string &licensePlate, const std::string &reason) = 0;
    virtual ~IAlertNotifier() = default;
};

#endif // IALERTNOTIFIER_H