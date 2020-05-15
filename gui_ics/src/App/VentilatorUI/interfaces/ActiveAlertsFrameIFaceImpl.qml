//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

ActiveAlertsFrameIFace {
id: root

onSurface_Screens_MoveTo_AlarmLogs_ScreenState: {
ViewManager.requestItem("AlarmLogs_ScreenState");
}

onSet_VentilatorParametersInterface_alertFlag: {
VentilatorParametersInterface.setAlertFlag(alertFlag);
}

onMoveToNoPopup: {
ViewManager.requestItem("NoPopup");
}


}
