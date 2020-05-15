import QtQuick 2.0
import GreenHouse 1.0
Displayable {
    id: root
    //Properties

    //Signals
    signal set_VentilatorParametersInterface_tidalVpopup(int tidalVpopup);

    signal set_VentilatorParametersInterface_tidalV(int tidalV);

    signal moveToNoPopup();

    signal call_VentilatorParametersInterface_adjustTidalV(int value);
}
