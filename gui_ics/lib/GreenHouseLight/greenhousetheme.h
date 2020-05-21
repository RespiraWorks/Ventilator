/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GREENHOUSETHEMEMANAGER_H
#define GREENHOUSETHEMEMANAGER_H

#include "greenhouse_global.h"
#include <QObject>
#include <QUrl>
#include <QHash>
#include <QFont>
#include <QColor>
#include <QVariant>
#include <QJsonObject>

namespace GreenHouse {
class Context;

///
/// \brief The Theme class is used to define a QML accessible theme instance
///
class GREENHOUSESHARED_EXPORT Theme : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Theme)
    Q_PROPERTY(
            QString themeBaseDir READ themeBaseDir WRITE setThemeBaseDir NOTIFY themeBaseDirChanged)
    Q_PROPERTY(bool cacheEffects READ cacheEffects CONSTANT)

public:
    enum Type { Color = 0, Font, String, Int, Asset };
    Q_ENUM(Type)

    ///
    /// \brief Theme creates a new Theme instance with the given parent
    /// \param parent
    ///
    explicit Theme(QObject *parent = nullptr);
    ~Theme() override = default;
    Theme(Theme &&) = delete;
    Theme &operator=(Theme &&) = delete;

    ///
    /// \brief themeBaseDir used to get the theme's base directory
    /// \return
    ///
    QString themeBaseDir() const;

    ///
    /// \brief parentTheme returns the name of the parent theme of this theme.
    /// \return
    ///
    QString parentTheme() const;

    ///
    /// \brief cacheGraphicalEffects returns the theme's graphical effect caching setting
    /// \return
    ///
    bool cacheEffects() const;

    ///
    /// \brief setThemeBaseDir used to set this theme's base directory
    /// \param dir the new theme base directory
    ///
    void setThemeBaseDir(const QString &dir);

    ///
    /// \brief asset used to get an asset url associated with the provided id
    /// \param id the requested asset's name/id for example "foobar.png"
    /// \return
    ///
    Q_INVOKABLE QUrl asset(const QString &id) const;

    ///
    /// \brief animationFrameAsset used to get an animation frame asset url associated with the
    /// provided id
    /// \param id the requested asset's name/id for example "foobar"
    /// \param frame the number of the requested frame
    /// \param frameIdLength the minimum leanght of the frame id generated from the provided frame
    /// number
    /// \return
    ///
    Q_INVOKABLE QUrl animationFrameAsset(const QString &id, int frame, int frameIdLength) const;

    ///
    /// \brief coloredAsset used to get an asset url associated with the provided id and the
    /// specified color
    /// \param id the requested asset's name/id for example "foobar.png"
    /// \param colorId the desired color's id for the requested asset, can be ignored if no colored
    /// match is found
    /// \return
    ///
    Q_INVOKABLE QUrl coloredAsset(const QString &id, const QString &colorId) const;

    ///
    /// \brief font used to get a font definition associated with the provided id
    /// \param id the requested font's name/id
    /// \return
    ///
    Q_INVOKABLE QFont font(const QString &id) const;

    ///
    /// \brief color used to get a color definition associated with the provided id
    /// \param id the requested color's name/id
    /// \return
    ///
    Q_INVOKABLE QColor color(const QString &id) const;

    ///
    /// \brief colorId used to get a color definition's id
    /// \param color the color definition to be identified
    /// \return
    ///
    Q_INVOKABLE QString colorId(const QColor &color) const;

    ///
    /// \brief alternativeColorIds used to get a color ids pointing to the same argb value
    /// \param id the requested color's name/id
    /// \return
    ///
    Q_INVOKABLE QStringList alternativeColorIds(const QString &id) const;

    ///
    /// \brief setParentThemeInstance used to set this theme's parent theme instance
    /// \param theme the new parent theme instance
    ///
    void setParentThemeInstance(Theme *theme);

    ///
    /// \brief init used to initialize the theme
    /// \param prefixPath a prefix value to be prepended when resolving asset urls etc.
    ///
    void init(const QString &prefixPath);

    ///
    /// \brief assetHash used to generate a unique name for an asset from the proviuded data string
    /// \param assetDataStr a string defining the asset's paramteres for example
    /// "w_100_h_100_rgb342434"
    /// \return a unique asset has id
    ///
    Q_INVOKABLE QByteArray assetHash(const QByteArray &assetDataStr) const;

