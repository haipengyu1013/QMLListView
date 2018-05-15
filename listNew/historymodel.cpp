#include <QFile>

#include "historymodel.h"
#include <QDebug>
#include <QHash>
#include <QtMath>

HistoryModel::HistoryModel(QObject *parent)
    : QAbstractListModel(parent)
{
    query();
}

QHash<int, QByteArray> HistoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "itemChecked";
    roles[Qt::UserRole + 2] = "itemRead";
    roles[Qt::UserRole + 3] = "itemType";
    roles[Qt::UserRole + 4] = "itemTime";
    return roles;
}

int HistoryModel::rowCount(const QModelIndex &) const
{
    return m_list.count();
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_list.count())
        return QVariant();

    if (role == Qt::UserRole + 1)
        return m_list.at(index.row()).checked;
    if (role == Qt::UserRole + 2)
        return m_list.at(index.row()).read;
    if (role == Qt::UserRole + 3)
        return m_list.at(index.row()).type;
    if (role == Qt::UserRole + 4)
        return m_list.at(index.row()).time;

    return QVariant();
}

void HistoryModel::query()
{
    QList<HistoryItem> list;

    /******************** Test ********************/
    for (int i = 0; i < 30; i++) {
        HistoryItem item;
        item.checked = false;
        item.read = false;
        item.type = 0;
        item.time = i;
        list.append(item);
    }
    /**********************************************/

    beginResetModel();
    m_list = list;
    endResetModel();
}

void HistoryModel::clear()
{
    beginResetModel();
    m_list.clear();
    endResetModel();
}

void HistoryModel::remove()
{
    QList<HistoryItem> list;

    foreach (HistoryItem item, m_list)
        if (item.checked == false)
            list.append(item);

    beginResetModel();
    m_list = list;
    endResetModel();
}

QVariant HistoryModel::value(int row, const QString &role)
{
    if (row < 0 || row >= m_list.count())
        return QVariant();

    if (role == "itemChecked")
        return m_list.at(row).checked;
    if (role == "itemRead")
        return m_list.at(row).read;
    if (role == "itemType")
        return m_list.at(row).type;
    if (role == "itemTime")
        return m_list.at(row).time;

    return QVariant();
}

void HistoryModel::setValue(int row, const QString &role, const QVariant &value)
{
    if (row < 0 || row >= m_list.count())
        return;

    HistoryItem item = m_list.at(row);

    if (role == "itemChecked") {
        item.checked = value.toBool();
        if(item.checked){
            m_selectItems.insert(row , item);
            m_Sequence.append(row);
        }
        else {
            m_selectItems.remove(row);
            m_Sequence.removeAll(row);
        }
    }
    if (role == "itemRead")
        item.read = value.toBool();

    m_list.replace(row, item);

    emit dataChanged(index(row), index(row));
}

void HistoryModel::selectItems(int index, int index2 , int mode , int rowCount ) // 0: SelecMode 1 : GridMode
{
    qDebug()<<"mode is "<<mode<<rowCount;

    if(index == -1 || index2 == -1)
        return;

    if(mode == 0) {
        qDebug()<<"mode is SelectMode::ListSelect";

        QList<int> tmpListIndex;
        //得到当前框选的Item
        if(index <= index2) {
            for(int i = index ; i < index2 + 1; ++i ) {
                tmpListIndex.append(i);
            }
        } else {
            for(int i = index ; i >= index2 ; --i ) {
                tmpListIndex.append(i);
            }
        }

        if( qAbs( index - index2) < m_TempAreaItems.count() ) {
            //去勾选
            QMapIterator<int, HistoryItem> i(m_TempAreaItems);
            while (i.hasNext()) {
                i.next();
                if(!tmpListIndex.contains(i.key())){
                    setValue(i.key() , "itemChecked" , false);
                    m_TempAreaItems.remove(i.key());
                }
            }
        } else {
            //勾选
            for(int i = 0 ; i < tmpListIndex.count(); ++i ) {
                if(!value(tmpListIndex.at(i) , "itemChecked").toBool()){
                    setValue(tmpListIndex.at(i) , "itemChecked" , true);
                    HistoryItem item = m_list.at(tmpListIndex.at(i));
                    m_TempAreaItems.insert( tmpListIndex.at(i) , item);
                    qDebug()<<"row is "<<tmpListIndex.at(i);
                }
            }
        }
    }

    if(mode == 1) {
        int tmpIndexRow = index / rowCount; //index所在行
        int tmpindex2Row = index2 / rowCount; //index2所在行
        int tmpRow = qAbs(tmpindex2Row - tmpIndexRow); //相差的行数
        int tmpIndexColumn = index % rowCount; //index所在列
        int tmpIndex2Column = index2 % rowCount; //index2所在列
        int tmpColumn = qAbs(tmpIndex2Column - tmpIndexColumn); //相差的列

        QList<int> tmpGridIndex;
        //得到当前框选的Item
        if(tmpIndexColumn <= tmpIndex2Column) { //从左往右

            if(tmpIndexRow <= tmpindex2Row) { //从上到下
                for(int i = 0; i <= tmpRow  ; ++ i) {
                    for(int j = 0; j <= tmpColumn; ++j) {
                        int curIndex = index + i*rowCount + j;
                        if (!(curIndex < 0 || curIndex >= m_list.count()))
                            tmpGridIndex.append(curIndex);
                    }
                }


            } else {

                for(int i = 0; i <= tmpRow ; ++ i) { //从下到上
                    for(int j = 0; j <= tmpColumn; ++j) {
                        int curIndex = index - i*rowCount + j;
                        if (!(curIndex < 0 || curIndex >= m_list.count()))
                            tmpGridIndex.append(curIndex);
                    }
                }
            }

        } else { //从右往左
            if(tmpIndexRow <= tmpindex2Row) { //从上到下

                for(int i = 0; i <= tmpRow ; ++ i) {
                    for(int j = 0; j <= tmpColumn; ++j) {
                        int curIndex = index + i*rowCount - j;
                        if (!(curIndex < 0 || curIndex >= m_list.count()))
                            tmpGridIndex.append(curIndex);
                    }
                }
            } else {
                for(int i = 0; i <= tmpRow ; ++ i) {//从下到上
                    for(int j = 0; j <= tmpColumn; ++j) {
                        int curIndex = index - i*rowCount - j;
                        if (!(curIndex < 0 || curIndex >= m_list.count()))
                            tmpGridIndex.append(curIndex);
                    }
                }
            }
        }

        //去勾选
        QMapIterator<int, HistoryItem> i(m_TempAreaItems);
        while (i.hasNext()) {
            i.next();
            if(!tmpGridIndex.contains(i.key())){
                setValue(i.key() , "itemChecked" , false);
                m_TempAreaItems.remove(i.key());
            }
        }

        //勾选
        for(int i = 0 ; i < tmpGridIndex.count(); ++i ) {
            if(!value(tmpGridIndex.at(i) , "itemChecked").toBool()){
                setValue(tmpGridIndex.at(i) , "itemChecked" , true);
                HistoryItem item = m_list.at(tmpGridIndex.at(i));
                m_TempAreaItems.insert(tmpGridIndex.at(i) , item);
            }
        }
    }
}

void HistoryModel::clearAreaItems()
{
    for(int row : m_Sequence) {
        qDebug()<<"row is "<<row;
    }
    m_TempAreaItems.clear();
}
