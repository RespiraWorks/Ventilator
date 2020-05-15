//NOTE: Please remove this line to prevent this file being overwritten by the flow generation process
import GreenHouse 1.0
import "qrc:/VentilatorUI/private/"

MainView_MainViewPanelIFace {
id: root

onSurface_Popups_MoveTo_AdjustTidalV: {
ViewManager.requestItem("AdjustTidalV");
}

onSurface_Popups_MoveTo_AdjustPEEP: {
ViewManager.requestItem("AdjustPEEP");
}

onSurface_Popups_MoveTo_AdjustRRMin: {
ViewManager.requestItem("AdjustRRMin");
}

onSet_VentilatorParametersInterface_tidalVpopup: {
VentilatorParametersInterface.setTidalVpopup(tidalVpopup);
}

onSet_VentilatorParametersInterface_peepPopup: {
VentilatorParametersInterface.setPeepPopup(peepPopup);
}

onSet_VentilatorParametersInterface_rrMinPopup: {
VentilatorParametersInterface.setRrMinPopup(rrMinPopup);
}


}
