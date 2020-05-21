#include "greenhouseviewmanager.h"
#include "greenhousecontext.h"
#include "greenhouse_log.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQuickItem>

namespace {
const QByteArray requestItemSig = "requestItem(QString)";
const QByteArray requestDefaultSig = "requestDefault()";
const QString layerComponent = QStringLiteral("qrc:/GreenHouseBuiltIn/qml/GreenHouseLayer.qml");

const QString PRELOAD_UI_TAG = QLatin1String("preloadUi");
const QString SURFACES_TAG = QLatin1String("surfaces");
const QString NAME_TAG = QLatin1String("name");
const QString Z_TAG = QLatin1String("z");
const QString ITEMS_TAG = QLatin1String("items");
const QString FILE_TAG = QLatin1String("file");
const QString PROXY_TAG = QLatin1String("proxyItems");
const QString DEFAULT_TAG = QLatin1String("default");

QQuickItem *instantiateItem(GreenHouse::Context *context, const QString &file)
{
    QQuickItem *item = nullptr;
    QQmlComponent cmp(context->qmlEngine(), QUrl(file));
    if (cmp.isReady()) {
        QObject *obj = cmp.create(context->qmlContext());
        item = qobject_cast<QQuickItem *>(obj);
        if (!item && obj)
            obj->deleteLater();
    } else {
        qWarning() << cmp.errorString();
    }
    return item;
}
}

