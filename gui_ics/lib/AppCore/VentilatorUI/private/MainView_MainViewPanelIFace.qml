import QtQuick 2.0
import GreenHouse 1.0
Displayable {
    id: root
    //Properties

    //Signals
    signal surface_Popups_MoveTo_AdjustTidalV();

    signal surface_Popups_MoveTo_AdjustPEEP();

    signal surface_Popups_MoveTo_AdjustRRMin();

    signal set_VentilatorParametersInterface_tidalVpopup(int tidalVpopup);

    signal set_VentilatorParametersInterface_peepPopup(int peepPopup);

    signal set_VentilatorParametersInterface_rrMinPopup(int rrMinPopup);
}
