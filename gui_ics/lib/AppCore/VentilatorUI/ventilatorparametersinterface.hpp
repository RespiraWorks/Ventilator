#pragma once

#include <QObject>
#include <greenhousecontext.h>
#include "../appcore_global.h"

namespace VentilatorUI {
///
/// \brief The VentilatorParametersInterface class is
///
class APPCORE_EXPORT VentilatorParametersInterface : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(VentilatorParametersInterface)

    Q_PROPERTY(int tidalV READ tidalV NOTIFY tidalVChanged)
    int m_tidalV = { 350 };
    Q_PROPERTY(int tidalVpopup READ tidalVpopup NOTIFY tidalVpopupChanged)
    int m_tidalVpopup = { 350 };
    Q_PROPERTY(int peep READ peep NOTIFY peepChanged)
    int m_peep = { 5 };
    Q_PROPERTY(int peepPopup READ peepPopup NOTIFY peepPopupChanged)
    int m_peepPopup = { 5 };
    Q_PROPERTY(int rrMin READ rrMin NOTIFY rrMinChanged)
    int m_rrMin = { 8 };
    Q_PROPERTY(int rrMinPopup READ rrMinPopup NOTIFY rrMinPopupChanged)
    int m_rrMinPopup = { 8 };
    Q_PROPERTY(bool alertFlag READ alertFlag NOTIFY alertFlagChanged)
    bool m_alertFlag = { false };
    Q_PROPERTY(bool preUseTestPassed READ preUseTestPassed NOTIFY preUseTestPassedChanged)
    bool m_preUseTestPassed = { false };
    Q_PROPERTY(bool showSettings READ showSettings NOTIFY showSettingsChanged)
    bool m_showSettings = { false };
    Q_PROPERTY(int acpMode READ acpMode NOTIFY acpModeChanged)
    int m_acpMode = { 0 };
    Q_PROPERTY(QString acpModeName READ acpModeName NOTIFY acpModeNameChanged)
    QString m_acpModeName = { QStringLiteral("AC/P") };
    Q_PROPERTY(int currentRate READ currentRate NOTIFY currentRateChanged)
    int m_currentRate = { 10 };
    Q_PROPERTY(int currentPeep READ currentPeep NOTIFY currentPeepChanged)
    int m_currentPeep = { 10 };
    Q_PROPERTY(int currentTidalV READ currentTidalV NOTIFY currentTidalVChanged)
    int m_currentTidalV = { 10 };
    Q_PROPERTY(int currentAdditionalValue READ currentAdditionalValue NOTIFY
                       currentAdditionalValueChanged)
    int m_currentAdditionalValue = { 10 };
    Q_PROPERTY(int powerSourceType READ powerSourceType NOTIFY powerSourceTypeChanged)
    int m_powerSourceType = { 0 };
    Q_PROPERTY(QString powerSourceAsset READ powerSourceAsset NOTIFY powerSourceAssetChanged)
    QString m_powerSourceAsset = { QStringLiteral("/122_2237.png") };
    Q_PROPERTY(QString batteryPercentageText READ batteryPercentageText NOTIFY
                       batteryPercentageTextChanged)
    QString m_batteryPercentageText = { QStringLiteral("100%") };
    Q_PROPERTY(QString batteryPercentageAsset READ batteryPercentageAsset NOTIFY
                       batteryPercentageAssetChanged)
    QString m_batteryPercentageAsset = { QStringLiteral("/254_142.png") };
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    int m_batteryLevel = { 100 };
    Q_PROPERTY(QString alertText READ alertText NOTIFY alertTextChanged)
    QString m_alertText = { QString() };
    Q_PROPERTY(int tidalVLow READ tidalVLow NOTIFY tidalVLowChanged)
    int m_tidalVLow = { 350 };
    Q_PROPERTY(int tidalVHigh READ tidalVHigh NOTIFY tidalVHighChanged)
    int m_tidalVHigh = { 750 };
    Q_PROPERTY(int peepLow READ peepLow NOTIFY peepLowChanged)
    int m_peepLow = { 5 };
    Q_PROPERTY(int peepHigh READ peepHigh NOTIFY peepHighChanged)
    int m_peepHigh = { 22 };
    Q_PROPERTY(int tidalVStep READ tidalVStep NOTIFY tidalVStepChanged)
    int m_tidalVStep = { 1 };
    Q_PROPERTY(int peepStep READ peepStep NOTIFY peepStepChanged)
    int m_peepStep = { 1 };
    Q_PROPERTY(int rrMinLow READ rrMinLow NOTIFY rrMinLowChanged)
    int m_rrMinLow = { 8 };
    Q_PROPERTY(int rrMinHigh READ rrMinHigh NOTIFY rrMinHighChanged)
    int m_rrMinHigh = { 25 };
    Q_PROPERTY(int rrMinStep READ rrMinStep NOTIFY rrMinStepChanged)
    int m_rrMinStep = { 1 };

public:
    explicit VentilatorParametersInterface(QObject *parent = nullptr);
    ~VentilatorParametersInterface() override = default;
    VentilatorParametersInterface(VentilatorParametersInterface &&) = delete;
    VentilatorParametersInterface &operator=(VentilatorParametersInterface &&) = delete;

public slots:
    ///
    /// \brief startPreUseTest
    ///
    virtual void startPreUseTest();

