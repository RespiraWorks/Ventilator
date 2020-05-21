/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GREENHOUSECONTEXT_H
#define GREENHOUSECONTEXT_H

#include "greenhouse_global.h"
#include "greenhouse_log.h"

#include "greenhousemodel.h"

#include <QDir>
#include <QHash>
#include <QJsonObject>

class QObject;

class QQuickItem;
class QQmlEngine;
class QQmlContext;
class QTranslator;

namespace GreenHouse {

///
/// \brief The Context class is used to provide an execution context for a GUI process.
///
/// The Context object is responsible for encapsulating a view manager based
/// UI process, by providing them with means to resolve their dependencies and providing an API
/// to change the UIs layout or trigger retransaltions.
///
class GREENHOUSESHARED_EXPORT Context
{
    Q_DISABLE_COPY(Context)
public:
    ///
    /// \brief Context Create a new context
    ///
    explicit Context() = default;

    ///
    /// /brief Context Create a new context using the provided QQmlEngine, theme prefix and root
    /// item /param qmlEngine a QQmlEngine instance to be used by the context's UI /param
    /// themePrefix a theme prefix string used to access the UI's root theme folder /param rootItem
    /// a QQuickItem to be used as the root container for the UI
    ///
    explicit Context(QQmlEngine *qmlEngine, const QString &themePrefix,
                     QQuickItem *rootItem = nullptr);

    virtual ~Context();
    Context(Context &&) = delete;
    Context &operator=(Context &&) = delete;

    ///
    /// \brief qmlEngine
    /// \return the QQmlEngine instance used by this context.
    ///
    QQmlEngine *qmlEngine() const;

    ///
    /// \brief qmlContext
    /// \return the QQmlContext used by the UI.
    ///
    QQmlContext *qmlContext() const;

    ///
    /// \brief contextRootItem
    /// \return the UI's current root container item
    ///
    QQuickItem *contextRootItem() const;

    ///
    /// \brief activeLanguageId
    /// \return the id of the UI's currently active language
    ///
    QString activeLanguageId() const;

    ///
    /// \brief translationsDir
    /// \return the directory containing the UI's translation files
    ///
    QDir translationsDir() const;

    ///
    /// \brief activeTheme
    /// \return the name of the UI's currently active theme
    ///
    QString activeTheme() const;

    ///
    /// \brief availableThemes
    /// \return a list of all available theme names
    ///
    QStringList availableThemes() const;

    ///
    /// \brief setRootItem used to set the UI's root item
    /// \param rootItem a QQuickItem to be used as the root container for the UI
    ///
    void setRootItem(QQuickItem *rootItem);

    ///
    /// \brief activateLanguage used to set the UI's active language
    /// \param languageId a language id for example "EN-us"
    ///
    void activateLanguage(const QString &languageId);

    ///
    /// \brief setTranslationsDir used to set the directory containing the UI's translations
    /// \param dir
    ///
    void setTranslationsDir(const QDir &dir);

    ///
    /// \brief activateTheme used to activate a theme
    /// \param theme the desired theme name
    ///
    void activateTheme(const QString &theme);

private:
    QQmlEngine *m_qmlEngine = nullptr;
    QQmlContext *m_qmlContext = nullptr;
    QQuickItem *m_rootItem = nullptr;

    QVector<QTranslator *> m_activetTranslators;
    QString m_activeLanguageId;
    QDir m_translationsDir;
};

template <class T>
inline void cleanupProperty(T, QObject *)
{
}
template <>
inline void cleanupProperty(QObject *propertyObj, QObject *owner)
{
    if (propertyObj && propertyObj->parent() == owner) {
        propertyObj->deleteLater();
        propertyObj = nullptr;
    }
}
template <>
inline void cleanupProperty(Model *propertyModel, QObject *owner)
{
    if (propertyModel && propertyModel->parent() == owner) {
        propertyModel->deleteLater();
        propertyModel = nullptr;
    }
}

template <class T>
inline bool different(T a, T b)
{
    return a != b;
}
template <>
inline bool different(const QString &a, const QString &b)
{
    return a.compare(b) != 0;
}
template <>
inline bool different(float a, float b)
{
    return a < b || a > b;
}
template <>
inline bool different(double a, double b)
{
    return a < b || a > b;
}
}

#endif // GREENHOUSECONTEXT_H
