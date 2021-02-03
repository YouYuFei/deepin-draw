#include "cgraphicsmasicoitem.h"
#include "cdrawparamsigleton.h"
#include "sitemdata.h"
#include "cgraphicstextitem.h"
#include "widgets/ctextedit.h"
#include "cgraphicsproxywidget.h"
#include "cdrawscene.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"
#include "cgraphicsitemselectedmgr.h"

#include <DApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>
#include <QDebug>

DWIDGET_USE_NAMESPACE

CGraphicsMasicoItem::CGraphicsMasicoItem(QGraphicsItem *parent)
    : CGraphicsPenItem(parent)
    , m_pixmap(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getCutSize())
    , m_nBlurEffect(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getBlurEffect())
{
    this->setSizeHandleRectFlag(CSizeHandleRect::Rotation, false);
}

CGraphicsMasicoItem::CGraphicsMasicoItem(const QPointF &startPoint, QGraphicsItem *parent)
    : CGraphicsPenItem(startPoint, parent)
    , m_pixmap(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getCutSize())
    , m_nBlurEffect(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getBlurEffect())
{
    this->setSizeHandleRectFlag(CSizeHandleRect::Rotation, false);
}

CGraphicsMasicoItem::CGraphicsMasicoItem(const SGraphicsBlurUnitData *data, const SGraphicsUnitHead &head, CGraphicsItem *parent)
    : CGraphicsPenItem(&(data->data), head, parent)
    , m_pixmap(CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getCutSize())
    , m_nBlurEffect(EBlurEffect(data->effect))
{
    updateBlurPath();
}

//CGraphicsMasicoItem::CGraphicsMasicoItem(const CGraphicsUnit &unit, CGraphicsItem *parent)
//    : CGraphicsPenItem(unit, parent)
//    , m_pixmap(CDrawParamSigleton::GetInstance()->getCutSize())
//    , m_nBlurEffect(CDrawParamSigleton::GetInstance()->getBlurEffect())
//{
//    m_nBlurEffect = static_cast<EBlurEffect>(unit.data.pBlur->effect);
//    setBlurWidth(unit.data.pBlur->blurWidth);
//}

int CGraphicsMasicoItem::type() const
{
    return BlurType;
}

void CGraphicsMasicoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    updateHandlesGeometry();
    QGraphicsScene *scene = this->scene();
    //绘制滤镜
    if (scene != nullptr) {
        //计算交叉矩形的区域
        QPixmap tmpPixmap = m_pixmap;
        painter->save();
        painter->setClipPath(m_blurPath, Qt::IntersectClip);
        //判断和他交叉的元素，裁剪出下层的像素
        //下层有图元才显示
        int imgWidth = tmpPixmap.width();
        int imgHeigth = tmpPixmap.height();
        int radius = 10;
        if (!tmpPixmap.isNull()) {
            tmpPixmap = tmpPixmap.scaled(imgWidth / radius, imgHeigth / radius, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if (m_nBlurEffect == BlurEffect) {
                tmpPixmap = tmpPixmap.scaled(imgWidth, imgHeigth, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            } else {
                tmpPixmap = tmpPixmap.scaled(imgWidth, imgHeigth);
            }
        }
        painter->drawPixmap(boundingRect().topLeft(), tmpPixmap);
        painter->restore();
    }

//    painter->setPen(pen());
//    painter->setBrush(brush());
//    painter->drawPath(getPath());

    paintMutBoundingLine(painter, option);
}

bool CGraphicsMasicoItem::isPosPenetrable(const QPointF &posLocal)
{
    Q_UNUSED(posLocal)
    return false;
}

void CGraphicsMasicoItem::updateMasicPixmap()
{
    if (this->scene() != nullptr) {
        bool flag = this->isSelected();
        QList<QGraphicsItem * > items = this->scene()->items();
        QList<QGraphicsItem * > filterItems = this->filterItems(items);
        QList<bool> filterItemsSelectFlags;

        auto curScene = static_cast<CDrawScene *>(scene());
        auto itemsMgr = curScene->getItemsMgr();
        auto itemsMgrFlag = itemsMgr->isVisible();
        if (itemsMgrFlag) {
            itemsMgr->setFlag(ItemHasNoContents, true);
        }

        for (int i = 0; i != filterItems.size(); i++) {
            QGraphicsItem *pItem = filterItems[i];
            filterItemsSelectFlags.push_back(pItem->isSelected());
            pItem->setFlag(ItemHasNoContents, true);
            if (pItem->type() == TextType) {
                if (!pItem->childItems().isEmpty()) {
                    QGraphicsItem *pChild = pItem->childItems().first();
                    pChild->setFlag(ItemHasNoContents, true);
                }
            }
        }

        drawScene()->setDrawForeground(false);

        this->hide();
        QRect rect = this->sceneBoundingRect().toRect()/*this->scene()->sceneRect().toRect()*/;
        m_pixmap = QPixmap(rect.width(), rect.height());
        m_pixmap.fill(QColor(255, 255, 255, 0));
        QPainter painterd(&m_pixmap);
        painterd.setRenderHint(QPainter::Antialiasing);
        painterd.setRenderHint(QPainter::SmoothPixmapTransform);

        curScene->getDrawParam()->setRenderImage(2);

        this->scene()->setBackgroundBrush(Qt::transparent);

        this->scene()->render(&painterd, QRectF(0, 0, m_pixmap.width(), m_pixmap.height()),
                              rect);

        curScene->getDrawParam()->setRenderImage(0);

        curScene->resetSceneBackgroundBrush();
        if (itemsMgrFlag) {
            itemsMgr->setFlag(ItemHasNoContents, false);
        }

        this->show();
        this->setSelected(flag);

        for (int i = 0; i != filterItems.size(); i++) {
            QGraphicsItem *pItem = filterItems[i];
            pItem->setFlag(ItemHasNoContents, false);
            pItem->setSelected(filterItemsSelectFlags[i]);

            if (pItem->type() == TextType) {
                if (!pItem->childItems().isEmpty()) {
                    QGraphicsItem *pChild = pItem->childItems().first();
                    pChild->setFlag(ItemHasNoContents, false);
                }
            }
        }

        drawScene()->setDrawForeground(true);

//        if (textItemIndex != -1) {
//            CGraphicsTextItem *pTextItem = dynamic_cast<CGraphicsTextItem *>(items[textItemIndex]) ;
//            if (pTextItem != nullptr) {
//                pTextItem->setVisible(true);
//                pTextItem->getTextEdit()->show();
//                pTextItem->getTextEdit()->setTextCursor(textCursor);
//                pTextItem->getTextEdit()->setFocus(Qt::MouseFocusReason);
//            }
//        }
    }
}

//void CGraphicsMasicoItem::updateMasic()
//{
////    CDrawScene* p
////    if (this->drawScene() != nullptr) {

////    }
//}

void CGraphicsMasicoItem::updateMasicPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

QRectF CGraphicsMasicoItem::boundingRect() const
{
    QRectF rect = this->shape().boundingRect();
    return rect;
}

QPainterPath CGraphicsMasicoItem::shape() const
{
    QPainterPath path = getPath();
    if (m_isShiftPress) {
        path.lineTo(m_straightLine.p2());
    }
    return qt_graphicsItem_shapeFromPath(/*getPath()*/path, pen());
}

void CGraphicsMasicoItem::resizeTo(CSizeHandleRect::EDirection dir, const QPointF &point)
{
    CGraphicsPenItem::resizeTo(dir, point);
    updateBlurPath();
}

void CGraphicsMasicoItem::resizeToMul(CSizeHandleRect::EDirection dir,
                                      const QPointF &offset,
                                      const double &xScale, const double &yScale,
                                      bool bShiftPress, bool bAltPress)
{
    CGraphicsPenItem::resizeToMul(dir, offset, xScale, yScale, bShiftPress, bAltPress);
    updateBlurPath();
}

void CGraphicsMasicoItem::updateBlurPath()
{
    QPainterPathStroker t_stroker;
    t_stroker.setWidth(pen().widthF());
    QPainterPath path = getPath();
    if (m_isShiftPress) {
        path.lineTo(m_straightLine.p2());
    }
    QPainterPath t_painterPath = t_stroker.createStroke(path);
    m_blurPath = t_painterPath.simplified();
}

EBlurEffect CGraphicsMasicoItem::getBlurEffect() const
{
    return m_nBlurEffect;
}

void CGraphicsMasicoItem::setBlurEffect(EBlurEffect effect)
{
    prepareGeometryChange();
    m_nBlurEffect = effect;
    updateBlurPath();
}

int CGraphicsMasicoItem::getBlurWidth() const
{
    return this->pen().width();
}

void CGraphicsMasicoItem::setBlurWidth(int width)
{
    prepareGeometryChange();
    QPen pen = this->pen();
    pen.setWidth(width);
    this->setPen(pen);
    updateBlurPath();
    if (this->drawScene() != nullptr) {
        this->drawScene()->getItemsMgr()->updateBoundingRect();
    }
    updateMasicPixmap();
    CGraphicsItem::updateShape();
}

CGraphicsUnit CGraphicsMasicoItem::getGraphicsUnit(bool all) const
{
    Q_UNUSED(all)
    CGraphicsUnit unit;

    unit.head.dataType = this->type();
    unit.head.dataLength = sizeof(SGraphicsBlurUnitData);
    unit.head.pen = this->pen();
    unit.head.brush = this->brush();
    unit.head.pos = this->pos();
    unit.head.rotate = this->rotation();
    unit.head.zValue = this->zValue();

    unit.data.pBlur = new SGraphicsBlurUnitData();
//    unit.data.pBlur->data.start_type = this->getPenStartType();
//    unit.data.pBlur->data.end_type = this->getPenEndType();
    unit.data.pBlur->data.path = this->getPath();
//    unit.data.pBlur->data.arrow = this->getArrow();

    unit.data.pBlur->effect = m_nBlurEffect;

    return unit;
}

void CGraphicsMasicoItem::loadGraphicsUnit(const CGraphicsUnit &data, bool allInfo)
{
    Q_UNUSED(allInfo)
    if (data.data.pBlur != nullptr) {
//        m_penStartType = data.data.pBlur->data.start_type;
//        m_penEndType = data.data.pBlur->data.end_type;
        setPath(data.data.pPen->path);
        m_nBlurEffect = EBlurEffect(data.data.pBlur->effect);
    }
    loadHeadData(data.head);

    calcVertexes();
    updateHandlesGeometry();
}

CGraphicsItem *CGraphicsMasicoItem::duplicateCreatItem()
{
    return new CGraphicsMasicoItem;
}

void CGraphicsMasicoItem::duplicate(CGraphicsItem *item)
{
    CGraphicsPenItem::duplicate(item);
    static_cast<CGraphicsMasicoItem *>(item)->setBlurEffect(m_nBlurEffect);
    static_cast<CGraphicsMasicoItem *>(item)->updateBlurPath();
    static_cast<CGraphicsMasicoItem *>(item)->updateMasicPixmap(m_pixmap);
}

QList<QGraphicsItem *> CGraphicsMasicoItem::filterItems(QList<QGraphicsItem *> items)
{
    qreal thisZValue = this->zValue();
    QList<QGraphicsItem *> retList;
    if (this->scene() != nullptr) {
        QList<QGraphicsItem *> allitems = this->scene()->items();

        foreach (QGraphicsItem *item, items) {
            //只对自定义的图元生效
            if ((item->type() > QGraphicsItem::UserType && item->type() < MgrType)) {
                if (item->type() == BlurType && item != this) {
                    retList.push_back(item);
                    continue;
                }
                qreal itemZValue = item->zValue();
                if (thisZValue < itemZValue) {
                    retList.push_back(item);
                }
            }
        }
    }

    return retList;
}

//ELineType CGraphicsMasicoItem::getPenStartType() const
//{
//    return m_penStartType;
//}

//void CGraphicsMasicoItem::setPenStartType(const ELineType &penType)
//{
//    m_penStartType = penType;
//}

//ELineType CGraphicsMasicoItem::getPenEndType() const
//{
//    return m_penEndType;
//}

//void CGraphicsMasicoItem::setPenEndType(const ELineType &penType)
//{
//    m_penEndType = penType;
//}