    ///
    /// \brief setPressureMode
    /// \param mode
    ///
    virtual void setPressureMode(int mode);

    ///
    /// \brief adjustTidalV
    /// \param value
    ///
    virtual void adjustTidalV(int value);

    ///
    /// \brief adjustPeep
    /// \param value
    ///
    virtual void adjustPeep(int value);

    ///
    /// \brief adjustRrMin
    /// \param value
    ///
    virtual void adjustRrMin(int value);

    ///
    /// \brief tidalV
    /// \return
    ///
    virtual int tidalV() const { return m_tidalV; }

    ///
    /// \brief setTidalV used to set the tidalV property
    ///
    virtual void setTidalV(int tidalV);

    ///
    /// \brief tidalVpopup
    /// \return
    ///
    virtual int tidalVpopup() const { return m_tidalVpopup; }

    ///
    /// \brief setTidalVpopup used to set the tidalVpopup property
    ///
    virtual void setTidalVpopup(int tidalVpopup);

    ///
    /// \brief peep
    /// \return
    ///
    virtual int peep() const { return m_peep; }

    ///
    /// \brief setPeep used to set the peep property
    ///
    virtual void setPeep(int peep);

    ///
    /// \brief peepPopup
    /// \return
    ///
    virtual int peepPopup() const { return m_peepPopup; }

    ///
    /// \brief setPeepPopup used to set the peepPopup property
    ///
    virtual void setPeepPopup(int peepPopup);

    ///
    /// \brief rrMin
    /// \return
    ///
    virtual int rrMin() const { return m_rrMin; }

    ///
    /// \brief setRrMin used to set the rrMin property
    ///
    virtual void setRrMin(int rrMin);

    ///
    /// \brief rrMinPopup
    /// \return
    ///
    virtual int rrMinPopup() const { return m_rrMinPopup; }

    ///
    /// \brief setRrMinPopup used to set the rrMinPopup property
    ///
    virtual void setRrMinPopup(int rrMinPopup);

    ///
    /// \brief alertFlag
    /// \return
    ///
    virtual bool alertFlag() const { return m_alertFlag; }

    ///
    /// \brief setAlertFlag used to set the alertFlag property
    ///
    virtual void setAlertFlag(bool alertFlag);

    ///
    /// \brief preUseTestPassed
    /// \return
    ///
    virtual bool preUseTestPassed() const { return m_preUseTestPassed; }

    ///
    /// \brief setPreUseTestPassed used to set the preUseTestPassed property
    ///
    virtual void setPreUseTestPassed(bool preUseTestPassed);

    ///
    /// \brief showSettings
    /// \return
    ///
    virtual bool showSettings() const { return m_showSettings; }

    ///
    /// \brief setShowSettings used to set the showSettings property
    ///
    virtual void setShowSettings(bool showSettings);

    ///
    /// \brief acpMode
    /// \return
    ///
    virtual int acpMode() const { return m_acpMode; }

    ///
    /// \brief setAcpMode used to set the acpMode property
    ///
    virtual void setAcpMode(int acpMode);

    ///
    /// \brief acpModeName
    /// \return
    ///
    virtual QString acpModeName() const { return m_acpModeName; }

    ///
    /// \brief setAcpModeName used to set the acpModeName property
    ///
    virtual void setAcpModeName(const QString &acpModeName);

    ///
    /// \brief currentRate
    /// \return
    ///
    virtual int currentRate() const { return m_currentRate; }

    ///
    /// \brief setCurrentRate used to set the currentRate property
    ///
    virtual void setCurrentRate(int currentRate);

