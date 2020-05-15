//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

AdjustTidalVIFace {
id: root

onSet_VentilatorParametersInterface_tidalVpopup: {
VentilatorParametersInterface.setTidalVpopup(tidalVpopup);
}

onSet_VentilatorParametersInterface_tidalV: {
VentilatorParametersInterface.setTidalV(tidalV);
}

onMoveToNoPopup: {
ViewManager.requestItem("NoPopup");
}

onCall_VentilatorParametersInterface_adjustTidalV: {
VentilatorParametersInterface.adjustTidalV(value);
}


}
