/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "greenhousecontext.h"
#include "greenhousetheme.h"
#include "greenhousemodel.h"
#include "greenhouse_log.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTranslator>

struct GreenHouseQRCInit {
    GreenHouseQRCInit() { Q_INIT_RESOURCE(builtinitems); }
};
static GreenHouseQRCInit init;

namespace GreenHouse {

Context::Context(QQmlEngine *qmlEngine, const QString &themePrefix, QQuickItem *rootItem)
  : m_qmlEngine(qmlEngine)
  , m_qmlContext(new QQmlContext(m_qmlEngine->rootContext(), m_qmlEngine))
  , m_rootItem(rootItem)
{
    qmlEngine->addImportPath(QStringLiteral("qrc:/"));
    qmlEngine->addImportPath(QStringLiteral("qrc:/GreenHouse/Imports/"));

    static int greenHouseModelId = qmlRegisterUncreatableType<GreenHouse::Model>(
            "GreenHouse", 1, 0, "GreenHouseModel",
            QStringLiteral("GreenHouseModel is not creatable"));
    Q_UNUSED(greenHouseModelId)

    static int greenHouseAbstractModelId = qmlRegisterUncreatableType<QAbstractItemModel>(
            "GreenHouse", 1, 0, "GreenHouseAbstractModel",
            QStringLiteral("GreenHouseAbstractModel is not creatable"));
    Q_UNUSED(greenHouseAbstractModelId)

    QDir themesDir = QDir(qApp->applicationDirPath());
    bool standaloneThemeFolderFound = false;
    if (themesDir.cdUp()) {
        if (themesDir.cd(QStringLiteral("themes"))) {
            if (themesDir.cd(themePrefix)) {
                ThemeManagerActivator::loadThemeConfig(
                        themesDir.absoluteFilePath(QStringLiteral("config.json")), this,
                        themesDir.absolutePath());
                standaloneThemeFolderFound = true;
            }
        }
    }
    if (!standaloneThemeFolderFound) {
        ThemeManagerActivator::loadThemeConfig(
                QStringLiteral(":/Themes/%1/config.json").arg(themePrefix), this);
    }
    m_qmlContext->setContextProperty(QStringLiteral("GreenHouseThemeManager"),
                                     ThemeManagerActivator::contextThemeManagerInstance(this));

    m_translationsDir =
            QDir(qApp->applicationDirPath(), QString(), QDir::NoSort, QDir::Files | QDir::Readable);
    if (m_translationsDir.cdUp()) {
        if (!m_translationsDir.cd(QStringLiteral("translations")))
            ghWarning() << "Default translations dir not found.";

    } else {
        ghWarning() << "Failed to cd up from the application dir in search for the default "
                       "translations dir.";
    }
}

Context::~Context()
{
    if (m_qmlContext)
        m_qmlContext->deleteLater();
    ThemeManagerActivator::contextThemeManagerInstance(this)->deleteLater();
}

QQmlEngine *Context::qmlEngine() const
{
    return m_qmlEngine;
}

QQmlContext *Context::qmlContext() const
{
    return m_qmlContext;
}

QQuickItem *Context::contextRootItem() const
{
    return m_rootItem;
}

QString Context::activeLanguageId() const
{
    return m_activeLanguageId;
}

QDir Context::translationsDir() const
{
    return m_translationsDir;
}

QString Context::activeTheme() const
{
    return ThemeManagerActivator::activeTheme(this);
}

QStringList Context::availableThemes() const
{
    return ThemeManagerActivator::availableThemes(this);
}

void Context::setRootItem(QQuickItem *rootItem)
{
    m_rootItem = rootItem;
}

void Context::activateLanguage(const QString &languageId)
{
    if (m_activeLanguageId != languageId) {
        for (QTranslator *translator : m_activetTranslators) {
            qApp->removeTranslator(translator);
            translator->deleteLater();
        }
        m_activetTranslators.clear();
        m_activeLanguageId = languageId;
        if (!m_activeLanguageId.isEmpty()) {
            m_translationsDir.setNameFilters(QStringList()
                                             << QStringLiteral("*%1*.qm").arg(m_activeLanguageId));
            QFileInfoList translationFiles = m_translationsDir.entryInfoList();
            for (const QFileInfo &translationFile : translationFiles) {
                auto translator = new QTranslator(qApp);
                if (translator->load(translationFile.baseName(), m_translationsDir.path())) {
                    qApp->installTranslator(translator);
                    m_activetTranslators.append(translator);
                } else {
                    ghWarning() << "Failed to load translation file:"
                                << translationFile.absoluteFilePath();
                    translator->deleteLater();
                }
            }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
            if (m_qmlEngine)
                m_qmlEngine->retranslate();
#endif
        }
    }
}

void Context::setTranslationsDir(const QDir &dir)
{
    if (m_translationsDir != dir) {
        m_translationsDir = dir;
        if (!m_activeLanguageId.isEmpty()) {
            const QString languageId = m_activeLanguageId;
            // NOTE: Retranslating using the newly provided translations folder
            // Clearing the active language to remove all old QTranslator instances
            activateLanguage(QString());
            activateLanguage(languageId);
        }
    }
}

void Context::activateTheme(const QString &theme)
{
    if (theme != activeTheme() && availableThemes().contains(theme)) {
        ThemeManagerActivator::activateTheme(theme, this);
    }
}
}
