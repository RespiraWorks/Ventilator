/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "greenhousemodel.h"
#include "greenhouse_log.h"

#include <QJsonDocument>
#include <QThread>

#include <utility>

// NOTE: https://bugreports.qt.io/browse/QTBUG-44944
// This is a bit of a hack but good enough for our usecase
inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

namespace GreenHouse {

Model::Model(QObject *parent)
  : QAbstractListModel(parent)
{
    makeConnections();
}

Model::Model(QStringList roles, QObject *parent)
  : QAbstractListModel(parent)
  , m_roles(std::move(roles))
{
    makeConnections();
}

Model::Model(QStringList roles, const QByteArray &data, QObject *parent)
  : QAbstractListModel(parent)
  , m_roles(std::move(roles))
{
    QJsonParseError err = {};
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        ghWarning() << "Model data parsing error:" << err.errorString() << data;
    } else {
        m_data = doc.array();
    }
    makeConnections();
}

Model::Model(QStringList roles, QJsonArray data, QObject *parent)
  : QAbstractListModel(parent)
  , m_roles(std::move(roles))
  , m_data(std::move(data))
{
    makeConnections();
}

QString Model::modelJsonData() const
{
    const QJsonDocument doc(m_dataFilter ? m_filteredData : m_data);
    return QLatin1String(doc.toJson(QJsonDocument::Indented));
}

QJsonObject Model::dataAt(int index) const
{
    const auto &modelData = m_dataFilter ? m_filteredData : m_data;
    return index >= 0 && modelData.size() > index ? modelData.at(index).toObject() : QJsonObject();
}

QJsonArray Model::data() const
{
    return m_dataFilter ? m_filteredData : m_data;
}

void Model::populate(const QStringList &roles, const QJsonArray &data, bool silent)
{
    if (thread() != QThread::currentThread()) {
        emit delayedPopulateWithRoles(roles, data, silent);
        return;
    }
    if (!m_dataFilter)
        beginResetModel();
    m_roles = roles;
    m_data = data;
    if (!m_dataFilter)
        endResetModel();
    else
        applyFilter();
    if (!silent)
        emit populated(roles, data);
}

void Model::populate(const QJsonArray &data)
{
    if (thread() != QThread::currentThread()) {
        emit delayedPopulate(data);
        return;
    }

    if (!m_dataFilter)
        beginResetModel();
    m_data = data;
    if (!m_data.isEmpty() && m_roles.isEmpty()) {
        const QJsonObject firstDataObj = m_data.first().toObject();
        for (auto it = firstDataObj.constBegin(); it != firstDataObj.constEnd(); ++it)
            m_roles.append(it.key());
    }
    if (!m_dataFilter)
        endResetModel();
    else
        applyFilter();
    emit populated(m_roles, data);
}

void Model::removeRow(int index, bool silent)
{
    if (m_dataFilter) {
        if (index >= 0 && index < m_filteredData.size()) {
            const auto removedData = m_filteredData.at(index);
            for (int i = 0; i < m_data.size(); ++i) {
                if (m_data.at(i) == removedData) {
                    m_data.removeAt(i);
                    break;
                }
            }
            beginRemoveRows(QModelIndex(), index, index);
            m_filteredData.removeAt(index);
            endRemoveRows();
        }
    } else {
        if (index >= 0 && index < m_data.size()) {
            beginRemoveRows(QModelIndex(), index, index);
            m_data.removeAt(index);
            endRemoveRows();
        }
    }
    if (!silent)
        emit rowRemoved(index);
}

void Model::prependRow(const QJsonObject &data, bool silent)
{
    if (m_dataFilter) {
        m_data.prepend(data);
        if (m_dataFilter(data)) {
            beginInsertRows(QModelIndex(), 0, 0);
            m_filteredData.prepend(data);
            endInsertRows();
            if (!silent)
                emit rowPrepended(data);
        }
    } else {
        beginInsertRows(QModelIndex(), 0, 0);
        m_data.prepend(data);
        endInsertRows();
    }
    if (!silent)
        emit rowPrepended(data);
}

void Model::appendRow(const QJsonObject &data, bool silent)
{
    if (m_dataFilter) {
        m_data.append(data);
        if (m_dataFilter(data)) {
            beginInsertRows(QModelIndex(), m_filteredData.size(), m_filteredData.size());
            m_filteredData.append(data);
            endInsertRows();
            if (!silent)
                emit rowAppended(data);
        }
    } else {
        beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
        m_data.append(data);
        endInsertRows();
    }
    if (!silent)
        emit rowAppended(data);
}

