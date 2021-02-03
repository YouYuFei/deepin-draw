#ifndef CGRAPHICSITEMGROUP_H
#define CGRAPHICSITEMGROUP_H

#include <QGraphicsItemGroup>
class CGraphicsItemGroup : public QGraphicsItemGroup
{
public:
    CGraphicsItemGroup();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};
#endif // CGRAPHICSITEMGROUP_H
