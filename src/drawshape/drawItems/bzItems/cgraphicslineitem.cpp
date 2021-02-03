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
#include "cgraphicslineitem.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"

#include <DSvgRenderer>

#include <QPen>
#include <QPainter>
#include <QPointF>
#include <QtMath>
#include <QDebug>

DTK_USE_NAMESPACE

CGraphicsLineItem::CGraphicsLineItem(QGraphicsItem *parent)
    : CGraphicsItem(parent)
    , m_startType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineStartType())
    , m_endType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineEndType())
{
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(const QLineF &line, QGraphicsItem *parent)
    : CGraphicsItem(parent)
    , m_startType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineStartType())
    , m_endType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineEndType())
{
    m_line = line;
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent)
    : CGraphicsItem(parent)
    , m_startType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineStartType())
    , m_endType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineEndType())
{
    setLine(p1.x(), p1.y(), p2.x(), p2.y(), true);
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
    : CGraphicsItem(parent)
    , m_line(x1, y1, x2, y2)
    , m_startType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineStartType())
    , m_endType(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getLineEndType())
{
    initLine();
}

CGraphicsLineItem::CGraphicsLineItem(const SGraphicsLineUnitData *data, const SGraphicsUnitHead &head, CGraphicsItem *parent)
    : CGraphicsItem(head, parent)
{
    setLine(data->point1, data->point2, true);
    m_startType = static_cast<ELineType>(data->start_type);
    m_endType = static_cast<ELineType>(data->end_type);
    initLine();
}

CGraphicsLineItem::~CGraphicsLineItem()
{

}

int CGraphicsLineItem::type() const
{
    return LineType;
}

QPainterPath CGraphicsLineItem::shape() const
{
    return CGraphicsItem::shape();
}

QRectF CGraphicsLineItem::boundingRect() const
{
    return shape().controlPointRect();
}

QRectF CGraphicsLineItem::rect() const
{
    QPainterPath path(m_dRectline.p1());
    path.lineTo(m_dRectline.p2());
    path.addPath(m_startPath);
    path.addPath(m_endPath);

    return path.controlPointRect().normalized();
}

void CGraphicsLineItem::rotatAngle(qreal angle)
{
    QPointF center = this->rect().center();

    this->setTransformOriginPoint(center);

    if (angle > 360) {
        angle -= 360;
    }

    QLineF line = static_cast<CGraphicsLineItem *>(this)->line();
    QPointF vector = line.p2() - line.p1();
    qreal oriangle = 0;
    if (vector.x() - 0 < 0.0001 && vector.x() - 0 > -0.0001) {
        if (line.p2().y() - line.p1().y() > 0.0001) {
            oriangle = 90;
        } else {
            oriangle = -90;
        }
    } else {
        oriangle = (-atan(vector.y() / vector.x())) * 180 / 3.14159 + 180;
    }
    angle = angle - oriangle;
    if (angle > 360) {
        angle -= 360;
    }
    this->setRotation(angle);
}

void CGraphicsLineItem::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point)
{
    bool shiftKeyPress = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getShiftKeyStatus();

    if (!shiftKeyPress) {
        QPointF local = mapFromScene(point);
        QPointF p1;
        QPointF p2;
        if (dir == CSizeHandleRect::LeftTop) {
            p1 = local;
            p2 = m_line.p2();
        } else {
            p1 = m_line.p1();
            p2 = local;
        }

        p1 = mapToScene(p1);
        p2 = mapToScene(p2);
        setRotation(0);
        setPos(0, 0);
        setLine(p1, p2);
    } else {
        QPointF local = mapFromScene(point);
        QPointF p1;
        QPointF p2;

        if (dir == CSizeHandleRect::LeftTop) {
            p1 = local;
            p2 = m_line.p2();
            QLineF v = QLineF(p1, p2);
            if (fabs(v.dx()) - fabs(v.dy()) > 0.0001) {
                p1.setY(p2.y());
            } else {
                p1.setX(p2.x());
            }
        } else {
            p1 = m_line.p1();
            p2 = local;

            QLineF v = QLineF(p1, p2);
            if (fabs(v.dx()) - fabs(v.dy()) > 0.0001) {
                p2.setY(p1.y());
            } else {
                p2.setX(p1.x());
            }
        }

        p1 = mapToScene(p1);
        p2 = mapToScene(p2);
        setRotation(0);
        setPos(0, 0);
        setLine(p1, p2);
    }
}

