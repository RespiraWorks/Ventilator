import QtQuick 2.0
import GreenHouse 1.0
Displayable {
    id: root
    //Properties

    //Signals
    signal surface_Popups_MoveTo_ModeSelection();

    signal set_VentilatorParametersInterface_showSettings(bool showSettings);

    signal set_VentilatorParametersInterface_preUseTestPassed(bool preUseTestPassed);

    signal surface_Popups_MoveTo_ActiveAlertsFrame();
}
