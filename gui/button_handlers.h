#ifndef BUTTON_HANDLERS_H
#define BUTTON_HANDLERS_H

#include <QDebug>
#include <QObject>
#include <string>
#include <iostream>

#define RR_DEFAULT 0
#define PEEP_DEFAULT 0
#define PIP_DEFAULT 0
#define IER_DEFAULT 1.1
#define RR_HIGH_ALARM_DEFAULT 0
#define RR_LOW_ALARM_DEFAULT 0
#define TV_HIGH_ALARM_DEFAULT 0
#define TV_LOW_ALARM_DEFAULT 0

class MyObject : public QObject{
   Q_OBJECT
public:
    explicit MyObject (QObject* parent = 0) : QObject(parent) {}

    // Respiratory Rate

    Q_INVOKABLE void rr_inc(){
        rr++;
    }

    Q_INVOKABLE void rr_dec(){
        if(rr > 0) rr--;
    }

    Q_INVOKABLE int rr_val(){
        return rr;
    }

    Q_INVOKABLE int rr_default(){
        return RR_DEFAULT;
    }

    // PIP

    Q_INVOKABLE void pip_inc(){
        pip++;
    }

    Q_INVOKABLE void pip_dec(){
        if(pip > 0) pip--;
    }

    Q_INVOKABLE int pip_val(){
        return pip;
    }

    Q_INVOKABLE int pip_default(){
        return PIP_DEFAULT;
    }

    // PEEP

    Q_INVOKABLE void peep_inc(){
        peep++;
    }

    Q_INVOKABLE void peep_dec(){
        if(peep > 0) peep--;
    }

    Q_INVOKABLE int peep_val(){
        return peep;
    }

    Q_INVOKABLE int peep_default(){
        return PEEP_DEFAULT;
    }

    // I:E ratio

    Q_INVOKABLE void ier_inc(){
        if(ier < 1.5) ier+=0.1;
    }

    Q_INVOKABLE void ier_dec(){
        if(ier > 1.1) ier-=0.1;
    }

    Q_INVOKABLE double ier_val(){
        return ier;
    }

    Q_INVOKABLE double ier_default(){
        return IER_DEFAULT;
    }

    // Alarms ///////////////

    // RR High Alarm

    Q_INVOKABLE void rrAlarmHigh_inc(){
        rrAlarmHigh++;
    }

    Q_INVOKABLE void rrAlarmHigh_dec(){
        if(rrAlarmHigh > 0) rrAlarmHigh--;
    }

    Q_INVOKABLE int rrAlarmHigh_val(){
        return rrAlarmHigh;
    }

    Q_INVOKABLE int rrAlarmHigh_default(){
        return RR_HIGH_ALARM_DEFAULT;
    }

    // RR Low Alarm

    Q_INVOKABLE void rrAlarmLow_inc(){
        rrAlarmLow++;
    }

    Q_INVOKABLE void rrAlarmLow_dec(){
        if(rrAlarmLow > 0) rrAlarmLow--;
    }

    Q_INVOKABLE int rrAlarmLow_val(){
        return rrAlarmLow;
    }

    Q_INVOKABLE int rrAlarmLow_default(){
        return RR_LOW_ALARM_DEFAULT;
    }

    // TV High Alarm

    Q_INVOKABLE void tvAlarmHigh_inc(){
        tvAlarmHigh++;
    }

    Q_INVOKABLE void tvAlarmHigh_dec(){
        if(tvAlarmHigh > 0) tvAlarmHigh--;
    }

    Q_INVOKABLE int tvAlarmHigh_val(){
        return tvAlarmHigh;
    }

    Q_INVOKABLE int tvAlarmHigh_default(){
        return TV_HIGH_ALARM_DEFAULT;
    }

    // RR Low Alarm

    Q_INVOKABLE void tvAlarmLow_inc(){
        tvAlarmLow++;
    }

    Q_INVOKABLE void tvAlarmLow_dec(){
        if(tvAlarmLow > 0) tvAlarmLow--;
    }

    Q_INVOKABLE int tvAlarmLow_val(){
        return tvAlarmLow;
    }

    Q_INVOKABLE int tvAlarmLow_default(){
        return TV_LOW_ALARM_DEFAULT;
    }

private:
    // TODO Replace constants with more realistic values
    int rr = RR_DEFAULT;
    int peep = PEEP_DEFAULT;
    int pip = PIP_DEFAULT;
    double ier = IER_DEFAULT;

    int rrAlarmHigh = RR_HIGH_ALARM_DEFAULT;
    int rrAlarmLow = RR_LOW_ALARM_DEFAULT;

    int tvAlarmHigh = TV_HIGH_ALARM_DEFAULT;
    int tvAlarmLow = TV_LOW_ALARM_DEFAULT;
};

#endif // BUTTON_HANDLERS_H