    ///
    /// \brief currentPeep
    /// \return
    ///
    virtual int currentPeep() const { return m_currentPeep; }

    ///
    /// \brief setCurrentPeep used to set the currentPeep property
    ///
    virtual void setCurrentPeep(int currentPeep);

    ///
    /// \brief currentTidalV
    /// \return
    ///
    virtual int currentTidalV() const { return m_currentTidalV; }

    ///
    /// \brief setCurrentTidalV used to set the currentTidalV property
    ///
    virtual void setCurrentTidalV(int currentTidalV);

    ///
    /// \brief currentAdditionalValue
    /// \return
    ///
    virtual int currentAdditionalValue() const { return m_currentAdditionalValue; }

    ///
    /// \brief setCurrentAdditionalValue used to set the currentAdditionalValue property
    ///
    virtual void setCurrentAdditionalValue(int currentAdditionalValue);

    ///
    /// \brief powerSourceType
    /// \return
    ///
    virtual int powerSourceType() const { return m_powerSourceType; }

    ///
    /// \brief setPowerSourceType used to set the powerSourceType property
    ///
    virtual void setPowerSourceType(int powerSourceType);

    ///
    /// \brief powerSourceAsset
    /// \return
    ///
    virtual QString powerSourceAsset() const { return m_powerSourceAsset; }

    ///
    /// \brief setPowerSourceAsset used to set the powerSourceAsset property
    ///
    virtual void setPowerSourceAsset(const QString &powerSourceAsset);

    ///
    /// \brief batteryPercentageText
    /// \return
    ///
    virtual QString batteryPercentageText() const { return m_batteryPercentageText; }

    ///
    /// \brief setBatteryPercentageText used to set the batteryPercentageText property
    ///
    virtual void setBatteryPercentageText(const QString &batteryPercentageText);

    ///
    /// \brief batteryPercentageAsset
    /// \return
    ///
    virtual QString batteryPercentageAsset() const { return m_batteryPercentageAsset; }

    ///
    /// \brief setBatteryPercentageAsset used to set the batteryPercentageAsset property
    ///
    virtual void setBatteryPercentageAsset(const QString &batteryPercentageAsset);

    ///
    /// \brief batteryLevel
    /// \return
    ///
    virtual int batteryLevel() const { return m_batteryLevel; }

    ///
    /// \brief setBatteryLevel used to set the batteryLevel property
    ///
    virtual void setBatteryLevel(int batteryLevel);

    ///
    /// \brief alertText
    /// \return
    ///
    virtual QString alertText() const { return m_alertText; }

    ///
    /// \brief setAlertText used to set the alertText property
    ///
    virtual void setAlertText(const QString &alertText);

    ///
    /// \brief tidalVLow
    /// \return
    ///
    virtual int tidalVLow() const { return m_tidalVLow; }

    ///
    /// \brief setTidalVLow used to set the tidalVLow property
    ///
    virtual void setTidalVLow(int tidalVLow);

    ///
    /// \brief tidalVHigh
    /// \return
    ///
    virtual int tidalVHigh() const { return m_tidalVHigh; }

    ///
    /// \brief setTidalVHigh used to set the tidalVHigh property
    ///
    virtual void setTidalVHigh(int tidalVHigh);

    ///
    /// \brief peepLow
    /// \return
    ///
    virtual int peepLow() const { return m_peepLow; }

    ///
    /// \brief setPeepLow used to set the peepLow property
    ///
    virtual void setPeepLow(int peepLow);

    ///
    /// \brief peepHigh
    /// \return
    ///
    virtual int peepHigh() const { return m_peepHigh; }

    ///
    /// \brief setPeepHigh used to set the peepHigh property
    ///
    virtual void setPeepHigh(int peepHigh);

    ///
    /// \brief tidalVStep
    /// \return
    ///
    virtual int tidalVStep() const { return m_tidalVStep; }

    ///
    /// \brief setTidalVStep used to set the tidalVStep property
    ///
    virtual void setTidalVStep(int tidalVStep);

    ///
    /// \brief peepStep
    /// \return
    ///
    virtual int peepStep() const { return m_peepStep; }

    ///
    /// \brief setPeepStep used to set the peepStep property
    ///
    virtual void setPeepStep(int peepStep);

    ///
    /// \brief rrMinLow
    /// \return
    ///
    virtual int rrMinLow() const { return m_rrMinLow; }

    ///
    /// \brief setRrMinLow used to set the rrMinLow property
    ///
    virtual void setRrMinLow(int rrMinLow);

