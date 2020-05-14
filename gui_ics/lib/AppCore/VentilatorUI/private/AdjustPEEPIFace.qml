import QtQuick 2.0
import GreenHouse 1.0
Displayable {
    id: root
    //Properties

    //Signals
    signal set_VentilatorParametersInterface_peepPopup(int peepPopup);

    signal set_VentilatorParametersInterface_peep(int peep);

    signal moveToNoPopup();

    signal call_VentilatorParametersInterface_adjustPeep(int value);
}
