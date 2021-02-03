#include "cgraphicsitemhighlight.h"
#include "cdrawparamsigleton.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"
#include "cgraphicsitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

CGraphicsItemHighLight::CGraphicsItemHighLight(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
}

//QPainterPath CGraphicsItemHighLight::shape() const
//{
//    return m_path;
//}

//void CGraphicsItemHightLight::move(QPointF beginPoint, QPointF movePoint)
//{
//}

int CGraphicsItemHighLight::type() const
{
    return hightLightType;
}

QRectF CGraphicsItemHighLight::boundingRect() const
{
    return shape().controlPointRect();
}

void CGraphicsItemHighLight::setPen(const QPen &pen)
{
    m_pen = pen;
}

//void CGraphicsItemHighLight::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point)
//{

//}

//void CGraphicsItemHighLight::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point, bool bShiftPress, bool bAltPress)
//{

//}

//QRectF CGraphicsItemHighLight::rect() const
//{
//    return m_path.controlPointRect().normalized();
//}

//void CGraphicsItemHighLight::updateGeometry()
//{

//}

//void CGraphicsItemHighLight::setPainterPath(QPainterPath path)
//{
//    m_path = path;
//    update();
//}

void CGraphicsItemHighLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // 获取系统活动色的颜色
    QPalette pa = this->scene()->palette();
    QPen pen;
    pen.setWidthF(0.5 / CManageViewSigleton::GetInstance()->getCurView()->transform().m11());
    QBrush selectBrush = pa.brush(QPalette::Active, QPalette:: Highlight);

    pen.setColor(selectBrush.color());
    pen.setWidth(2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(this->path());
}
