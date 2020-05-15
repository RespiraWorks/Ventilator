//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

AdjustPEEPIFace {
id: root

onSet_VentilatorParametersInterface_peepPopup: {
VentilatorParametersInterface.setPeepPopup(peepPopup);
}

onSet_VentilatorParametersInterface_peep: {
VentilatorParametersInterface.setPeep(peep);
}

onMoveToNoPopup: {
ViewManager.requestItem("NoPopup");
}

onCall_VentilatorParametersInterface_adjustPeep: {
VentilatorParametersInterface.adjustPeep(value);
}


}