signals:
    void themeBaseDirChanged();

private:
    void loadFonts();
    void loadColors();

    QString m_themeBaseDir;
    QString m_imageProviderId;
    QString m_animationImageProviderId;
    QString m_themePrefixDir;
    QString m_themePrefix;
    QString m_parentTheme;
    QList<int> m_loadedFontFiles;
    QHash<QString, QFont> m_fontMap;
    QHash<QString, QColor> m_colors;
    bool m_cacheEffects = false;

    Theme *m_parentThemeInstance = nullptr;
};

///
/// \brief The ThemeManager class is used to access and manage a Context's currently active theme
/// from QML
///
class GREENHOUSESHARED_EXPORT ThemeManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ThemeManager)
    Q_PROPERTY(GreenHouse::Theme *theme READ theme NOTIFY themeChanged)
    Q_PROPERTY(QString themeName READ themeName NOTIFY themeChanged)

public:
    ~ThemeManager() override;
    ThemeManager(ThemeManager &&) = delete;
    ThemeManager &operator=(ThemeManager &&) = delete;
    ///
    /// \brief theme used to get the currently active theme's instance
    /// \return
    ///
    GreenHouse::Theme *theme() const;

    ///
    /// \brief themeName used to get the currently active theme's name
    /// \return
    ///
    QString themeName() const;

signals:
    void themeChanged();

private:
    explicit ThemeManager(Context *context = nullptr, QObject *parent = nullptr);
    void activateTheme(const QString &name);
    void createThemeParent(Theme *theme);

    Context *m_context;
    Theme *m_themeInstance = nullptr;
    QList<Theme *> m_parentThemeInstances;

    friend class ThemeManagerActivator;
};

///
/// \brief The ThemeManagerActivator class is used to manage and a Context's theme from C++
///
class GREENHOUSESHARED_EXPORT ThemeManagerActivator
{
    Q_DISABLE_COPY(ThemeManagerActivator)

public:
    ThemeManagerActivator() = delete;
    ~ThemeManagerActivator() = default;
    ThemeManagerActivator(ThemeManagerActivator &&) = delete;
    ThemeManagerActivator &operator=(ThemeManagerActivator &&) = delete;
    ///
    /// \brief contextThemeManagerInstance used to get a ThemeManagerInstance assigned to the
    /// provided context
    /// \param context
    /// \return
    ///
    static QObject *contextThemeManagerInstance(Context *context);

    ///
    /// \brief loadThemeConfig used to load a theme configuration
    /// \param configPath the configuration file's path
    /// \param context the context instance this configuration will be assigned to
    /// \param themeBasePath an optional theme base path
    ///
    static void loadThemeConfig(const QString &configPath, Context *context,
                                const QString &themeBasePath = QString());

    ///
    /// \brief registerThemeCollection used to register a context's theme collection
    /// \param themes a mapping of theme names to theme urls
    /// \param context the context instance this configuration will be assigned to
    ///
    static void registerThemeCollection(const QMap<QString, QUrl> &themes, Context *context);

    ///
    /// \brief activateTheme used to set a context's currently active theme based on the provided
    /// name
    /// \param theme the requested theme's name
    /// \param context
    ///
    static void activateTheme(const QString &theme, Context *context);

    ///
    /// \brief activeTheme returns the name of the parent theme of this theme.
    /// \param context the queried context
    /// \return
    ///
    static QString activeTheme(const Context *context);

    ///
    /// \brief availableThemes used to get all of the available themes for a given context
    /// \param context the queried context
    /// \return
    ///
    static QStringList availableThemes(const Context *context);
};
}

#endif // GREENHOUSETHEMEMANAGER_H
