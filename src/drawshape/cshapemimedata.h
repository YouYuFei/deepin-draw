/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     WangXin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CSHAPEMIMEDATA_H
#define CSHAPEMIMEDATA_H

#include "sitemdata.h"

#include <QGraphicsItem>
#include <QMimeData>

class CGraphicsItem;

class CShapeMimeData : public QMimeData
{
    Q_OBJECT
public:
    //explicit CShapeMimeData(QList<QGraphicsItem *> items);

    explicit CShapeMimeData(const CGroupBzItemsTreeInfo &itemsTreeInfo);
    ~CShapeMimeData();

//    QList<CGraphicsItem *> itemList() const;
//    QList<CGraphicsItem *> creatCopyItems() const;


    bool hasFormat(const QString &mimetype) const;

    CGroupBzItemsTreeInfo itemsTreeInfo();

private:
    //QList<CGraphicsItem *> m_itemList;

    CGroupBzItemsTreeInfo treeInfo = CGroupBzItemsTreeInfo();
};

#endif // CSHAPEMIMEDATA_H
