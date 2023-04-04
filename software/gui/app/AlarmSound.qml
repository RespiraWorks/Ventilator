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
        case tAlarmPriority.HIGH: source = 'qrc:/sounds/alarm_high.wav'; break;
        case tAlarmPriority.MEDIUM: source = 'qrc:/sounds/alarm_medium.wav'; break;
        // TODO: Currently we don't have any low priority alarms, but this needs
        // a tone similar to alarm_medium.wav but with only two beeps.
        case tAlarmPriority.LOW: source = ''; break;
        case tAlarmPriority.NONE: source = ''; break;
        }

        if(priority != tAlarmPriority.NONE) {
            play()
        } else {
            stop()
        }

    }
}
