/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     Renran
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

#include "cpentool.h"
#include "cdrawscene.h"
#include "cgraphicspenitem.h"
#include "cdrawparamsigleton.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

CPenTool::CPenTool()
    : IDrawTool (pen)
    , m_pPenItem(nullptr)
{

}

CPenTool::~CPenTool()
{

}

void CPenTool::mousePressEvent(QGraphicsSceneMouseEvent *event, CDrawScene *scene)
{
//    if (event->button() == Qt::LeftButton) {
//        scene->clearSelection();
//        m_sPointPress = event->scenePos();
//        m_pPenItem = new CGraphicsPenItem(m_sPointPress);
//        m_pPenItem->setDrawFlag(true);
//        QPen pen = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getPen();
//        m_pPenItem->setPen(pen);
//        m_pPenItem->setBrush(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getBrush());
//        m_pPenItem->setPenStartType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getPenStartType());
//        m_pPenItem->setPenEndType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getPenEndType());
//        m_pPenItem->setPixmap();
//        m_pPenItem->setZValue(scene->getMaxZValue() + 1);
//        scene->addItem(m_pPenItem);


//        m_bMousePress = true;
//    } /*else if (event->button() == Qt::RightButton) {
//        CDrawParamSigleton::GetInstance()->setCurrentDrawToolMode(selection);
//        emit scene->signalChangeToSelect();
//    } */else {
//        scene->mouseEvent(event);
//    }

    if (event->button() == Qt::LeftButton) {
        IDrawTool::CDrawToolEvent::CDrawToolEvents e = IDrawTool::CDrawToolEvent::fromQEvent(event, scene);
        toolStart(&e.first());
    }
    //scene->mouseEvent(event);
}

void CPenTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, CDrawScene *scene)
{
//    Q_UNUSED(scene)
//    if (m_bMousePress && nullptr != m_pPenItem) {
//        QPointF pointMouse = event->scenePos();
//        bool shiftKeyPress = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getShiftKeyStatus();
//        m_pPenItem->updatePenPath(pointMouse, shiftKeyPress);
//    }
//    scene->mouseEvent(event);

    if (event->buttons() & Qt::LeftButton) {
        IDrawTool::CDrawToolEvent::CDrawToolEvents e = IDrawTool::CDrawToolEvent::fromQEvent(event, scene);
        toolUpdate(&e.first());
    }
}

void CPenTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, CDrawScene *scene)
{
//    if (event->button() == Qt::LeftButton) {
//        m_sPointRelease = event->scenePos();
//        //如果鼠标没有移动
//        if ( m_pPenItem != nullptr) {
//            if ( event->scenePos() == m_sPointPress ) {
//                scene->removeItem(m_pPenItem);
//                delete m_pPenItem;
//            } else {
//                m_pPenItem->drawComplete();
//                emit scene->itemAdded(m_pPenItem);
//                m_pPenItem->setSelected(true);
//                m_pPenItem->setDrawFlag(false);
//            }
//        }

//        m_pPenItem = nullptr;
//        m_bMousePress = false;
//    }
//    CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCurrentDrawToolMode(selection);
//    emit scene->signalChangeToSelect();

//   scene->mouseEvent(event);

    IDrawTool::CDrawToolEvent::CDrawToolEvents e = IDrawTool::CDrawToolEvent::fromQEvent(event, scene);
    toolFinish(&e.first());

    CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCurrentDrawToolMode(selection);
    emit scene->signalChangeToSelect();
}

void CPenTool::toolStart(IDrawTool::CDrawToolEvent *event)
{
    IDrawTool::toolStart(event);

    //qDebug() << "toolStart allStartInfo size === " << allStartInfo.size();

    SRecordedStartInfo &startedInfo = allStartInfo[event->uuid()];
    CDrawScene *scene = event->scene();
    scene->clearSelection();
    startedInfo.m_sPointPress = event->pos();
    CGraphicsPenItem *pPenItem = new CGraphicsPenItem(startedInfo.m_sPointPress);
    pPenItem->setDrawFlag(true);
    QPen pen = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getPen();
    pPenItem->setPen(pen);
    pPenItem->setBrush(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getBrush());
    pPenItem->setPenStartType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getPenStartType());
    pPenItem->setPenEndType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getPenEndType());
    pPenItem->setPixmap();
    pPenItem->setZValue(scene->getMaxZValue() + 1);
    scene->addItem(pPenItem);
    startedInfo.tempItem = pPenItem;
}

void CPenTool::toolUpdate(IDrawTool::CDrawToolEvent *event)
{
    auto it = allStartInfo.find(event->uuid());
    if (it != allStartInfo.end()) {
        CGraphicsPenItem *pPenIem = dynamic_cast<CGraphicsPenItem *>(it->tempItem);
        if (nullptr != pPenIem) {
            QPointF pointMouse = event->pos();
            bool shiftKeyPress = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getShiftKeyStatus();
            pPenIem->updatePenPath(pointMouse, shiftKeyPress);
        }
        it.value().m_sLastPress = event->pos();
    } else {
        toolStart(event);
    }
}

void CPenTool::toolFinish(IDrawTool::CDrawToolEvent *event)
{
    auto it = allStartInfo.find(event->uuid());
    if (it != allStartInfo.end()) {
        CGraphicsPenItem *pPenIem = dynamic_cast<CGraphicsPenItem *>(it->tempItem);
        CDrawScene *pScene = qobject_cast<CDrawScene *>(pPenIem->scene());
        if (nullptr != pPenIem) {
            if (event->pos() == it.value().m_sPointPress) {
                if (pScene != nullptr) {
                    pScene->removeItem(pPenIem);
                }
                delete pPenIem;
            } else {
                pPenIem->drawComplete();
                if (pScene != nullptr) {
                    emit pScene->itemAdded(pPenIem);
                }
                // [BUG 28087] 所绘制的画笔未默认呈现选中状态
                pPenIem->setSelected(true);
                pPenIem->setDrawFlag(false);
            }
        }
        it.value().m_sLastPress = event->pos();
    }
    IDrawTool::toolFinish(event);
}
