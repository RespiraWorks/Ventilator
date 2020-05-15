/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "greenhousetheme.h"
#include "greenhousecontext.h"
#include "greenhouse_log.h"
#include "greenhousetheme.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QRegularExpression>

namespace GreenHouse {

namespace {
QHash<const Context *, ThemeManager *> contextThemeManagers;
QHash<const Context *, QMap<QString, QUrl>> contextThemeCollections;
QHash<const Context *, QString> contextTheme;
QHash<const Context *, QString> contextThemeBasePath;

const QString THEME_OBJECT_PATH_TAG = QStringLiteral("themeObjectPath");
const QString THEME_OBJECT_NAME_TAG = QStringLiteral("name");
const QString THEMES_TAG = QStringLiteral("themes");
const QString DEFAULT_THEME_TAG = QStringLiteral("defaultTheme");
const QString PARENT_THEME_TAG = QStringLiteral("parentTheme");
const QString IMAGE_PROVIDER_TAG = QStringLiteral("imageProviderId");
const QString ANIMATION_IMAGE_PROVIDER_TAG = QStringLiteral("animationImageProviderId");
const QString CACHE_EFFECTS_TAG = QStringLiteral("cacheEffects");

const QString FONT_NAME_TAG = QStringLiteral("name");
const QString FONT_FAMILY_TAG = QStringLiteral("family");
const QString FONT_STYLE_NAME_TAG = QStringLiteral("styleName");
const QString FONT_PIXEL_SIZE_TAG = QStringLiteral("pixelSize");
const QString FONT_WEIGHT_TAG = QStringLiteral("weight");
const QString FONT_CAPITALIZATION_TAG = QStringLiteral("capitalization");
const QString FONT_ITALIC_TAG = QStringLiteral("italic");
const QString FONT_BOLD_TAG = QStringLiteral("bold");
const QString FONT_UNDERLINE_TAG = QStringLiteral("underline");
const QString FONT_STRIKEOUT_TAG = QStringLiteral("strikeOut");

const QString COLOR_NAME_TAG = QStringLiteral("name");
const QString COLOR_R_TAG = QStringLiteral("colorR");
const QString COLOR_G_TAG = QStringLiteral("colorG");
const QString COLOR_B_TAG = QStringLiteral("colorB");
const QString COLOR_A_TAG = QStringLiteral("colorA");

const QString TYPE_HINT_TAG = QStringLiteral("_typehits_");
const QRegularExpression ASSET_URL_PATTER = QRegularExpression(QStringLiteral("://"));

const QList<QChar> FILE_SEPARATOR_CHARS = { QLatin1Char('/'), QLatin1Char('\\') };

const QChar RAW_COLOR_PREFIX = QLatin1Char('#');
const int RAW_RGB_LENGTH = 7;
const int RAW_ARGB_LENGTH = 9;
const int RAW_COLOR_START = 1;
const int RAW_COLOR_STEP = 2;

void removeFileSeparators(QString &str)
{
    for (const QChar &separator : FILE_SEPARATOR_CHARS)
        str.remove(separator);
}

QString extractAssetExtension(const QString &asset)
{
    const QStringList lst = asset.split(QLatin1Char('.'));
    if (lst.length() > 1)
        return QStringLiteral(".%1").arg(lst.last());
    return QString();
}

QString extractAssetColor(const QString &asset, const QStringList &colorKeys)
{
    QString res;
    for (QString colorKey : colorKeys) {
        removeFileSeparators(colorKey);
        if (!colorKey.isEmpty() && asset.endsWith(colorKey)) {
            res = colorKey;
            break;
        }
    }
    return res;
}

int rawColorChannelValue(const QString &channelValue)
{
    bool ok = false;
    const int value = qMin(255, qMax(0, channelValue.toInt(&ok, 16)));
    return ok ? value : 0;
}
}

Theme::Theme(QObject *parent)
  : QObject(parent)
{
}

QString Theme::themeBaseDir() const
{
    return m_themeBaseDir;
}

QString Theme::parentTheme() const
{
    return m_parentTheme;
}

bool Theme::cacheEffects() const
{
    return m_cacheEffects;
}

void Theme::setThemeBaseDir(const QString &dir)
{
    if (m_themeBaseDir != dir) {
        m_themeBaseDir = dir;
        emit themeBaseDirChanged();
    }
}

QUrl Theme::asset(const QString &id) const
{
    if (id.isEmpty())
        return QUrl();
    if (ASSET_URL_PATTER.match(id).hasMatch())
        return QUrl(id);
    QString assetPath = m_themePrefixDir + m_themeBaseDir + QStringLiteral("/") + id;
    assetPath.remove(QStringLiteral("qrc"));
    if (!QFile::exists(assetPath)) {
        if (m_parentThemeInstance)
            return m_parentThemeInstance->asset(id);
        return QUrl();
    }
    return QUrl(m_imageProviderId + m_themePrefix + m_themePrefixDir + m_themeBaseDir
                + QStringLiteral("/") + id);
}

QUrl Theme::animationFrameAsset(const QString &id, int frame, int frameIdLength) const
{
    QString res;
    QString frameId = QString::number(frame);
    while (frameId.length() < frameIdLength)
        frameId.prepend(QLatin1Char('0'));
    frameId.prepend(QLatin1Char('_'));
    frameId.prepend(id);
    frameId.append(QStringLiteral(".png"));
    QString assetPath = m_themePrefixDir + m_themeBaseDir + QStringLiteral("/") + frameId;
    assetPath.remove(QStringLiteral("qrc"));
    if (!QFile::exists(assetPath)) {
        if (m_parentThemeInstance)
            return m_parentThemeInstance->animationFrameAsset(id, frame, frameIdLength);
        return QUrl();
    }
    return QUrl(m_animationImageProviderId + m_themePrefix + m_themePrefixDir + m_themeBaseDir
                + QStringLiteral("/") + frameId);
}

QUrl Theme::coloredAsset(const QString &id, const QString &colorId) const
{
    if (id.isEmpty())
        return QUrl();
    if (!colorId.isEmpty()) {
        const QString extension = extractAssetExtension(id);
        if (!extension.isEmpty()) {
            QString newId = id.left(id.length() - extension.length());
            const QString currentColorId = extractAssetColor(newId, m_colors.keys());
            if (!currentColorId.isEmpty()) {
                QString cleanColorId = colorId;
                removeFileSeparators(cleanColorId);
                newId = newId.left(newId.length() - currentColorId.length());
                QUrl assetUrl = asset(newId + cleanColorId + extension);
                if (!assetUrl.isEmpty()) {
                    return assetUrl;
                } else {
                    const QStringList alternativeIds = alternativeColorIds(colorId);
                    for (const auto &alternativeId : alternativeIds) {
                        cleanColorId = alternativeId;
                        removeFileSeparators(cleanColorId);
                        assetUrl = asset(newId + cleanColorId + extension);
                        if (!assetUrl.isEmpty())
                            return assetUrl;
                    }
                }
            }
        }
    }
    return asset(id);
}

QFont Theme::font(const QString &id) const
{
    QFont res;
    auto it = m_fontMap.find(id);
    if (it != m_fontMap.constEnd())
        res = it.value();
    else if (m_parentThemeInstance)
        res = m_parentThemeInstance->font(id);
    return res;
}

QMap<QString, QColor> rawColors;
QColor Theme::color(const QString &id) const
{
    QColor res;
    if (id.startsWith(RAW_COLOR_PREFIX) && id.length() >= RAW_RGB_LENGTH
        && id.length() <= RAW_ARGB_LENGTH) {
        auto it = rawColors.find(id);
        if (it == rawColors.end()) {
            int ind = RAW_COLOR_START;
            if (id.length() == RAW_ARGB_LENGTH) {
                res.setAlpha(rawColorChannelValue(id.mid(ind, RAW_COLOR_STEP)));
                ind += RAW_COLOR_STEP;
            }
            res.setRed(rawColorChannelValue(id.mid(ind, RAW_COLOR_STEP)));
            ind += RAW_COLOR_STEP;
            res.setGreen(rawColorChannelValue(id.mid(ind, RAW_COLOR_STEP)));
            ind += RAW_COLOR_STEP;
            res.setBlue(rawColorChannelValue(id.mid(ind, RAW_COLOR_STEP)));
            it = rawColors.insert(id, res);
        }
        return it.value();
    }
    auto it = m_colors.find(id);
    if (it != m_colors.constEnd())
        res = it.value();
    else if (m_parentThemeInstance)
        res = m_parentThemeInstance->color(id);
    return res;
}

QString Theme::colorId(const QColor &color) const
{
    QString res = m_colors.key(color, QString());
    if (res.isEmpty() && m_parentThemeInstance)
        res = m_parentThemeInstance->colorId(color);
    return res;
}

QStringList Theme::alternativeColorIds(const QString &id) const
{
    QStringList alts;
    const QColor idColor = color(id);
    if (m_parentThemeInstance)
        alts = m_parentThemeInstance->alternativeColorIds(id);
    for (auto it = m_colors.constBegin(); it != m_colors.constEnd(); ++it) {
        if (it.key() != id && it.value() == idColor && !alts.contains(it.key()))
            alts.append(it.key());
    }
    return alts;
}

void Theme::setParentThemeInstance(Theme *theme)
{
    m_parentThemeInstance = theme;
}

void Theme::init(const QString &prefixPath)
{
    m_parentTheme.clear();
    if (!m_themeBaseDir.startsWith(QStringLiteral("qrc"))) {
        m_themePrefixDir = prefixPath;
        m_themePrefix = QStringLiteral("file:");
    } else {
        m_themePrefixDir.clear();
        m_themePrefix.clear();
    }
    QString baseDirPath = m_themePrefixDir + m_themeBaseDir;
    baseDirPath.remove(QStringLiteral("qrc"));
    QDir baseDir(baseDirPath);
    if (baseDir.exists()) {
        QFile configFile(baseDir.filePath(QStringLiteral("config.json")));
        if (configFile.open(QIODevice::ReadOnly)) {
            QJsonParseError err = {};
            const QJsonDocument config = QJsonDocument::fromJson(configFile.readAll(), &err);
            if (err.error != QJsonParseError::NoError) {
                themingWarning() << "Mallformed theme config file:" << configFile.fileName()
                                 << err.errorString();
                return;
            } else {
                const QJsonObject themeConfig = config.object();
                m_parentTheme = themeConfig.value(PARENT_THEME_TAG).toString();
                m_imageProviderId = themeConfig.value(IMAGE_PROVIDER_TAG).toString();
                m_animationImageProviderId =
                        themeConfig.value(ANIMATION_IMAGE_PROVIDER_TAG).toString();
                m_cacheEffects = themeConfig.value(CACHE_EFFECTS_TAG).toBool();
            }
        }
    }
    loadFonts();
    loadColors();
}

QByteArray Theme::assetHash(const QByteArray &assetDataStr) const
{
    return QCryptographicHash::hash(assetDataStr, QCryptographicHash::Md5);
}

void Theme::loadFonts()
{
    themingDebug() << "";
    for (int id : m_loadedFontFiles)
        QFontDatabase::removeApplicationFont(id);
    m_loadedFontFiles.clear();
    m_fontMap.clear();
    QString fontsDirPath =
            m_themePrefixDir + m_themeBaseDir + QStringLiteral("/") + QStringLiteral("Fonts");
    fontsDirPath.remove(QStringLiteral("qrc"));
    QDir fontsDir(fontsDirPath);
    if (fontsDir.exists()) {
        QFileInfoList fontFiles = fontsDir.entryInfoList(QStringList() << QStringLiteral("*.ttf")
                                                                       << QStringLiteral("*.otf"));
        for (const QFileInfo &fontFile : fontFiles) {
            themingDebug() << "Loading font file: " << fontFile.absoluteFilePath();
            m_loadedFontFiles.append(
                    QFontDatabase::addApplicationFont(fontFile.absoluteFilePath()));
        }
        QFile fontDefintionsFile(fontsDir.filePath(QStringLiteral("definitions.json")));
        if (fontDefintionsFile.open(QIODevice::ReadOnly)) {
            QJsonParseError err = {};
            const QJsonDocument fontsDefinitions =
                    QJsonDocument::fromJson(fontDefintionsFile.readAll(), &err);
            if (err.error != QJsonParseError::NoError) {
                themingWarning() << "Mallformed fonts defintion file:"
                                 << fontDefintionsFile.fileName() << err.errorString();
                return;
            } else {
                const QJsonArray definitions = fontsDefinitions.array();
                for (const QJsonValue &definition : definitions) {
                    const QJsonObject defObj = definition.toObject();
                    QFont font;
                    font.setFamily(defObj.value(FONT_FAMILY_TAG).toString());
                    font.setStyleName(
                            defObj.value(FONT_STYLE_NAME_TAG).toString(QStringLiteral("Regular")));
                    font.setPixelSize(defObj.value(FONT_PIXEL_SIZE_TAG).toInt());
                    font.setWeight(defObj.value(FONT_WEIGHT_TAG).toInt());
                    font.setCapitalization(
                            QFont::Capitalization(defObj.value(FONT_CAPITALIZATION_TAG).toInt()));
                    font.setItalic(defObj.value(FONT_ITALIC_TAG).toBool());
                    font.setBold(defObj.value(FONT_BOLD_TAG).toBool());
                    font.setUnderline(defObj.value(FONT_UNDERLINE_TAG).toBool());
                    font.setStrikeOut(defObj.value(FONT_STRIKEOUT_TAG).toBool());
                    m_fontMap.insert(defObj.value(FONT_NAME_TAG).toString(), font);
                }
            }
            themingDebug() << "Loaded font definitions" << m_fontMap;
        }
    }
}

void Theme::loadColors()
{
    themingDebug() << "";
    m_colors.clear();
    QString colorsDirPath =
            m_themePrefixDir + m_themeBaseDir + QStringLiteral("/") + QStringLiteral("Colors");
    colorsDirPath.remove(QStringLiteral("qrc"));
    QDir colorsDir(colorsDirPath);
    if (colorsDir.exists()) {
        QFile colorDefintionsFile(colorsDir.filePath(QStringLiteral("definitions.json")));
        if (colorDefintionsFile.open(QIODevice::ReadOnly)) {
            QJsonParseError err = {};
            const QJsonDocument colorsDefinitions =
                    QJsonDocument::fromJson(colorDefintionsFile.readAll(), &err);
            if (err.error != QJsonParseError::NoError) {
                themingWarning() << "Mallformed colors defintion file:"
                                 << colorDefintionsFile.fileName() << err.errorString();
                return;
            } else {
                const QJsonArray definitions = colorsDefinitions.array();
                for (const QJsonValue &definition : definitions) {
                    const QJsonObject defObj = definition.toObject();
                    QColor color;
                    color.setRed(defObj.value(COLOR_R_TAG).toInt());
                    color.setGreen(defObj.value(COLOR_G_TAG).toInt());
                    color.setBlue(defObj.value(COLOR_B_TAG).toInt());
                    color.setAlpha(defObj.value(COLOR_A_TAG).toInt());
                    m_colors.insert(defObj.value(COLOR_NAME_TAG).toString(), color);
                }
            }
            themingDebug() << "Loaded color definitions" << m_colors;
        }
    }
}

ThemeManager::~ThemeManager()
{
    contextThemeManagers.remove(m_context);
    contextThemeCollections.remove(m_context);
    contextTheme.remove(m_context);
    contextThemeBasePath.remove(m_context);
}

GreenHouse::Theme *ThemeManager::theme() const
{
    return m_themeInstance;
}

QString ThemeManager::themeName() const
{
    return contextTheme.value(m_context);
}

void ThemeManager::activateTheme(const QString &name)
{
    if (themeName() != name || !m_themeInstance) {
        if (m_themeInstance) {
            m_themeInstance->deleteLater();
            m_themeInstance = nullptr;
            for (Theme *parentTheme : m_parentThemeInstances)
                parentTheme->deleteLater();
            m_parentThemeInstances.clear();
        }

        auto themes = contextThemeCollections.find(m_context);
        if (themes != contextThemeCollections.end()) {
            auto themeDesc = themes.value().find(name);
            if (themeDesc != themes.value().end()) {
                // NOTE: Prefer synchronous loading for the theme; it should
                // ideally be constructed and ready for the UI before the UI is first drawn.
                auto cmp = new QQmlComponent(m_context->qmlEngine(), themeDesc.value(),
                                             QQmlComponent::CompilationMode::PreferSynchronous);
                auto componentReadyHandler = [cmp, this]() {
                    QObject *obj = cmp->create(m_context->qmlContext());
                    m_themeInstance = qobject_cast<GreenHouse::Theme *>(obj);
                    if (!m_themeInstance && obj) {
                        // Theme conversion failed (non-Theme obj was created);
                        // delete invalid object
                        themingWarning()
                                << "Failed to construct Theme object from Component:" << cmp;
                        obj->deleteLater();
                    } else {
                        // Initialize Theme
                        themingDebug()
                                << "Successfully converted Component-created object to Theme:"
                                << m_themeInstance;
                        m_themeInstance->setParent(this);
                        m_themeInstance->init(contextThemeBasePath.value(m_context, QString()));
                        createThemeParent(m_themeInstance);
                    }
                    cmp->deleteLater();
                    emit themeChanged();
                };
                if (cmp->isReady()) {
                    componentReadyHandler();
                } else {
                    themingWarning() << "Theme component not ready!" << cmp->errorString();
                    if (cmp->isError()) {
                        themingWarning() << "Theme component in error state:" << cmp->errorString();
                    } else {
                        connect(cmp, &QQmlComponent::statusChanged, this,
                                [cmp, componentReadyHandler, this](QQmlComponent::Status status) {
                                    if (status == QQmlComponent::Ready) {
                                        themingWarning() << "Theme component delay Ready";
                                        componentReadyHandler();
                                    } else {
                                        themingWarning() << "Theme component delayed error state:"
                                                         << cmp->errorString();
                                        cmp->deleteLater();
                                        emit themeChanged();
                                    }
                                });
                    }
                }
            }
        }
    }
}

void ThemeManager::createThemeParent(Theme *theme)
{
    if (theme && !theme->parentTheme().isEmpty()) {
        auto themes = contextThemeCollections.find(m_context);
        if (themes != contextThemeCollections.end()) {
            auto themeDesc = themes.value().find(theme->parentTheme());
            if (themeDesc != themes.value().end()) {
                QQmlComponent cmp(m_context->qmlEngine(), themeDesc.value());
                if (cmp.isReady()) {
                    QObject *obj = cmp.create(m_context->qmlContext());
                    auto parentTheme = qobject_cast<GreenHouse::Theme *>(obj);
                    if (!parentTheme && obj) {
                        obj->deleteLater();
                    } else {
                        parentTheme->setParent(theme);
                        theme->setParentThemeInstance(parentTheme);
                        parentTheme->init(contextThemeBasePath.value(m_context, QString()));
                        m_parentThemeInstances.append(parentTheme);
                        createThemeParent(parentTheme);
                    }
                } else {
                    themingWarning() << cmp.errorString();
                }
            }
        }
    }
}

ThemeManager::ThemeManager(Context *context, QObject *parent)
  : QObject(parent)
  , m_context(context)
{
}

QObject *ThemeManagerActivator::contextThemeManagerInstance(Context *context)
{
    auto it = contextThemeManagers.find(context);
    if (it == contextThemeManagers.end()) {
        auto manager = new ThemeManager(context, context->qmlEngine());
        it = contextThemeManagers.insert(context, manager);
        manager->activateTheme(contextTheme.value(context));
    }
    return it.value();
}

void ThemeManagerActivator::loadThemeConfig(const QString &configPath, Context *context,
                                            const QString &themeBasePath)
{
    static bool themeManagerRegisterd = false;
    if (!themeManagerRegisterd) {
        qmlRegisterType<GreenHouse::Theme>("GreenHouse.Theming", 1, 0, "GreenHouseTheme");
        themeManagerRegisterd = true;
    }

    if (!context)
        return;
    QFile configFile(configPath);
    if (configFile.open(QIODevice::ReadOnly)) {
        QJsonParseError err = {};
        const QJsonDocument configDoc = QJsonDocument::fromJson(configFile.readAll(), &err);
        if (err.error != QJsonParseError::NoError) {
            themingWarning() << "Mallformed theme config file:" << configFile.fileName()
                             << err.errorString();
            return;
        } else {
            const QJsonObject configObj = configDoc.object();
            QString defaultTheme = configObj.value(DEFAULT_THEME_TAG).toString();
            const QJsonArray themeArray = configObj.value(THEMES_TAG).toArray();
            QMap<QString, QUrl> themes;
            for (const QJsonValue &val : themeArray) {
                const QJsonObject themeObj = val.toObject();
                const QString themePath =
                        themeBasePath + themeObj.value(THEME_OBJECT_PATH_TAG).toString();
                QRegularExpression regex(QStringLiteral("^(?:qrc|https{0,1}):.*$"));
                const QRegularExpressionMatch matcher(regex.match(themePath));
                if (matcher.hasMatch()) {
                    themes.insert(themeObj.value(THEME_OBJECT_NAME_TAG).toString(),
                                  QUrl(themePath));
                } else {
                    themes.insert(themeObj.value(THEME_OBJECT_NAME_TAG).toString(),
                                  QUrl::fromLocalFile(themePath));
                }
            }
            themingDebug() << "Loaded Themes:" << themes;
            themingDebug() << "Default Theme:" << defaultTheme;
            contextThemeBasePath.insert(context, themeBasePath);
            registerThemeCollection(themes, context);
            activateTheme(defaultTheme, context);
        }
    } else {
        themingWarning() << "Failed to read theme config file:" << configFile.fileName()
                         << configFile.errorString();
    }
}

void ThemeManagerActivator::registerThemeCollection(const QMap<QString, QUrl> &themes,
                                                    Context *context)
{
    if (!context)
        return;
    contextThemeCollections.insert(context, themes);
    // NOTE: Clearing the context's active theme
    if (contextThemeManagers.contains(context))
        contextThemeManagers.value(context)->activateTheme(QString());
    contextTheme.insert(context, QString());
}

void ThemeManagerActivator::activateTheme(const QString &theme, Context *context)
{
    if (!context)
        return;
    if (contextThemeManagers.contains(context))
        contextThemeManagers.value(context)->activateTheme(theme);
    contextTheme.insert(context, theme);
}

QString ThemeManagerActivator::activeTheme(const Context *context)
{
    return contextTheme.value(context, QString());
}

QStringList ThemeManagerActivator::availableThemes(const Context *context)
{
    return contextThemeCollections.value(context, QMap<QString, QUrl>()).keys();
}
}
