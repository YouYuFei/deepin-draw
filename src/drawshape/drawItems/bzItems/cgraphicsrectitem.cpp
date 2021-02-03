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
#include "cgraphicsrectitem.h"
#include "csizehandlerect.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"

#include <QPainter>
#include <QPixmap>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QGraphicsBlurEffect>

CGraphicsRectItem::CGraphicsRectItem(CGraphicsItem *parent)
    : CGraphicsItem(parent)
{
    CGraphicsItem::initHandle();
}

CGraphicsRectItem::CGraphicsRectItem(const QRectF &rect, CGraphicsItem *parent)
    : CGraphicsItem(parent)
{
    m_topLeftPoint = rect.topLeft();
    m_bottomRightPoint = rect.bottomRight();
    CGraphicsItem::initHandle();
}

CGraphicsRectItem::CGraphicsRectItem(qreal x, qreal y, qreal w, qreal h, CGraphicsItem *parent)
    : CGraphicsItem(parent)
{
    QRectF rect(x, y, w, h);
    rect = rect.normalized();
    m_topLeftPoint = rect.topLeft();
    m_bottomRightPoint = rect.bottomRight();
    CGraphicsItem::initHandle();
}

CGraphicsRectItem::CGraphicsRectItem(const SGraphicsRectUnitData &rectData, const SGraphicsUnitHead &head, CGraphicsItem *parent)
    : CGraphicsItem(head, parent)
{
    //    QPointF leftTop;
    //    QPointF rightBottom;

    //    if (RectType == unit.head.dataType) {
    //        leftTop = QPointF(unit.data.pRect->leftTopX, unit.data.pRect->leftTopY);
    //        rightBottom = QPointF(unit.data.pRect->rightBottomX, unit.data.pRect->rightBottomY);
    //    } else if (PictureType == unit.head.dataType) {
    //        leftTop = QPointF(unit.data.pPic->rect.leftTopX, unit.data.pPic->rect.leftTopY);
    //        rightBottom = QPointF(unit.data.pPic->rect.rightBottomX, unit.data.pPic->rect.rightBottomY);
    //    }


    this->m_topLeftPoint = rectData.topLeft;
    this->m_bottomRightPoint =  rectData.bottomRight;
    //this->setTransformOriginPoint(this->rect().center());
    this->setTransformOriginPoint(QRectF(m_topLeftPoint, m_bottomRightPoint).center());

    CGraphicsItem::initHandle();
}

CGraphicsRectItem::~CGraphicsRectItem()
{

}

int CGraphicsRectItem::type() const
{
    return RectType;
}

void CGraphicsRectItem::setRect(const QRectF &rect)
{
    prepareGeometryChange();
    m_topLeftPoint = rect.topLeft();
    m_bottomRightPoint = rect.bottomRight();
    updateHandlesGeometry();
}

void CGraphicsRectItem::setXYRedius(int xRedius, int yRedius, bool preview)
{
    if (!preview) {
        m_xRedius = xRedius;
        m_yRedius = yRedius;
    } else {
        m_rediusForPreview = xRedius;
    }
    m_isPreviewRedius = preview;
}

int CGraphicsRectItem::getXRedius()
{
    return  m_xRedius;
}

int CGraphicsRectItem::getPaintRedius()
{
    if (m_isPreviewRedius) {
        return m_rediusForPreview;
    }
    return m_xRedius;
}

void CGraphicsRectItem::loadGraphicsRectUnit(const SGraphicsRectUnitData &rectData)
{
    this->m_topLeftPoint = rectData.topLeft;
    this->m_bottomRightPoint = rectData.bottomRight;
    this->m_xRedius = rectData.xRedius;
    this->m_yRedius = rectData.yRedius;

    m_isPreviewRedius = false;

    this->setTransformOriginPoint(QRectF(m_topLeftPoint, m_bottomRightPoint).center());
}

QPainterPath CGraphicsRectItem::inSideShape() const
{
    QPainterPath path;
    path.addRoundedRect(this->rect(), m_xRedius, m_yRedius, Qt::AbsoluteSize);
    return path;
}

