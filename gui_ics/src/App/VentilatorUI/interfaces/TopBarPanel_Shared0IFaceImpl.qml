//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

TopBarPanel_Shared0IFace {
id: root

onSurface_Popups_MoveTo_ModeSelection: {
ViewManager.requestItem("ModeSelection");
}

onSet_VentilatorParametersInterface_showSettings: {
VentilatorParametersInterface.setShowSettings(showSettings);
}

onSet_VentilatorParametersInterface_preUseTestPassed: {
VentilatorParametersInterface.setPreUseTestPassed(preUseTestPassed);
}

onSurface_Popups_MoveTo_ActiveAlertsFrame: {
ViewManager.requestItem("ActiveAlertsFrame");
}


}
