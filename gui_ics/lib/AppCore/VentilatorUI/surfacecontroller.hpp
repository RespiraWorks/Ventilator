#pragma once

#include <QObject>
#include <greenhousecontext.h>
#include "../appcore_global.h"

namespace VentilatorUI {
///
/// \brief The SurfaceController class is
///
class APPCORE_EXPORT SurfaceController : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SurfaceController)

public:
    explicit SurfaceController(QObject *parent = nullptr);
    ~SurfaceController() override = default;
    SurfaceController(SurfaceController &&) = delete;
    SurfaceController &operator=(SurfaceController &&) = delete;

public slots:

signals:
    ///
    /// \brief showPopups
    ///
    void showPopups();

    ///
    /// \brief hidePopups
    ///
    void hidePopups();

    ///
    /// \brief showMenus
    ///
    void showMenus();

    ///
    /// \brief hideMenus
    ///
    void hideMenus();

    ///
    /// \brief surfaceMenusBack
    ///
    void surfaceMenusBack();

    ///
    /// \brief surfacePopupsMoveToAdjustTidalV
    ///
    void surfacePopupsMoveToAdjustTidalV();

    ///
    /// \brief surfacePopupsMoveToAdjustPEEP
    ///
    void surfacePopupsMoveToAdjustPEEP();

    ///
    /// \brief surfacePopupsBack
    ///
    void surfacePopupsBack();

    ///
    /// \brief showBackground
    ///
    void showBackground();

    ///
    /// \brief hideBackground
    ///
    void hideBackground();

    ///
    /// \brief showMainViewPanel
    ///
    void showMainViewPanel();

    ///
    /// \brief hideMainViewPanel
    ///
    void hideMainViewPanel();

    ///
    /// \brief showTopBarPanel
    ///
    void showTopBarPanel();

    ///
    /// \brief hideTopBarPanel
    ///
    void hideTopBarPanel();

    ///
    /// \brief surfaceBackgroundMoveToBackground_Shared0
    ///
    void surfaceBackgroundMoveToBackground_Shared0();

    ///
    /// \brief surfaceMainViewPanelMoveToAlarmLogs_MainViewPanel
    ///
    void surfaceMainViewPanelMoveToAlarmLogs_MainViewPanel();

    ///
    /// \brief surfaceTopBarPanelMoveToTopBarPanel_Shared0
    ///
    void surfaceTopBarPanelMoveToTopBarPanel_Shared0();

    ///
    /// \brief surfaceMainViewPanelMoveToMainView_MainViewPanel
    ///
    void surfaceMainViewPanelMoveToMainView_MainViewPanel();

    ///
    /// \brief surfaceMainViewPanelMoveToPreUseTest_MainViewPanel
    ///
    void surfaceMainViewPanelMoveToPreUseTest_MainViewPanel();

    ///
    /// \brief surfacePopupsMoveToAdjustRRMin
    ///
    void surfacePopupsMoveToAdjustRRMin();

    ///
    /// \brief surfacePopupsMoveToModeSelection
    ///
    void surfacePopupsMoveToModeSelection();

    ///
    /// \brief surfaceScreensBack
    ///
    void surfaceScreensBack();

    ///
    /// \brief surfacePopupsMoveToActiveAlertsFrame
    ///
    void surfacePopupsMoveToActiveAlertsFrame();

    ///
    /// \brief surfaceScreensMoveToAlarmLogs_ScreenState
    ///
    void surfaceScreensMoveToAlarmLogs_ScreenState();

    ///
    /// \brief showBottom_Navigation_Bar_Panel
    ///
    void showBottom_Navigation_Bar_Panel();

    ///
    /// \brief hideBottom_Navigation_Bar_Panel
    ///
    void hideBottom_Navigation_Bar_Panel();

    ///
    /// \brief showCenter_Content_Panel
    ///
    void showCenter_Content_Panel();

    ///
    /// \brief hideCenter_Content_Panel
    ///
    void hideCenter_Content_Panel();

    ///
    /// \brief showDark_Backgrounds_Main
    ///
    void showDark_Backgrounds_Main();

    ///
    /// \brief hideDark_Backgrounds_Main
    ///
    void hideDark_Backgrounds_Main();

    ///
    /// \brief surfaceBottom_Navigation_Bar_PanelMoveToAlarms_Bottom_Navigation_Bar_Panel
    ///
    void surfaceBottom_Navigation_Bar_PanelMoveToAlarms_Bottom_Navigation_Bar_Panel();

    ///
    /// \brief surfaceCenter_Content_PanelMoveToAlarms_Center_Content_Panel
    ///
    void surfaceCenter_Content_PanelMoveToAlarms_Center_Content_Panel();

    ///
    /// \brief surfaceDark_Backgrounds_MainMoveToAlarms_Dark_Backgrounds_Main
    ///
    void surfaceDark_Backgrounds_MainMoveToAlarms_Dark_Backgrounds_Main();

    ///
    /// \brief surfaceMainViewPanelMoveToSettingPlaceholder_MainViewPanel
    ///
    void surfaceMainViewPanelMoveToSettingPlaceholder_MainViewPanel();

    ///
    /// \brief surfacePopupsMoveToAdjustIERatio
    ///
    void surfacePopupsMoveToAdjustIERatio();

    ///
    /// \brief surfaceScreensMoveToSettingPlaceholder_ScreenState
    ///
    void surfaceScreensMoveToSettingPlaceholder_ScreenState();
};
}
