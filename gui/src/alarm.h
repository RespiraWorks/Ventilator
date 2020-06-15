#ifndef ALARM_H_
#define ALARM_H_

#include <QObject>

class AlarmPriority {
  Q_GADGET

public:
  enum class Enum {
    NONE,
    LOW,
    MEDIUM,
    HIGH,
  };

  Q_ENUM(Enum)
};

#endif // ALARM_H_
