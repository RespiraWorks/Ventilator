import QtQuick 2.0
import QtMultimedia 5.0
import Respira 1.0

SoundEffect {
    id: root
    property int  priority
    loops: SoundEffect.Infinite
    onPriorityChanged:update()

    function update() {
        switch(root.priority) {
        case AlarmPriority.HIGH: source = 'qrc:/sounds/alarm_high.wav'; break;
        case AlarmPriority.MEDIUM: source = 'qrc:/sounds/alarm_medium.wav'; break;
        case AlarmPriority.LOW: source = 'qrc:/sounds/alarm_low.wav'; break;
        case AlarmPriority.NONE: source = ''; break;
        }

        if(priority != AlarmPriority.NONE) {
            play()
        } else {
            stop()
        }

    }
}
