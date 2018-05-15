#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QList>

struct HistoryItem
{
    bool checked;
    bool read;
    int  type;
    int  time;
};

//enum SelectMode{
//    ListSelect = 0,
//    GridSelect = 1
//};


class HistoryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    HistoryModel(QObject *parent = 0);

public:
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public:
    Q_INVOKABLE void query();
    Q_INVOKABLE void clear();
    Q_INVOKABLE void remove();
    Q_INVOKABLE QVariant value(int row, const QString &role);
    Q_INVOKABLE void setValue(int row, const QString &role, const QVariant &value);
    Q_INVOKABLE void selectItems(int index  , int index2 , int mode  , int rowCount );
    Q_INVOKABLE void clearAreaItems();
private:
    QList<HistoryItem> m_list;
    QMap<int , HistoryItem> m_selectItems;
    QList<int> m_Sequence;
    QMap<int , HistoryItem> m_TempAreaItems;

};

#endif // HISTORYMODEL_H
