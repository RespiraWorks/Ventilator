#ifndef BUTTON_HANDLERS_H
#define BUTTON_HANDLERS_H

#include <QDebug>
#include <QObject>
#include <string>
#include <iostream>

class MyObject : public QObject{
   Q_OBJECT
public:
    explicit MyObject (QObject* parent = 0) : QObject(parent) {}

    // Respiratory Rate

    Q_INVOKABLE void rr_increase(){
        rr++;
    }

    Q_INVOKABLE void rr_decrease(){
        if(rr > 0) rr--;
    }

    Q_INVOKABLE int rr_value(){
        return rr;
    }

    // PIP

    Q_INVOKABLE void pip_increase(){
        pip++;
    }

    Q_INVOKABLE void pip_decrease(){
        if(pip > 0) pip--;
    }

    Q_INVOKABLE int pip_value(){
        return pip;
    }

    // PEEP

    Q_INVOKABLE void peep_increase(){
        peep++;
    }

    Q_INVOKABLE void peep_decrease(){
        if(peep > 0) peep--;
    }

    Q_INVOKABLE int peep_value(){
        return peep;
    }

    // I:E ratio

    Q_INVOKABLE void ier_increase(){
        if(ier < 1.5) ier+=0.1;
    }

    Q_INVOKABLE void ier_decrease(){
        if(ier > 1.1) ier-=0.1;
    }

    Q_INVOKABLE double ier_value(){
        return ier;
    }


private:
    int rr = 0;
    int peep = 0;
    int pip = 0;
    double ier = 1.1;
};

#endif // BUTTON_HANDLERS_H
