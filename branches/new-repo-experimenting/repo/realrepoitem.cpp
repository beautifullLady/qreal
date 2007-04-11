#include <QtXml>
#include <QtSql>

#include "realrepoitem.h"

#define UNCONST const_cast<RealRepoItem *>(this)->

static QVariant getQuery(QString query)
{
	qDebug() << query;
	QSqlQuery q1(query);
        if (!q1.next()) {
            return QVariant();
        }
	return q1.value(0);
}

QHash<int,QList<RealRepoItem *> > listByUuid;
QHash<int,QList<QPersistentModelIndex> > indexByUuid;

RealRepoItem::RealRepoItem(NodeType type, int row, int id, RealRepoItem *parent)
    : parentItem(parent), rowNumber(row), childCount(-1),
	m_id(id), m_type(type)
{
    // Record the item's location within its parent.
    switch (type) {
	case Root:	childTableName = "el_0";			break;
	case Category:	childTableName = QString("el_%1").arg(id);	break;
	default:	childTableName = QString("cont_%1").arg(id);	break;
    }

    updateData();

    listByUuid[m_id].append(this);
}

RealRepoItem::~RealRepoItem()
{
    listByUuid[m_id].removeAll(this);

    qDeleteAll(childItems);
}

void RealRepoItem::updateData()
{
    m_name = getQuery(QString("SELECT name FROM el_%1 WHERE id=%2;")
	                .arg(m_type > Element ? m_type : 0).arg(m_id)).toString();
}

void RealRepoItem::setName(QString name)
{
    getQuery(QString("UPDATE el_%1 SET name='%3' WHERE id=%2;")
                            .arg(m_type).arg(m_id).arg(name.replace("'","''")));
}

RealRepoItem *RealRepoItem::child(int row)
{
    if (childItems.contains(row))
        return childItems[row];
	
    RealRepoItem *child;
    QString query;

    if ( m_type < Element ) {
	query = QString("SELECT id FROM %1;").arg(childTableName);
    } else {
	query = QString("SELECT id, type FROM %1;").arg(childTableName);
    }

    if ( ! childQuery.isActive() ) {
	qDebug() << query;
	childQuery.exec(query);
    }

    if ( !childQuery.seek(row) ) {
        qDebug() << "INTERNAL DB INCONSISTENCY";
        return 0;
    }

    int childId = childQuery.value(0).toInt();
    NodeType childCategory;
    if ( m_type == Root )
	childCategory = Category;
    else if ( m_type == Category )
	childCategory = static_cast<NodeType>(m_id);
    else
	childCategory = static_cast<NodeType>(childQuery.value(1).toInt());
    
    child = new RealRepoItem(childCategory, row, childId, this);
    childItems[row] = child;
    return child;
}

int RealRepoItem::rowCount() const
{
    if ( childCount == -1 ) {
	if ( childQuery.isActive() ) {
	    UNCONST childCount = childQuery.size();
	} else {
	    UNCONST childCount = getQuery(QString("SELECT count(*) FROM %1;").arg(childTableName) ).toInt();
	}
    }
	
    return childCount;
}

void RealRepoItem::childrenCleanup()
{
    childCount = -1;
    childQuery.clear();
    qDeleteAll(childItems);
    childItems.clear();
}
