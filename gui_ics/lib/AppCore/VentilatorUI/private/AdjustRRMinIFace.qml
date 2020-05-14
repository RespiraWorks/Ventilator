import QtQuick 2.0
import GreenHouse 1.0
Displayable {
    id: root
    //Properties

    //Signals
    signal set_VentilatorParametersInterface_rrMinPopup(int rrMinPopup);

    signal set_VentilatorParametersInterface_rrMin(int rrMin);

    signal moveToNoPopup();

    signal call_VentilatorParametersInterface_adjustRrMin(int value);
}