bool Model::setRowData(int rowIndex, const QJsonObject &data, bool silent)
{
    if (m_dataFilter) {
        if (rowIndex < 0 || rowIndex >= m_filteredData.size())
            return false;
        const auto replacedData = m_filteredData.at(rowIndex).toObject();
        for (int i = 0; i < m_data.size(); ++i) {
            if (m_data.at(i) == replacedData) {
                m_data.replace(i, data);
                break;
            }
        }
        if (m_dataFilter(data)) {
            m_filteredData.replace(rowIndex, data);
            emit dataChanged(index(rowIndex), index(rowIndex));
        } else {
            beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
            m_filteredData.removeAt(rowIndex);
            endRemoveRows();
        }
    } else {
        if (rowIndex < 0 || rowIndex >= m_data.size())
            return false;
        m_data.replace(rowIndex, data);
        emit dataChanged(index(rowIndex), index(rowIndex));
    }
    if (!silent)
        emit rowDataSet(rowIndex, data);
    return true;
}

bool Model::setModelJsonData(const QString &data)
{
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(data.toLatin1(), &error);
    if (error.error == QJsonParseError::NoError) {
        if (doc.isArray()) {
            populate(doc.array());
            emit populated(m_roles, m_data);
            return true;
        }
        ghWarning() << "Model JSON must be in array format.";
    } else {
        ghWarning() << error.errorString();
    }
    return false;
}

void Model::sortData(const CompareFunc &compareFunc)
{
    beginResetModel();
    std::sort(m_data.begin(), m_data.end(), compareFunc);
    if (m_dataFilter)
        std::sort(m_filteredData.begin(), m_filteredData.end(), compareFunc);
    endResetModel();
}

void Model::setDataFilter(const FilterFunc &dataFilter)
{
    m_dataFilter = dataFilter;
    applyFilter();
}

void Model::clearDataFilter()
{
    if (m_dataFilter) {
        beginResetModel();
        m_dataFilter = nullptr;
        m_filteredData = QJsonArray();
        endResetModel();
    }
}

void Model::clear()
{
    populate({});
}

void Model::reset()
{
    clearDataFilter();
    clear();
}

int Model::rowCount(const QModelIndex &) const
{
    return m_dataFilter ? m_filteredData.size() : m_data.size();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    QVariant res;
    QJsonObject rowData = dataAt(index.row());
    int roleIndex = role - Qt::UserRole;
    if (roleIndex >= 0 && roleIndex < m_roles.length())
        res = rowData.value(m_roles.value(roleIndex)).toVariant();
    return res;
}

QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> roles;
    for (int i = 0; i < m_roles.length(); ++i)
        roles.insert(Qt::UserRole + i, m_roles.at(i).toLatin1());
    return roles;
}

void Model::makeConnections()
{
    connect(this, &Model::rowsRemoved, this, &Model::lengthChanged);
    connect(this, &Model::rowsInserted, this, &Model::lengthChanged);
    connect(this, &Model::modelReset, this, &Model::lengthChanged);
    connect(this, &Model::rowsRemoved, this, &Model::modelJsonDataChanged);
    connect(this, &Model::rowsInserted, this, &Model::modelJsonDataChanged);
    connect(this, &Model::modelReset, this, &Model::modelJsonDataChanged);
    connect(this, &Model::dataChanged, this, &Model::modelJsonDataChanged);

    // NOTE: using the raw overload connection syntax to keep Qt 5.6 compatibility
    connect(this, &Model::delayedPopulateWithRoles, this,
            static_cast<void (Model::*)(const QStringList &, const QJsonArray &, bool)>(
                    &Model::populate),
            Qt::QueuedConnection);
    connect(this, &Model::delayedPopulate, this,
            static_cast<void (Model::*)(const QJsonArray &)>(&Model::populate),
            Qt::QueuedConnection);
}

void Model::applyFilter()
{
    if (m_dataFilter) {
        beginResetModel();
        m_filteredData = QJsonArray();
        // NOTE: We can't use a reference as QJsonArray does not provide reference access to its
        // values
        // and using QJsonValueRef does not buy us anything as it is intended for inline editing
        // rather
        // than just pure access
        for (const QJsonValue row : m_data) {
            if (m_dataFilter(row.toObject()))
                m_filteredData.append(row);
        }
        endResetModel();
    }
}
}