qreal CGraphicsRectItem::incLength() const
{
    return 0;
}

void CGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    beginCheckIns(painter);

    const QPen curPen = this->paintPen();

    int redius = getPaintRedius();

    //先绘制填充
    qreal penWidthOffset = curPen.widthF() / 2.0;
    QRectF rectIn = QRectF(rect().topLeft() + QPointF(penWidthOffset, penWidthOffset),
                           rect().size() - QSizeF(2 * penWidthOffset, 2 * penWidthOffset));

    painter->setPen(Qt::NoPen);
    painter->setBrush(paintBrush());
    painter->drawRoundedRect(rectIn, qMax(redius - penWidthOffset, 0.0), qMax(redius - penWidthOffset, 0.0), Qt::AbsoluteSize);

    //再绘制描边
    painter->setPen(curPen.width() == 0 ? Qt::NoPen : curPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rect(), redius, redius, Qt::AbsoluteSize);

    endCheckIns(painter);

    //224,224,224
    paintMutBoundingLine(painter, option);
}

void CGraphicsRectItem::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point)
{
    bool shiftKeyPress = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getShiftKeyStatus();
    bool altKeyPress = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getAltKeyStatus();
    resizeTo(dir, point, shiftKeyPress, altKeyPress);

}

void CGraphicsRectItem::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point,
                                 bool bShiftPress, bool bAltPress)
{
    QPointF local = mapFromScene(point);
    QRectF rect = this->rect();
    QPointF topLeft = rect.topLeft();
    QPointF centerPoint = rect.center();
    qreal w = rect.width();
    qreal h = rect.height();
    qreal scale = w / h;//长宽比

    bool shiftKeyPress = bShiftPress;
    bool altKeyPress = bAltPress;

    if (!shiftKeyPress && !altKeyPress) {
        switch (dir) {
        case CSizeHandleRect::Right:
            if (local.x() - rect.left() > 0.1) {
                rect.setRight(local.x());
            }
            break;
        case CSizeHandleRect::RightTop:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.bottom() < 0.1) {
                rect.setTopRight(local);
            }
            break;
        case CSizeHandleRect::RightBottom:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.top() > 0.1) {
                rect.setBottomRight(local);
            }
            break;
        case CSizeHandleRect::LeftBottom:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.top() > 0.1) {
                rect.setBottomLeft(local);
            }
            break;
        case CSizeHandleRect::Bottom:
            if (local.y() - rect.top() > 0.1) {
                rect.setBottom(local.y());
            }
            break;
        case CSizeHandleRect::LeftTop:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.bottom() < 0.1) {
                rect.setTopLeft(local);
            }
            break;
        case CSizeHandleRect::Left:
            if (rect.right() - local.x() > 0.1) {
                rect.setLeft(local.x());
            }
            break;
        case CSizeHandleRect::Top:
            if (local.y() - rect.bottom() < 0.1) {
                rect.setTop(local.y());
            }
            break;
        default:
            break;
        }
    }
    //按住SHIFT等比拉伸
    else if ((shiftKeyPress && !altKeyPress)) {
        switch (dir) {
        case CSizeHandleRect::Right:
            if (local.x() - rect.left() > 0.1) {
                //变换后的宽度和高度
                qreal w2 = local.x() - rect.left();
                qreal h2 = w2 / scale;
                topLeft.setY(topLeft.y() - (h2 - h) / 2);
                rect.setTopLeft(topLeft);
                rect.setWidth(w2);
                rect.setHeight(h2);
            }
            break;
        case CSizeHandleRect::RightTop:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.bottom() < 0.1) {
                //rect.setTopRight(local);
                QPointF bottomLeft = rect.bottomLeft();
                qreal w2 = local.x() - bottomLeft.x();
                qreal h2 = bottomLeft.y() - local.y();

                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                topLeft.setY(topLeft.y() - (h2 - h));

                rect.setTopLeft(topLeft);
                rect.setWidth(w2);
                rect.setHeight(h2);
            }
            break;
        case CSizeHandleRect::RightBottom:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.top() > 0.1) {
                //rect.setBottomRight(local);

                qreal w2 = local.x() - topLeft.x();
                qreal h2 = local.y() - topLeft.y();
                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                rect.setWidth(w2);
                rect.setHeight(h2);
            }
            break;
        case CSizeHandleRect::LeftBottom:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.top() > 0.1) {
                //rect.setBottomLeft(local);
                QPointF topRight = rect.topRight();
                qreal w2 = topRight.x() - local.x();
                qreal h2 = local.y() - topRight.y();
                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                topLeft.setX(topRight.x() - w2);
                rect.setTopLeft(topLeft);
                rect.setWidth(w2);
                rect.setHeight(h2);

            }
            break;
        case CSizeHandleRect::Bottom:
            if (local.y() - rect.top() > 0.1) {
                //rect.setBottom(local.y());
                qreal h2 = local.y() - topLeft.y();
                qreal w2 = h2 * scale;
                topLeft.setX(topLeft.x() - (w2 - w) / 2);
                rect.setTopLeft(topLeft);
                rect.setWidth(w2);
                rect.setHeight(h2);
            }
            break;
        case CSizeHandleRect::LeftTop:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.bottom() < 0.1) {
                //rect.setTopLeft(local);
                QPointF bottomRight = rect.bottomRight();
                qreal w2 = bottomRight.x() - local.x();
                qreal h2 = bottomRight.y() - local.y();
                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                topLeft = bottomRight - QPointF(w2, h2);
                rect.setTopLeft(topLeft);
            }
            break;
        case CSizeHandleRect::Left:
            if (rect.right() - local.x() > 0.1) {
                //rect.setLeft(local.x());
                qreal w2 = rect.right() - local.x();
                qreal h2 = w2 / scale;
                topLeft.setX(local.x());
                topLeft.setY(topLeft.y() - (h2 - h) / 2);
                rect.setTopLeft(topLeft);
                rect.setWidth(w2);
                rect.setHeight(h2);
            }
            break;
        case CSizeHandleRect::Top:
            if (local.y() - rect.bottom() < 0.1) {
                //rect.setTop(local.y());

                qreal h2 = rect.bottom() - local.y();
                qreal w2 = h2 * scale;
                topLeft.setX(topLeft.x() - (w2 - w) / 2);
                topLeft.setY(local.y());
                rect.setTopLeft(topLeft);
                rect.setWidth(w2);
                rect.setHeight(h2);
            }
            break;
        default:
            break;
        }
    }
    //中心拉伸
    else if ((!shiftKeyPress && altKeyPress)) {
        switch (dir) {
        case CSizeHandleRect::Right:
            if (local.x() - rect.left() > 0.1) {
                //rect.setRight(local.x());
                rect.setRight(local.x());
                rect.setLeft(centerPoint.x() * 2 - local.x());
            }
            break;
        case CSizeHandleRect::RightTop:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.bottom() < 0.1) {
                rect.setTopRight(local);
                rect.setBottomLeft(2 * centerPoint - local);
            }
            break;
        case CSizeHandleRect::RightBottom:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.top() > 0.1) {
                rect.setBottomRight(local);
                rect.setTopLeft(2 * centerPoint - local);
            }
            break;
        case CSizeHandleRect::LeftBottom:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.top() > 0.1) {
                rect.setBottomLeft(local);
                rect.setTopRight(2 * centerPoint - local);
            }
            break;
        case CSizeHandleRect::Bottom:
            if (local.y() - rect.top() > 0.1) {
                //rect.setBottom(local.y());
                rect.setBottom(local.y());
                rect.setTop(centerPoint.y() * 2 - local.y());
            }
            break;
        case CSizeHandleRect::LeftTop:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.bottom() < 0.1) {
                rect.setTopLeft(local);
                rect.setBottomRight(2 * centerPoint - local);
            }
            break;
        case CSizeHandleRect::Left:
            if (rect.right() - local.x() > 0.1) {
                rect.setLeft(local.x());
                rect.setRight(2 * centerPoint.x() - local.x());
            }
            break;
        case CSizeHandleRect::Top:
            if (local.y() - rect.bottom() < 0.1) {
                rect.setTop(local.y());
                rect.setBottom(2 * centerPoint.y() - local.y());
            }
            break;
        default:
            break;
        }
    }
    //等比中心拉伸
    else if ((shiftKeyPress && altKeyPress)) {
        switch (dir) {
        case CSizeHandleRect::Right:
            if (local.x() - rect.left() > 0.1) {
                //变换后的宽度和高度
                qreal h2 = local.x() - centerPoint.x();
                qreal w2 = h2 * scale;
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::RightTop:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.bottom() < 0.1) {
                qreal w2 = local.x() - centerPoint.x();
                qreal h2 = centerPoint.y() - local.y();

                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::RightBottom:
            if (local.x() - rect.left() > 0.1 && local.y() - rect.top() > 0.1) {
                //rect.setBottomRight(local);

                qreal w2 = local.x() - centerPoint.x();
                qreal h2 = local.y() - centerPoint.y();
                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::LeftBottom:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.top() > 0.1) {

                qreal w2 = centerPoint.x() - local.x();
                qreal h2 = local.y() - centerPoint.y();
                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::Bottom:
            if (local.y() - rect.top() > 0.1) {
                qreal h2 = local.y() - centerPoint.y();
                qreal w2 = h2 * scale;
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::LeftTop:
            if (local.x() - rect.right() < 0.1 && local.y() - rect.bottom() < 0.1) {
                qreal w2 = centerPoint.x() - local.x();
                qreal h2 = centerPoint.y() - local.y();
                if (h2 * scale >= w2) {
                    w2 = h2 * scale;
                } else {
                    h2 = w2 / scale;
                }
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::Left:
            if (rect.right() - local.x() > 0.1) {
                qreal w2 = centerPoint.x() - local.x();
                qreal h2 = w2 / scale;
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        case CSizeHandleRect::Top:
            if (local.y() - rect.bottom() < 0.1) {
                qreal h2 = centerPoint.y() - local.y();
                qreal w2 = h2 * scale;
                QPointF topLeft = centerPoint - QPointF(w2, h2);
                QPointF rightBottom = centerPoint + QPointF(w2, h2);
                rect = QRectF(topLeft, rightBottom);
            }
            break;
        default:
            break;
        }
    }

    rect = rect.normalized();
    centerPoint = mapToScene(rect.center());
    rect.setRect(centerPoint.rx() - rect.width() * 0.5, centerPoint.ry() - rect.height() * 0.5, rect.width(), rect.height());
    prepareGeometryChange();
    setTransformOriginPoint(centerPoint);
    this->setPos(0, 0);

    this->setRect(rect);

    updateHandlesGeometry();
}

void CGraphicsRectItem::resizeToMul(CSizeHandleRect::EDirection dir,
                                    const QPointF &offset,
                                    const double &xScale, const double &yScale,
                                    bool bShiftPress, bool bAltPress)
{
    Q_UNUSED(bShiftPress);
    Q_UNUSED(bAltPress);

    QRectF rect = this->rect();
    QPointF bottomRight = rect.bottomRight();
    QPointF topLeft = rect.topLeft();
    QPointF topRight = rect.topRight();
    QPointF bottomLeft = rect.bottomLeft();
    switch (dir) {
    case CSizeHandleRect::LeftTop:
        //qDebug() << "CSizeHandleRect::LeftTop" <<  endl;
        topLeft.setX(topLeft.x() + rect.width() * xScale);
        topLeft.setY(topLeft.y() + rect.height() * yScale);
        rect.setTopLeft(topLeft);
        break;
    case CSizeHandleRect::Top:
        //qDebug() << "CSizeHandleRect::Top" <<  endl;
        topLeft.setX(topLeft.x() + rect.width() * xScale);
        topLeft.setY(topLeft.y() + rect.height() * yScale);
        rect.setTopLeft(topLeft);
        break;
    case CSizeHandleRect::RightTop:
        //qDebug() << "CSizeHandleRect::RightTop" <<  endl;
        topRight.setX(topRight.x() + rect.width() * xScale);
        topRight.setY(topRight.y() + rect.height() * yScale);
        rect.setTopRight(topRight);
        break;
    case CSizeHandleRect::Right:
        //qDebug() << "CSizeHandleRect::Right" <<  endl;
        bottomRight.setX(bottomRight.x() + rect.width()*xScale);
        bottomRight.setY(bottomRight.y() + rect.height()*yScale);
        rect.setBottomRight(bottomRight);
        break;
    case CSizeHandleRect::RightBottom:
        //qDebug() << "CSizeHandleRect::RightBottom" <<  endl;
        bottomRight.setX(bottomRight.x() + rect.width()*xScale);
        bottomRight.setY(bottomRight.y() + rect.height()*yScale);
        rect.setBottomRight(bottomRight);
        break;
    case CSizeHandleRect::Bottom:
        //qDebug() << "CSizeHandleRect::Bottom" <<  endl;
        bottomRight.setX(bottomRight.x() + rect.width()*xScale);
        bottomRight.setY(bottomRight.y() + rect.height()*yScale);
        rect.setBottomRight(bottomRight);
        break;
    case CSizeHandleRect::LeftBottom:
        //qDebug() << "CSizeHandleRect::LeftBottom" <<  endl;
        bottomLeft.setX(bottomLeft.x() + rect.width() * xScale);
        bottomLeft.setY(bottomLeft.y() + rect.height()*yScale);
        rect.setBottomLeft(bottomLeft);
        break;
    case CSizeHandleRect::Left:
        //qDebug() << "CSizeHandleRect::Left" <<  endl;
        topLeft.setX(topLeft.x() + rect.width() * xScale);
        topLeft.setY(topLeft.y() + rect.height() * yScale);
        rect.setTopLeft(topLeft);
        break;
    default:
        break;
    }

    this->setRect(rect);
    this->setTransformOriginPoint(rect.center());
    this->moveBy(offset.x(), offset.y());
    updateHandlesGeometry();
}

CGraphicsItem *CGraphicsRectItem::duplicateCreatItem()
{
    return new CGraphicsRectItem();
}

void CGraphicsRectItem::duplicate(CGraphicsItem *item)
{
    static_cast<CGraphicsRectItem * >(item)->setRect(this->rect());
    static_cast<CGraphicsRectItem * >(item)->setXYRedius(m_xRedius, m_yRedius);
    CGraphicsItem::duplicate(item);
}

void CGraphicsRectItem::loadGraphicsUnit(const CGraphicsUnit &data, bool allInfo)
{
    Q_UNUSED(allInfo)
    if (data.data.pRect != nullptr) {
        loadGraphicsRectUnit(*data.data.pRect);
    }
    loadHeadData(data.head);
}

CGraphicsUnit CGraphicsRectItem::getGraphicsUnit(bool all) const
{
    Q_UNUSED(all)

    CGraphicsUnit unit;

    unit.head.dataType = this->type();
    unit.head.dataLength = sizeof(SGraphicsRectUnitData);
    unit.head.pen = this->pen();
    unit.head.brush = this->brush();
    unit.head.pos = this->pos();
    unit.head.rotate = this->rotation();
    unit.head.zValue = this->zValue();

    unit.data.pRect = new SGraphicsRectUnitData();
    unit.data.pRect->topLeft = this->m_topLeftPoint;
    unit.data.pRect->bottomRight = this->m_bottomRightPoint;
    unit.data.pRect->xRedius = this->m_xRedius;
    unit.data.pRect->yRedius = this->m_yRedius;

    return unit;
}

QRectF CGraphicsRectItem::rect() const
{
    return QRectF(m_topLeftPoint, m_bottomRightPoint);
}
