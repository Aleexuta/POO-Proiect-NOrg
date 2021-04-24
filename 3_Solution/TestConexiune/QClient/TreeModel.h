#pragma once
#ifndef TREEMODEL_H
#define TREEMODEL_H
#include <qabstractitemmodel.h>
#include <qvariant.h>
class TreeItem;
class TreeModel :
    public QAbstractItemModel
{
    Q_OBJECT
private:
    TreeModel(const QStringList& headers, const QString& data, QObject* parent = 0);
    ~TreeModel();
public:
    static TreeModel* instance;
    static TreeModel*   getInstance(const QStringList& headers, const QString& data, QObject* parent = 0);
    static TreeModel* getInstance();
    void deleteInstance();

    QVariant data(const QModelIndex& index, int role)const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)const;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex())const;
    QModelIndex parent(const QModelIndex& index) const;

    int rowCount(const QModelIndex& parent = QModelIndex())const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex& index)const;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role = Qt::EditRole);

    bool insertRows(int position, int rows, const QModelIndex& parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex& parent = QModelIndex());


public:
    TreeItem* getRootItem();
    void setID(int id);
    int getIdForIndex(const QModelIndex& index);
    void setIDNode(int id, const QModelIndex & index=QModelIndex());
    void setIcon(QIcon & icon,const QModelIndex& index = QModelIndex());
private:
    void setupModelData(const QStringList& lines, TreeItem* parent);
    TreeItem* getItem(const QModelIndex & index) const;

    TreeItem* rootItem;
};


#endif