void CGraphicsLineItem::resizeTo(CSizeHandleRect::EDirection dir,
                                 const QPointF &point,
                                 bool bShiftPress, bool bAltPress)
{
    Q_UNUSED(bShiftPress)
    Q_UNUSED(bAltPress)

    resizeTo(dir, point);
}

void CGraphicsLineItem::resizeToMul(CSizeHandleRect::EDirection dir, const QPointF &offset, const double &xScale, const double &yScale, bool bShiftPress, bool bAltPress)
{
//    bool shiftKeyPress = bShiftPress;
//    bool altKeyPress = bAltPress;
    Q_UNUSED(bShiftPress)
    Q_UNUSED(bAltPress)

    //QRectF rect = this->rect();

    QRectF rect = QRectF(QPointF(qMin(m_line.p1().x(), m_line.p2().x()), qMin(m_line.p1().y(), m_line.p2().y())),
                         QPointF(qMax(m_line.p1().x(), m_line.p2().x()), qMax(m_line.p1().y(), m_line.p2().y())));

    QPointF bottomRight = rect.bottomRight();
    QPointF topLeft = rect.topLeft();
    QPointF topRight = rect.topRight();
    QPointF bottomLeft = rect.bottomLeft();

    QPointF p1;
    QPointF p2;
    p2 = m_line.p2();
    p1 = m_line.p1();

    //if (!shiftKeyPress && !altKeyPress) {
    switch (dir) {
    case CSizeHandleRect::LeftTop:
        topLeft.setX(topLeft.x() + rect.width() * xScale);
        topLeft.setY(topLeft.y() + rect.height() * yScale);
        rect.setTopLeft(topLeft);
        break;
    case CSizeHandleRect::Top:
        topLeft.setX(topLeft.x() + rect.width() * xScale);
        topLeft.setY(topLeft.y() + rect.height() * yScale);
        rect.setTopLeft(topLeft);
        break;
    case CSizeHandleRect::RightTop:
        topRight.setX(topRight.x() + rect.width() * xScale);
        topRight.setY(topRight.y() + rect.height() * yScale);
        rect.setTopRight(topRight);
        break;
    case CSizeHandleRect::Right:
        bottomRight.setX(bottomRight.x() + rect.width()*xScale);
        bottomRight.setY(bottomRight.y() + rect.height()*yScale);
        rect.setBottomRight(bottomRight);
        break;
    case CSizeHandleRect::RightBottom:
        bottomRight.setX(bottomRight.x() + rect.width()*xScale);
        bottomRight.setY(bottomRight.y() + rect.height()*yScale);
        rect.setBottomRight(bottomRight);
        break;
    case CSizeHandleRect::Bottom:
        bottomRight.setX(bottomRight.x() + rect.width()*xScale);
        bottomRight.setY(bottomRight.y() + rect.height()*yScale);
        rect.setBottomRight(bottomRight);
        break;
    case CSizeHandleRect::LeftBottom:
        bottomLeft.setX(bottomLeft.x() + rect.width() * xScale);
        bottomLeft.setY(bottomLeft.y() + rect.height()*yScale);
        rect.setBottomLeft(bottomLeft);
        break;
    case CSizeHandleRect::Left:
        topLeft.setX(topLeft.x() + rect.width() * xScale);
        topLeft.setY(topLeft.y() + rect.height() * yScale);
        rect.setTopLeft(topLeft);
        break;
    default:
        break;
    }
    //}
    //setLine(p1, p2);
    if (p1.x() <= p2.x()) {
        if (p1.y() <= p2.y()) {
            p1 = rect.topLeft();
            p2 = rect.bottomRight();
        } else {
            p1 = rect.bottomLeft();
            p2 = rect.topRight();
        }
    } else {
        if (p1.y() <= p2.y()) {
            p1 = rect.topRight();
            p2 = rect.bottomLeft();
        } else {
            p1 = rect.bottomRight();
            p2 = rect.topLeft();
        }
    }

    setLine(p1, p2);
    this->moveBy(offset.x(), offset.y());
    updateHandlesGeometry();
}