    ///
    /// \brief rrMinHigh
    /// \return
    ///
    virtual int rrMinHigh() const { return m_rrMinHigh; }

    ///
    /// \brief setRrMinHigh used to set the rrMinHigh property
    ///
    virtual void setRrMinHigh(int rrMinHigh);

    ///
    /// \brief rrMinStep
    /// \return
    ///
    virtual int rrMinStep() const { return m_rrMinStep; }

    ///
    /// \brief setRrMinStep used to set the rrMinStep property
    ///
    virtual void setRrMinStep(int rrMinStep);

signals:
    ///
    /// \brief tidalVChanged emitted when the tidalV property changes
    ///
    void tidalVChanged();

    ///
    /// \brief tidalVpopupChanged emitted when the tidalVpopup property changes
    ///
    void tidalVpopupChanged();

    ///
    /// \brief peepChanged emitted when the peep property changes
    ///
    void peepChanged();

    ///
    /// \brief peepPopupChanged emitted when the peepPopup property changes
    ///
    void peepPopupChanged();

    ///
    /// \brief rrMinChanged emitted when the rrMin property changes
    ///
    void rrMinChanged();

    ///
    /// \brief rrMinPopupChanged emitted when the rrMinPopup property changes
    ///
    void rrMinPopupChanged();

    ///
    /// \brief alertFlagChanged emitted when the alertFlag property changes
    ///
    void alertFlagChanged();

    ///
    /// \brief preUseTestPassedChanged emitted when the preUseTestPassed property changes
    ///
    void preUseTestPassedChanged();

    ///
    /// \brief showSettingsChanged emitted when the showSettings property changes
    ///
    void showSettingsChanged();

    ///
    /// \brief acpModeChanged emitted when the acpMode property changes
    ///
    void acpModeChanged();

    ///
    /// \brief acpModeNameChanged emitted when the acpModeName property changes
    ///
    void acpModeNameChanged();

    ///
    /// \brief currentRateChanged emitted when the currentRate property changes
    ///
    void currentRateChanged();

    ///
    /// \brief currentPeepChanged emitted when the currentPeep property changes
    ///
    void currentPeepChanged();

    ///
    /// \brief currentTidalVChanged emitted when the currentTidalV property changes
    ///
    void currentTidalVChanged();

    ///
    /// \brief currentAdditionalValueChanged emitted when the currentAdditionalValue property
    /// changes
    ///
    void currentAdditionalValueChanged();

    ///
    /// \brief powerSourceTypeChanged emitted when the powerSourceType property changes
    ///
    void powerSourceTypeChanged();

    ///
    /// \brief powerSourceAssetChanged emitted when the powerSourceAsset property changes
    ///
    void powerSourceAssetChanged();

    ///
    /// \brief batteryPercentageTextChanged emitted when the batteryPercentageText property changes
    ///
    void batteryPercentageTextChanged();

    ///
    /// \brief batteryPercentageAssetChanged emitted when the batteryPercentageAsset property
    /// changes
    ///
    void batteryPercentageAssetChanged();

    ///
    /// \brief batteryLevelChanged emitted when the batteryLevel property changes
    ///
    void batteryLevelChanged();

    ///
    /// \brief alertTextChanged emitted when the alertText property changes
    ///
    void alertTextChanged();

    ///
    /// \brief tidalVLowChanged emitted when the tidalVLow property changes
    ///
    void tidalVLowChanged();

    ///
    /// \brief tidalVHighChanged emitted when the tidalVHigh property changes
    ///
    void tidalVHighChanged();

    ///
    /// \brief peepLowChanged emitted when the peepLow property changes
    ///
    void peepLowChanged();

    ///
    /// \brief peepHighChanged emitted when the peepHigh property changes
    ///
    void peepHighChanged();

    ///
    /// \brief tidalVStepChanged emitted when the tidalVStep property changes
    ///
    void tidalVStepChanged();

    ///
    /// \brief peepStepChanged emitted when the peepStep property changes
    ///
    void peepStepChanged();

    ///
    /// \brief rrMinLowChanged emitted when the rrMinLow property changes
    ///
    void rrMinLowChanged();

    ///
    /// \brief rrMinHighChanged emitted when the rrMinHigh property changes
    ///
    void rrMinHighChanged();

    ///
    /// \brief rrMinStepChanged emitted when the rrMinStep property changes
    ///
    void rrMinStepChanged();
};
}
