//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

AdjustRRMinIFace {
id: root

onSet_VentilatorParametersInterface_rrMinPopup: {
VentilatorParametersInterface.setRrMinPopup(rrMinPopup);
}

onSet_VentilatorParametersInterface_rrMin: {
VentilatorParametersInterface.setRrMin(rrMin);
}

onMoveToNoPopup: {
ViewManager.requestItem("NoPopup");
}

onCall_VentilatorParametersInterface_adjustRrMin: {
VentilatorParametersInterface.adjustRrMin(value);
}


}