QLineF CGraphicsLineItem::line() const
{
    return m_line;
}

void CGraphicsLineItem::setLine(const QLineF &line, bool init)
{
    prepareGeometryChange();
    m_line = line;
    calcVertexes();

    if (init) {
        //CGraphicsItem::updateGeometry();
    } else {
        updateHandlesGeometry();
    }
}

void CGraphicsLineItem::setLine(const QPointF &p1, const QPointF &p2, bool init)
{
    setLine(p1.x(), p1.y(), p2.x(), p2.y(), init);
}

void CGraphicsLineItem::setLine(qreal x1, qreal y1, qreal x2, qreal y2, bool init)
{
    setLine(QLineF(x1, y1, x2, y2), init);
}

CGraphicsItem *CGraphicsLineItem::duplicateCreatItem()
{
    return new CGraphicsLineItem;
}

void CGraphicsLineItem::duplicate(CGraphicsItem *item)
{
    static_cast<CGraphicsLineItem *>(item)->setLine(this->m_line);
    static_cast<CGraphicsLineItem *>(item)->setPen(this->pen());
    static_cast<CGraphicsLineItem *>(item)->setLineStartType(this->getLineStartType());
    static_cast<CGraphicsLineItem *>(item)->setLineEndType(this->getLineEndType());
    CGraphicsItem::duplicate(item);
}

CGraphicsUnit CGraphicsLineItem::getGraphicsUnit(bool all) const
{
    Q_UNUSED(all)
    CGraphicsUnit unit;

    unit.head.dataType = this->type();
    unit.head.dataLength = sizeof(SGraphicsLineUnitData);
    unit.head.pen = this->pen();
    unit.head.brush = this->brush();
    unit.head.pos = this->pos();
    unit.head.rotate = this->rotation();
    unit.head.zValue = this->zValue();

    unit.data.pLine = new SGraphicsLineUnitData();
    unit.data.pLine->point1 = this->line().p1();
    unit.data.pLine->point2 = this->line().p2();
    unit.data.pLine->start_type = m_startType;
    unit.data.pLine->end_type = m_endType;

    return  unit;
}

void CGraphicsLineItem::loadGraphicsUnit(const CGraphicsUnit &data, bool allInfo)
{
    Q_UNUSED(allInfo)
    if (data.data.pLine != nullptr) {
        m_startType = data.data.pLine->start_type;
        m_endType = data.data.pLine->end_type;
        m_line = QLineF(data.data.pLine->point1, data.data.pLine->point2);
    }
    loadHeadData(data.head);

    calcVertexes();
    updateHandlesGeometry();
}

//int CGraphicsLineItem::getQuadrant() const
//{
//    int nRet = 1;
//    if (m_line.p2().x() - m_line.p1().x() > 0.0001 && m_line.p2().y() - m_line.p1().y() < 0.0001) {
//        nRet = 1;
//    } else if (m_line.p2().x() - m_line.p1().x() > 0.0001 && m_line.p2().y() - m_line.p1().y() > 0.0001) {
//        nRet = 2;
//    } else if (m_line.p2().x() - m_line.p1().x() < 0.0001 && m_line.p2().y() - m_line.p1().y() > 0.0001) {
//        nRet = 3;
//    } else if (m_line.p2().x() - m_line.p1().x() < 0.0001 && m_line.p2().y() - m_line.p1().y() < 0.0001) {
//        nRet = 4;
//    }
//    return nRet;
//}

void CGraphicsLineItem::setLineStartType(ELineType type)
{
    m_startType = type;
    updateShape();
}

ELineType CGraphicsLineItem::getLineStartType() const
{
    return m_startType;
}

void CGraphicsLineItem::setLineEndType(ELineType type)
{
    m_endType = type;
    updateShape();
}

ELineType CGraphicsLineItem::getLineEndType() const
{
    return m_endType;
}

