#ifndef GREENHOUSEVIEWMANAGER_H
#define GREENHOUSEVIEWMANAGER_H

#include "greenhouse_global.h"
#include <QObject>
#include <QMap>
#include <QSet>

class QQuickItem;

namespace GreenHouse {
class Context;
class ViewManager;

class GREENHOUSESHARED_EXPORT ViewItem : public QObject
{
    Q_OBJECT
public:
    explicit ViewItem(const QString &name, const QString &qmlFile, QObject *parent = nullptr);

    Context *context();
    QQuickItem *item() const { return m_item; }
    QString name() { return m_name; }

    void showItem(const QString &replaces = QString());
    void hideItem(const QString &replacedBy = QString());
    void removeUI();
    void createItem();

    void addProxyItem(const QString &proxyItem);
    void setProxyItems(const QSet<QString> &proxyItems);
    QSet<QString> proxyItems() const { return m_proxyItems; }

private slots:
    void requestItem(const QString &name);
    void requestDefault();

private:
    QString m_name;
    QString m_qmlFile;
    QQuickItem *m_item;
    QObject *m_itemInterface;
    QSet<QString> m_proxyItems;
};

class GREENHOUSESHARED_EXPORT ViewSurface : public QObject
{
    Q_OBJECT
public:
    explicit ViewSurface(const QString &name, int z, QObject *parent = nullptr);

    Context *context();
    QQuickItem *surface() const { return m_surface; }
    QString name() { return m_name; }
    ViewItem *activeItem() const { return m_active; }

    void showSurcace();
    void showItem(const QString &name);
    void showDefault();
    void precreateUi();

    void registerItem(ViewItem *item, bool defaultItem = false);
    void removeUI();

    ViewManager *viewManager() const;

private:
    void createSurface();

    int m_z;

    ViewItem *m_default;
    ViewItem *m_active;
    QMap<QString, ViewItem *> m_items;

    QString m_name;
    QQuickItem *m_surface;
};

class GREENHOUSESHARED_EXPORT ViewManager : public QObject
{
    Q_OBJECT
public:
    explicit ViewManager(Context *context, QObject *parent = nullptr);
    explicit ViewManager(Context *context, const QString &descriptorFile,
                         QObject *parent = nullptr);

    Context *context() const { return m_context; }

    void registerSurface(ViewSurface *surface);
    void removeUI();
    void recreateUi();
    void start();
    void setPrecreateUiOnEntry(bool precreate);

public slots:
    void requestItem(const QString &name);
    void requestDefault(const QString &name);

private:
    Context *m_context;
    QMap<QString, ViewSurface *> m_surfaces;
    bool m_running;
    bool m_precreateUi;
};
}

#endif // GREENHOUSEVIEWMANAGER_H