namespace GreenHouse {

ViewItem::ViewItem(const QString &name, const QString &qmlFile, QObject *parent)
  : QObject(parent)
  , m_name(name)
  , m_qmlFile(qmlFile)
  , m_item(nullptr)
  , m_itemInterface(nullptr)
{
}

void ViewItem::showItem(const QString &replaces)
{
    createItem();
    if (m_item) {
        if (m_itemInterface && m_itemInterface->metaObject()->indexOfProperty("replaces") != -1)
            m_itemInterface->setProperty("replaces", QVariant::fromValue(replaces));

        if (m_itemInterface
            && m_itemInterface->metaObject()->indexOfProperty("allowedToDisplay") != -1)
            m_itemInterface->setProperty("allowedToDisplay", QVariant::fromValue(true));
        else
            m_item->setVisible(true);
    } else if (!m_proxyItems.isEmpty()) {
        auto surface = qobject_cast<ViewSurface *>(parent());
        if (surface) {
            auto viewManager = surface->viewManager();
            if (viewManager) {
                for (const auto &proxy : m_proxyItems)
                    viewManager->requestItem(proxy);
            }
        }
    }
}

void ViewItem::hideItem(const QString &replacedBy)
{
    if (m_item) {
        if (m_itemInterface && m_itemInterface->metaObject()->indexOfProperty("replacedBy") != -1)
            m_itemInterface->setProperty("replacedBy", QVariant::fromValue(replacedBy));

        if (m_itemInterface
            && m_itemInterface->metaObject()->indexOfProperty("allowedToDisplay") != -1)
            m_itemInterface->setProperty("allowedToDisplay", QVariant::fromValue(false));
        else
            m_item->setVisible(false);
    }
}

void ViewItem::removeUI()
{
    if (m_item) {
        m_item->deleteLater();
        m_item = nullptr;
        m_itemInterface = nullptr;
    }
}

void ViewItem::createItem()
{
    if (!m_item && !m_qmlFile.isEmpty()) {
        Context *ctx = context();
        if (ctx) {
            m_item = instantiateItem(ctx, m_qmlFile);
            if (m_item) {
                m_item->setParent(this);
                ViewSurface *parentSurface = qobject_cast<ViewSurface *>(parent());
                if (parentSurface && parentSurface->surface()) {
                    m_item->setParentItem(parentSurface->surface());
                    const QVariant interface = m_item->property("itemInterface");
                    if (interface.canConvert<QObject *>()) {
                        m_itemInterface = interface.value<QObject *>();
                        if (m_itemInterface) {
                            m_itemInterface->setProperty("itemName", m_name);
                            m_itemInterface->setProperty("surfaceName", parentSurface->name());
                            const QMetaObject *meta = m_itemInterface->metaObject();
                            int requestItemIndex = meta->indexOfSignal(requestItemSig);
                            if (requestItemIndex != -1) {
                                QMetaMethod signal = meta->method(requestItemIndex);
                                QMetaMethod slot = metaObject()->method(
                                        metaObject()->indexOfSlot(requestItemSig));
                                connect(m_itemInterface, signal, this, slot);
                            }
                            int requestDefaultIndex = meta->indexOfSignal(requestDefaultSig);
                            if (requestDefaultIndex != -1) {
                                QMetaMethod signal = meta->method(requestDefaultIndex);
                                QMetaMethod slot = metaObject()->method(
                                        metaObject()->indexOfSlot(requestDefaultSig));
                                connect(m_itemInterface, signal, this, slot);
                            }
                        }
                    }
                    if (m_itemInterface
                        && m_itemInterface->metaObject()->indexOfSignal("init()") != -1)
                        QMetaObject::invokeMethod(m_itemInterface, "init", Qt::DirectConnection);
                    else
                        m_item->setVisible(false);
                } else {
                    m_item->deleteLater();
                    m_item = nullptr;
                    ghWarning() << Q_FUNC_INFO << "No surface found for:" << m_name;
                }
            } else {
                ghWarning() << Q_FUNC_INFO << "QML loading failed:" << m_qmlFile;
            }
        } else {
            ghWarning() << Q_FUNC_INFO << "No context found for:" << m_name;
        }
    }
}

void ViewItem::addProxyItem(const QString &proxyItem)
{
    if (proxyItem.isEmpty())
        return;
    m_proxyItems.insert(proxyItem);
}

void ViewItem::setProxyItems(const QSet<QString> &proxyItems)
{
    m_proxyItems = proxyItems;
}

void ViewItem::requestItem(const QString &name)
{
    ViewSurface *parentSurface = qobject_cast<ViewSurface *>(parent());
    if (parentSurface && parentSurface->activeItem() == this) {
        ViewManager *viewManager = qobject_cast<ViewManager *>(parentSurface->parent());
        if (viewManager)
            viewManager->requestItem(name);
    }
}

void ViewItem::requestDefault()
{
    ViewSurface *parentSurface = qobject_cast<ViewSurface *>(parent());
    if (parentSurface && parentSurface->activeItem() == this) {
        parentSurface->showDefault();
    }
}

Context *ViewItem::context()
{
    Context *context = nullptr;
    ViewSurface *vs = qobject_cast<ViewSurface *>(this->parent());
    if (vs)
        context = vs->context();
    return context;
}

ViewSurface::ViewSurface(const QString &name, int z, QObject *parent)
  : QObject(parent)
  , m_z(z)
  , m_default(nullptr)
  , m_active(nullptr)
  , m_items()
  , m_name(name)
  , m_surface(nullptr)
{
}

Context *ViewSurface::context()
{
    Context *context = nullptr;
    ViewManager *vm = qobject_cast<ViewManager *>(this->parent());
    if (vm)
        context = vm->context();
    return context;
}

void ViewSurface::showSurcace()
{
    createSurface();
    if (m_surface)
        m_surface->setVisible(true);
    if (m_active) {
        m_active->showItem();
    } else if (m_default) {
        m_active = m_default;
        m_active->showItem();
    }
}

void ViewSurface::showItem(const QString &name)
{
    ViewItem *item = m_items.value(name, nullptr);
    if (item && item != m_active) {
        QString oldItemName;
        if (m_active) {
            m_active->hideItem(name);
            oldItemName = m_active->name();
        }
        m_active = item;
        m_active->showItem(oldItemName);
    }
}

void ViewSurface::showDefault()
{
    const QString defaultItemName = m_default ? m_default->name() : QString();
    const QString oldItemName = m_active ? m_active->name() : QString();
    if (m_default != m_active && m_active)
        m_active->hideItem(defaultItemName);
    m_active = m_default;
    if (m_active)
        m_active->showItem(oldItemName);
}

void ViewSurface::precreateUi()
{
    for (ViewItem *item : m_items.values())
        item->createItem();
}

void ViewSurface::registerItem(ViewItem *item, bool defaultItem)
{
    if (!item) {
        ghWarning() << Q_FUNC_INFO << "Invalid item instance";
        return;
    }
    if (m_items.contains(item->name())) {
        ghWarning() << Q_FUNC_INFO << "Name colision detected:" << item->name();
        return;
    }
    m_items.insert(item->name(), item);
    item->setParent(this);
    if (defaultItem)
        m_default = item;
}

void ViewSurface::removeUI()
{
    if (m_surface) {
        for (ViewItem *item : m_items.values())
            item->removeUI();
        m_surface->deleteLater();
        m_surface = nullptr;
    }
}

ViewManager *ViewSurface::viewManager() const
{
    return qobject_cast<ViewManager *>(parent());
}

void ViewSurface::createSurface()
{
    if (!m_surface) {
        Context *ctx = context();
        if (ctx) {
            m_surface = instantiateItem(ctx, layerComponent);
            if (m_surface) {
                m_surface->setZ(m_z);
                m_surface->setParent(this);
                if (ctx->contextRootItem()) {
                    m_surface->setParentItem(ctx->contextRootItem());
                    m_surface->setVisible(false);
                } else {
                    m_surface->deleteLater();
                    m_surface = nullptr;
                    ghWarning() << Q_FUNC_INFO << "No root item found for:" << m_name;
                    return;
                }
            } else {
                ghWarning() << Q_FUNC_INFO << "QML loading failed.";
                return;
            }
        } else {
            ghWarning() << Q_FUNC_INFO << "No context found for:" << m_name;
            return;
        }
    }
}

ViewManager::ViewManager(Context *context, QObject *parent)
  : QObject(parent)
  , m_context(context)
  , m_surfaces()
  , m_running(false)
  , m_precreateUi(false)
{
}

ViewManager::ViewManager(Context *context, const QString &descriptorFile, QObject *parent)
  : QObject(parent)
  , m_context(context)
  , m_surfaces()
  , m_running(false)
  , m_precreateUi(false)
{
    QFile descriptor(descriptorFile);
    if (descriptor.exists()) {
        if (descriptor.open(QIODevice::ReadOnly)) {
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(descriptor.readAll(), &error);
            if (error.error == QJsonParseError::NoError) {
                const QJsonObject docObj = doc.object();
                auto it = docObj.constFind(SURFACES_TAG);
                if (it != docObj.constEnd()) {
                    const QJsonArray surfaceArray = it.value().toArray();
                    for (const auto sIt : surfaceArray) {
                        const QJsonObject surfaceObj = sIt.toObject();
                        auto objIt = surfaceObj.constFind(NAME_TAG);
                        if (objIt != surfaceObj.constEnd()) {
                            const QString surfaceName = objIt.value().toString();
                            int z = 0;
                            objIt = surfaceObj.constFind(Z_TAG);
                            if (objIt != surfaceObj.constEnd())
                                z = objIt.value().toInt();
                            ViewSurface *surface = new ViewSurface(surfaceName, z, this);
                            objIt = surfaceObj.constFind(ITEMS_TAG);
                            if (objIt != surfaceObj.constEnd()) {
                                const QJsonArray itemArray = objIt.value().toArray();
                                for (const auto itemIt : itemArray) {
                                    const QJsonObject itemObj = itemIt.toObject();
                                    objIt = itemObj.constFind(NAME_TAG);
                                    if (objIt != itemObj.constEnd()) {
                                        const QString itemName = objIt.value().toString();
                                        objIt = itemObj.constFind(FILE_TAG);
                                        if (objIt != itemObj.constEnd()) {
                                            const QString itemFile = objIt.value().toString();
                                            bool isDefault = false;
                                            objIt = itemObj.constFind(DEFAULT_TAG);
                                            if (objIt != itemObj.constEnd())
                                                isDefault = objIt.value().toBool();
                                            ViewItem *item =
                                                    new ViewItem(itemName, itemFile, surface);
                                            surface->registerItem(item, isDefault);
                                            objIt = itemObj.constFind(PROXY_TAG);
                                            if (objIt != itemObj.constEnd()) {
                                                const QJsonArray proxyArray =
                                                        objIt.value().toArray();
                                                for (const auto &proxy : proxyArray)
                                                    item->addProxyItem(proxy.toString());
                                            }
                                        }
                                    }
                                }
                            }
                            registerSurface(surface);
                        }
                    }
                    it = docObj.constFind(PRELOAD_UI_TAG);
                    if (it != docObj.constEnd())
                        m_precreateUi = it.value().toBool();
                } else {
                    ghWarning() << Q_FUNC_INFO << "The provided" << descriptorFile
                                << "descriptor file does not contain any surfaces.";
                }
            } else {
                ghWarning() << Q_FUNC_INFO << "The provided" << descriptorFile
                            << "descriptor file is invalid.";
                ghWarning() << error.errorString();
            }
        } else {
            ghWarning() << Q_FUNC_INFO << "The provided" << descriptorFile
                        << "descriptor file is not readable.";
        }
    } else {
        ghWarning() << Q_FUNC_INFO << "The provided" << descriptorFile
                    << "descriptor file does not exist.";
    }
}

void ViewManager::registerSurface(ViewSurface *surface)
{
    if (!surface) {
        ghWarning() << Q_FUNC_INFO << "Invalid surface instance";
        return;
    }
    if (m_surfaces.contains(surface->name())) {
        ghWarning() << Q_FUNC_INFO << "Name colision detected:" << surface->name();
        return;
    }
    m_surfaces.insert(surface->name(), surface);
    surface->setParent(this);
}

void ViewManager::removeUI()
{
    for (ViewSurface *surface : m_surfaces.values())
        surface->removeUI();
}

void ViewManager::recreateUi()
{
    if (!m_running)
        return;
    for (ViewSurface *surface : m_surfaces.values()) {
        surface->showSurcace();
        if (m_precreateUi)
            surface->precreateUi();
    }
}

void ViewManager::start()
{
    if (m_running)
        return;
    for (ViewSurface *surface : m_surfaces.values()) {
        surface->showSurcace();
        if (m_precreateUi)
            surface->precreateUi();
    }
    m_running = true;
}

void ViewManager::setPrecreateUiOnEntry(bool precreate)
{
    if (m_precreateUi != precreate) {
        m_precreateUi = precreate;
        if (m_running) {
            if (m_precreateUi) {
                for (ViewSurface *surface : m_surfaces.values())
                    surface->precreateUi();
            } else {
                removeUI();
                recreateUi();
            }
        }
    }
}

void ViewManager::requestItem(const QString &name)
{
    for (ViewSurface *surface : m_surfaces.values())
        surface->showItem(name);
}

void ViewManager::requestDefault(const QString &name)
{
    ViewSurface *surface = m_surfaces.value(name, nullptr);
    if (surface)
        surface->showDefault();
}
}