void CGraphicsLineItem::updateHandlesGeometry()
{
    qreal penwidth = this->pen().widthF();
    for (Handles::iterator it = m_handles.begin(); it != m_handles.end(); ++it) {
        CSizeHandleRect *hndl = *it;

        if (!this->isSelected() || this->getMutiSelect()) {
            hndl->hide();
            continue;
        }

        hndl->show();

        qreal w = hndl->boundingRect().width();
        qreal h = hndl->boundingRect().height();
        switch (hndl->dir()) {
        case CSizeHandleRect::LeftTop:
            hndl->move(m_dRectline.p1().x() - w / 2, m_dRectline.p1().y() - h / 2);
            break;
        case CSizeHandleRect::RightBottom:
            hndl->move(m_dRectline.p2().x() - w / 2, m_dRectline.p2().y() - h / 2);
            break;
        case CSizeHandleRect::Rotation: {
            // 以下代码没有实际作用
            QPointF centerPos = (m_dRectline.p1() + m_dRectline.p2()) / 2;
            qreal k = 0;
            qreal ang = 0;

            if (qAbs(m_dRectline.p2().x() - m_dRectline.p1().x()) < 0.0001) {
                hndl->move(m_dRectline.p1().x() - h - penwidth, centerPos.y());
            } else {
                k = -(m_dRectline.p2().y() - m_dRectline.p1().y()) / (m_dRectline.p2().x() - m_dRectline.p1().x());
                ang = atan(k);

                //增加线宽的长度防止缩放造成位置不正确
                qreal x = qAbs((h + penwidth) * sin(ang));
                qreal y = qAbs((h + penwidth) * cos(ang));
                //第一象限
                if (m_dRectline.p2().x() - m_dRectline.p1().x() > 0.0001 && m_dRectline.p2().y() - m_dRectline.p1().y() < 0.0001) {
                    hndl->move(centerPos.x() - w / 2 - x, centerPos.y() - h / 2 - y);
                } else if (m_dRectline.p2().x() - m_dRectline.p1().x() > 0.0001 && m_dRectline.p2().y() - m_dRectline.p1().y() > 0.0001) {
                    hndl->move(centerPos.x() - w / 2 + x, centerPos.y() - h / 2 - y);
                } else if (m_dRectline.p2().x() - m_dRectline.p1().x() < 0.0001 && m_dRectline.p2().y() - m_dRectline.p1().y() > 0.0001) {
                    hndl->move(centerPos.x() - w / 2 + x, centerPos.y() - h / 2 + y);
                } else if (m_dRectline.p2().x() - m_dRectline.p1().x() < 0.0001 && m_dRectline.p2().y() - m_dRectline.p1().y() < 0.0001) {
                    hndl->move(centerPos.x() - w / 2 - x, centerPos.y() - h / 2 + y);
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

void CGraphicsLineItem::updateShape()
{
    calcVertexes();
    CGraphicsItem::updateShape();
}

void CGraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    updateHandlesGeometry();

    painter->setRenderHint(QPainter::Antialiasing, true);

    const QPen curPen = paintPen();
    painter->setPen(curPen.width() == 0 ? Qt::NoPen : curPen);

    beginCheckIns(painter);

    painter->setBrush(Qt::NoBrush);
    if (curPen.width()) {
        if (m_startType == soildArrow || m_startType == soildRing) {
            painter->setBrush(QBrush(QColor(curPen.color())));
        }
    }
    painter->drawPath(m_startPath);

    painter->setBrush(Qt::NoBrush);
    if (curPen.width()) {
        if (m_endType == soildArrow || m_endType == soildRing) {
            painter->setBrush(QBrush(QColor(curPen.color())));
        }
    }
    painter->drawPath(m_endPath);

    painter->drawLine(m_line);

    endCheckIns(painter);
}

QVariant CGraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        updateHandlesGeometry();
    }
    return CGraphicsItem::itemChange(change, value);
}

bool CGraphicsLineItem::isPosPenetrable(const QPointF &posLocal)
{
    Q_UNUSED(posLocal)

    return false;
}

void CGraphicsLineItem::initLine()
{
    //CGraphicsItem::initHandle();

    updateHandlesGeometry();

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    calcVertexes();
}

void CGraphicsLineItem::initHandle()
{
    clearHandle();

    m_handles.reserve(CSizeHandleRect::None);

    m_handles.push_back(new CSizeHandleRect(this, CSizeHandleRect::LeftTop));
    m_handles.push_back(new CSizeHandleRect(this, CSizeHandleRect::RightBottom));

    updateHandlesGeometry();

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    calcVertexes();
}

void CGraphicsLineItem::drawStart()
{
    QPointF prePoint = m_line.p1();
    QPointF currentPoint = m_line.p2();
    if (prePoint == currentPoint) {
        return;
    }

    QLineF templine(m_line.p2(), m_line.p1());

    QLineF v = templine.unitVector();
    v.setLength(10 + pen().width() * 3); //改变单位向量的大小，实际就是改变箭头长度
    v.translate(QPointF(templine.dx(), templine.dy()));

    QLineF n = v.normalVector(); //法向量
    n.setLength(n.length() * 0.5); //这里设定箭头的宽度
    QLineF n2 = n.normalVector().normalVector(); //两次法向量运算以后，就得到一个反向的法向量

    QPointF p1 = v.p2();
    QPointF p2 = n.p2();
    QPointF p3 = n2.p2();

    //减去一个箭头的宽度
    QPointF diffV = p1 - m_line.p1();
    p1 -= diffV;
    p2 -= diffV;
    p3 -= diffV;

    switch (m_startType) {
    case noneLine: {
        m_startPath = QPainterPath(p1);
        m_dRectline.setP1(p1);
        break;
    }
    case normalArrow: {
        p1 += diffV;
        p2 += diffV;
        p3 += diffV;
        m_startPath = QPainterPath(p1);
        m_startPath.lineTo(p2);
        m_startPath.moveTo(p1);
        m_startPath.lineTo(p3);
        m_startPath.moveTo(p1);
        QPointF center = (p2 + p3) / 2;
        m_startPath.lineTo(center);
        m_startPath.moveTo(p1);
        m_dRectline.setP1(p1);
        break;
    }
    case soildArrow: {
        p1 += diffV;
        p2 += diffV;
        p3 += diffV;
        m_startPath = QPainterPath(p1);
        m_startPath.lineTo(p3);
        m_startPath.lineTo(p2);
        m_startPath.lineTo(p1);
        m_dRectline.setP1(p1);
        break;
    }
    case normalRing: {
        qreal radioWidth = this->pen().width() * 2;
        QPointF center;
        qreal yOff = qSin(m_line.angle() / 180 * M_PI) * radioWidth;
        qreal xOff = qCos(m_line.angle() / 180 * M_PI) * radioWidth;
        center = m_line.p1() + QPointF(-xOff, yOff);
        QRectF ecliRect(center + QPointF(-radioWidth, -radioWidth), QSizeF(2 * radioWidth, 2 * radioWidth));
        m_startPath = QPainterPath(center + QPointF(radioWidth, 0));
        m_startPath.arcTo(ecliRect, 0, 360);
        m_dRectline.setP1(center + QPointF(-xOff, yOff));
        break;
    }
    case soildRing: {
        qreal radioWidth = this->pen().width() * 2;
        QPointF center;
        qreal yOff = qSin(m_line.angle() / 180 * M_PI) * radioWidth;
        qreal xOff = qCos(m_line.angle() / 180 * M_PI) * radioWidth;
        center = m_line.p1() + QPointF(-xOff, yOff);
        QRectF ecliRect(center + QPointF(-radioWidth, -radioWidth), QSizeF(2 * radioWidth, 2 * radioWidth));
        m_startPath = QPainterPath(center + QPointF(radioWidth, 0));
        m_startPath.arcTo(ecliRect, 0, 360);
        m_dRectline.setP1(center + QPointF(-xOff, yOff));
        break;
    }
    }
}

void CGraphicsLineItem::drawEnd()
{
    QPointF prePoint = m_line.p1();
    QPointF currentPoint = m_line.p2();
    if (prePoint == currentPoint) {
        return;
    }

    QLineF v = m_line.unitVector();
    v.setLength(10 + pen().width() * 3); //改变单位向量的大小，实际就是改变箭头长度
    v.translate(QPointF(m_line.dx(), m_line.dy()));

    QLineF n = v.normalVector(); //法向量
    n.setLength(n.length() * 0.5); //这里设定箭头的宽度
    QLineF n2 = n.normalVector().normalVector(); //两次法向量运算以后，就得到一个反向的法向量

    QPointF p1 = v.p2();
    QPointF p2 = n.p2();
    QPointF p3 = n2.p2();

    //减去一个箭头的宽度
    QPointF diffV = p1 - m_line.p2();
    //    QPointF diffV = p1 - m_line.p1();
    p1 -= diffV;
    p2 -= diffV;
    p3 -= diffV;

    // 绘制终点
    switch (m_endType) {
    case noneLine: {
        m_endPath = QPainterPath(p1);
        m_dRectline.setP2(p1);
        break;
    }
    case normalArrow: {
        p1 += diffV;
        p2 += diffV;
        p3 += diffV;
        m_endPath = QPainterPath(p1);
        m_endPath.lineTo(p2);
        m_endPath.moveTo(p1);
        m_endPath.lineTo(p3);
        m_endPath.moveTo(p1);
        QPointF center = (p2 + p3) / 2;
        m_endPath.lineTo(center);
        m_endPath.moveTo(p1);
        m_dRectline.setP2(p1);
        break;
    }
    case soildArrow: {
        p1 += diffV;
        p2 += diffV;
        p3 += diffV;
        m_endPath = QPainterPath(p1);
        m_endPath.lineTo(p3);
        m_endPath.lineTo(p2);
        m_endPath.lineTo(p1);
        m_dRectline.setP2(p1);
        break;
    }
    case normalRing: {
        qreal radioWidth = this->pen().width() * 2;
        QPointF center;
        qreal yOff = qSin(m_line.angle() / 180 * M_PI) * radioWidth;
        qreal xOff = qCos(m_line.angle() / 180 * M_PI) * radioWidth;
        center = m_line.p2() + QPointF(xOff, -yOff);
        QRectF ecliRect(center + QPointF(-radioWidth, -radioWidth), QSizeF(2 * radioWidth, 2 * radioWidth));
        m_endPath = QPainterPath(center + QPointF(radioWidth, 0));
        m_endPath.arcTo(ecliRect, 0, 360);
        m_dRectline.setP2(center + + QPointF(xOff, -yOff));
        break;
    }
    case soildRing: {
        qreal radioWidth = this->pen().width() * 2;
        QPointF center;
        qreal yOff = qSin(m_line.angle() / 180 * M_PI) * radioWidth;
        qreal xOff = qCos(m_line.angle() / 180 * M_PI) * radioWidth;
        center = m_line.p2() + QPointF(xOff, -yOff);
        QRectF ecliRect(center + QPointF(-radioWidth, -radioWidth), QSizeF(2 * radioWidth, 2 * radioWidth));
        m_endPath = QPainterPath(center + QPointF(radioWidth, 0));
        m_endPath.arcTo(ecliRect, 0, 360);
        m_dRectline.setP2(center + + QPointF(xOff, -yOff));
        break;
    }
    }
}

void CGraphicsLineItem::calcVertexes()
{
    prepareGeometryChange();

    // 绘制起点
    drawStart();

    // 绘制终点
    drawEnd();

    updateHandlesGeometry();

    // 更新画布区域
    if (scene() != nullptr)
        scene()->views().first()->viewport()->update();
}

QPainterPath CGraphicsLineItem::getHighLightPath()
{
    QPainterPath path(m_line.p1());
    path.lineTo(m_line.p2());
    path.addPath(m_startPath);
    path.addPath(m_endPath);
    return path;
}

//void CGraphicsLineItem::setLinePenWidth(int width)
//{
//    this->pen().setWidth(width);
//    calcVertexes();
//    updateHandlesGeometry();
//}

QPainterPath CGraphicsLineItem::inSideShape() const
{
    QPainterPath path;

    if (this->curView() == nullptr)
        return path;

    if (m_line == QLineF())
        return path;

    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());

    QPen pen = this->pen();
    qreal scale = curView()->getDrawParam()->getScale();
    if (pen.width() * int(scale) < 20) {
        if (scale > 1) {
            pen.setWidthF(20 / scale);
        } else {
            pen.setWidth(20);
        }

    }

    path.addPath(m_startPath);
    path.addPath(m_endPath);

    return qt_graphicsItem_shapeFromPath(path, pen);
}
