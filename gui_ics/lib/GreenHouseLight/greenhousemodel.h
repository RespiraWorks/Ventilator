/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GREENHOUSEMODEL_H
#define GREENHOUSEMODEL_H

#include "greenhouse_global.h"

#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>

#include <functional>

namespace GreenHouse {

///
/// \brief The Model class is a convenience QAbstractListModel subclass designed to work with
/// JsonData
///
class GREENHOUSESHARED_EXPORT Model : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(Model)

    Q_PROPERTY(QStringList roles READ roles CONSTANT)
    Q_PROPERTY(int length READ rowCount NOTIFY lengthChanged)
    Q_PROPERTY(QString modelJsonData READ modelJsonData NOTIFY modelJsonDataChanged)

public:
    ///
    /// \brief Model create a new model instance with the given parent
    /// \param parent
    ///
    explicit Model(QObject *parent = nullptr);
    ~Model() override = default;
    Model(Model &&) = delete;
    Model &operator=(Model &&) = delete;

    ///
    /// \brief Model create a new model instance with the given roles and parent
    /// \param roles a list of strings containing supported model role names
    /// \param parent
    ///
    explicit Model(QStringList roles, QObject *parent = nullptr);

    ///
    /// \brief Model create a new model instance with the given roles, initial data and parent
    /// \param roles a list of strings containing supported model role names
    /// \param data initial model Json data
    /// \param parent
    ///
    explicit Model(QStringList roles, const QByteArray &data, QObject *parent = nullptr);

    ///
    /// \brief Model create a new model instance with the given roles, initial data and parent
    /// \param roles a list of strings containing supported model role names
    /// \param data initial model Json data
    /// \param parent
    ///
    explicit Model(QStringList roles, QJsonArray data, QObject *parent = nullptr);

    ///
    /// \brief roles used to get a list of roles supported by this model instance
    /// \return a list of supporter roles
    ///
    QStringList roles() const { return m_roles; }

    ///
    /// \brief modelJsonData used to get the model's data as a formatted JSON string
    /// \return the model's data as a formatted JSON string
    ///
    QString modelJsonData() const;

    /**
     * \brief dataAt used to retrieve all of the model's data from a given index
     * \param index
     * \return a QJsonObject containing the model data found at the requested index
     */
    Q_INVOKABLE QJsonObject dataAt(int index) const;

    ///
    /// \brief data used to retrieve all of the model's data
    /// \return a QJsonArray containing the model data
    ///
    Q_INVOKABLE QJsonArray data() const;

    ///
    /// \brief populate used to populate and set the model's roles using the provided data
    /// \param roles a list of supported model roles
    /// \param data a QJsonArray containing the model's data
    /// \param silent a flag used to determine if the model is supposed raise the matching notify
    /// signal, this is mainly
    /// used to propagate the model changes via RPC
    ///
    void populate(const QStringList &roles, const QJsonArray &data, bool silent = false);

    ///
    /// \brief populate used to populate the model using the provided data
    /// \param data a QJsonArray containing the model's data
    ///
    void populate(const QJsonArray &data);

    ///
    /// \brief removeRow used to remove a model row
    /// \param index the index of the row to be removed
    /// \param silent a flag used to determine if the model is supposed raise the matching notify
    /// signal, this is mainly
    /// used to propagate the model changes via RPC
    ///
    Q_INVOKABLE void removeRow(int index, bool silent = false);

    ///
    /// \brief prependRow used to prepend a new row to the model
    /// \param data the new row's data
    /// \param silent a flag used to determine if the model is supposed raise the matching notify
    /// signal, this is mainly
    /// used to propagate the model changes via RPC
    ///
    Q_INVOKABLE void prependRow(const QJsonObject &data, bool silent = false);

    ///
    /// \brief appendRow used to append a new row to the model
    /// \param data the new row's data
    /// \param silent a flag used to determine if the model is supposed raise the matching notify
    /// signal, this is mainly
    /// used to propagate the model changes via RPC
    ///
    Q_INVOKABLE void appendRow(const QJsonObject &data, bool silent = false);

    ///
    /// \brief setRowData used to update a row's data
    /// \param rowIndex the updated row's index
    /// \param data updated row data
    /// \param silent a flag used to determine if the model is supposed raise the matching notify
    /// signal, this is mainly
    /// used to propagate the model changes via RPC
    /// \return true if the update was possible false otherwise
    ///
    Q_INVOKABLE bool setRowData(int rowIndex, const QJsonObject &data, bool silent = false);

    ///
    /// \brief setModelJsonData used to update the model's data using a formatted JSON string
    /// \param data a formatted JSON string containg the model's new data
    /// \return true if the update was sucessfull
    ///
    Q_INVOKABLE bool setModelJsonData(const QString &data);

    using CompareFunc = std::function<bool(const QJsonValue &, const QJsonValue &)>;
    ///
    /// \brief sortData used to sort the model's data using the provided compare function
    /// \param compareFunc
    ///
    void sortData(const CompareFunc &compareFunc);

    using FilterFunc = std::function<bool(const QJsonObject &)>;
    ///
    /// \brief setDataFilter used to set the model's custom filter function
    /// \param dataFilter the custom filter function
    ///
    void setDataFilter(const FilterFunc &dataFilter);

    ///
    /// \brief clearDataFilter used to clear any custom model filtering
    ///
    void clearDataFilter();

    ///
    /// \brief clear used to remove all data from this model, does not remove the custom model
    /// filtering, use reset to
    /// remove everything
    ///
    void clear();

    ///
    /// \brief reset used to reset the model, clears all data and removes the custom model filtering
    ///
    void reset();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void lengthChanged() const;
    void modelJsonDataChanged() const;
    void delayedPopulateWithRoles(const QStringList &roles, const QJsonArray &data,
                                  bool silent) const;
    void delayedPopulate(const QJsonArray &data) const;

    void populated(const QStringList &roles, const QJsonArray &data);
    void rowRemoved(int row);
    void rowAppended(const QJsonObject &data);
    void rowPrepended(const QJsonObject &data);
    void rowDataSet(int row, const QJsonObject &data);

private:
    void makeConnections();
    void applyFilter();

    QStringList m_roles;
    QJsonArray m_data;

    FilterFunc m_dataFilter;
    QJsonArray m_filteredData;
};
}

#endif // GREENHOUSEMODEL_H
