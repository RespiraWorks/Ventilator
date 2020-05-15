import QtQuick 2.0
import GreenHouse 1.0
Displayable {
    id: root
    //Properties

    //Signals
    signal surface_Screens_MoveTo_AlarmLogs_ScreenState();

    signal set_VentilatorParametersInterface_alertFlag(bool alertFlag);

    signal moveToNoPopup();
}
