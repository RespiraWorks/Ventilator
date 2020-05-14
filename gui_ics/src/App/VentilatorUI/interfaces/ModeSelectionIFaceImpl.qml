//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

ModeSelectionIFace {
id: root

onMoveToNoPopup: {
ViewManager.requestItem("NoPopup");
}

onCall_VentilatorParametersInterface_setPressureMode: {
VentilatorParametersInterface.setPressureMode(mode);
}


}
