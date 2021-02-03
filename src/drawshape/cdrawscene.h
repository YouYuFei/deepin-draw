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
#ifndef CDRAWSCENE_H
#define CDRAWSCENE_H
#include "cselecttool.h"
#include <QGraphicsScene>
#include <QObject>

#include "drawshape/globaldefine.h"

class QGraphicsSceneMouseEvent;
class QKeyEvent;
class CGraphicsRectItem;
class CGraphicsPolygonItem;
class CGraphicsPolygonalStarItem;
class CGraphicsPenItem;
class CGraphicsLineItem;
class CGraphicsMasicoItem;
class CGraphicsItemSelectedMgr;
class CGraphicsItemHighLight;
class CDrawParamSigleton;
class CGraphicsView;

class CDrawScene : public QGraphicsScene
{
    Q_OBJECT
    friend CSelectTool;

public:
    /**
     * @brief CDrawScene 构造函数
     * @param parent
     */
    explicit CDrawScene(CGraphicsView *view = nullptr,
                        const QString &uuid = "",
                        bool isModified = false);
    ~CDrawScene() override;

    /**
     * @brief drawView 返回视图指针
     */
    CGraphicsView *drawView();


    /**
     * @brief resetScene 重置场景
     */
    void resetScene();

    /**
     * @brief keyEvent 从绘图工具返回键盘事件
     * @param keyEvent
     */
    void keyEvent(QKeyEvent *keyEvent);
    /**
     * @brief setCursor 设置鼠标指针形状
     * @param cursor
     */
    void setCursor(const QCursor &cursor);

    /**
     * @brief mouseEvent 鼠标事件
     * @param mouseEvent
     */
    void mouseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    /**
     * @brief drawBackground
     * @param painter
     * @param rect
     */
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    /**
     * @brief resetSceneBackgroundBrush  重新设置背景颜色
     */
    void resetSceneBackgroundBrush();
    /**
     * @brief showCutItem 显示裁剪图元
     */
    void showCutItem();
    /**
     * @brief quitCutMode 退出裁剪
     */
    void quitCutMode();
    /**
     * @brief doCutScene 进行裁剪
     */
    void doCutScene();
    /**
     * @brief doAdjustmentScene 自适应
     * @param rect scene大小
     */
    void doAdjustmentScene(QRectF rect, CGraphicsItem *item = nullptr);

    void setItemDisable(bool canSelecte);

//    void textFontFamilyChanged();

//    void textFontSizeChanged();

    void blockUpdateBlurItem(bool b);
    /**
     * @brief updateBlurItem 刷新
     * @param changeItem 引起变化的图元
     */
    void updateBlurItem(QGraphicsItem *changeItem = nullptr);

    void switchTheme(int type);

    CGraphicsItemSelectedMgr *getItemsMgr() const;
    CGraphicsItemHighLight *getItemHighLight() const;

//    qreal totalScalefactor();

    /**
     * @brief getCDrawParam　获取绘制数据
     */
    CDrawParamSigleton *getDrawParam();

    bool getModify() const;
    void setModify(bool isModify);

    bool isBussizeItem(QGraphicsItem *pItem);
    bool isBussizeHandleNodeItem(QGraphicsItem *pItem);
    bool isBzAssicaitedItem(QGraphicsItem *pItem);

    CGraphicsItem *getAssociatedBzItem(QGraphicsItem *pItem);

    void clearMrSelection();
    void selectItem(QGraphicsItem *pItem, bool onlyBzItem = true, bool updateAttri = true, bool updateRect = true);
    void notSelectItem(QGraphicsItem *pItem, bool updateAttri = true, bool updateRect = true);
    void selectItemsByRect(const QRectF &rect, bool replace = true, bool onlyBzItem = true);

//    void moveMrItem(const QPointF &prePos, const QPointF &curPos);
//    void resizeMrItem(CSizeHandleRect::EDirection direction,
//                      const QPointF &prePos,
//                      const QPointF &curPos,
//                      bool keepRadio = false);

    QList<QGraphicsItem *> getBzItems(const QList<QGraphicsItem *> &items = QList<QGraphicsItem *>());

    //EDesSort降序（第一个为最顶层）   EAesSort升序（第一个为最底层）
    enum ESortItemTp { EDesSort,
                       EAesSort,
                       ESortCount
                     };
    void sortZ(QList<QGraphicsItem *> &list, ESortItemTp tp = EDesSort);
    QList<QGraphicsItem *> returnSortZItems(const QList<QGraphicsItem *> &list, ESortItemTp tp = EDesSort);

    //penalgor表示Penetration algorithm，穿透算法
    CGraphicsItem *topBzItem(const QPointF &pos, bool penalgor = true, int IncW = 0);

    CGraphicsItem *firstBzItem(const QList<QGraphicsItem *> &items,
                               bool haveDesSorted = false);

    /*
     *  penalgor表示Penetration algorithm，穿透算法，需要考虑穿透算法过滤
        isBzItem表示必须返回的顶层图元需要是业务图元。
        seeNodeAsBzItem 将业务图元的子图元node也当做是业务图元的一部分，如果为true将返回其父业务图元
        filterMrAndHightLight 过滤掉不参与考虑的图元
    */
    QGraphicsItem *firstItem(const QPointF &pos,
                             const QList<QGraphicsItem *> &items = QList<QGraphicsItem *>(),
                             bool isListDesSorted = true,
                             bool penalgor = false,
                             bool isBzItem = false,
                             bool seeNodeAsBzItem = false,
                             bool filterMrAndHightLight = true,
                             int incW = 0);

    //第一个非Mr的item
    QGraphicsItem *firstNotMrItem(const QList<QGraphicsItem *> items);

    void moveItems(const QList<QGraphicsItem *> &itemlist, const QPointF &move);

//    //以图元的中心进行旋转
//    void rotatBzItem(CGraphicsItem *pBzItem, qreal angle);

    /**
     * @brief setMaxZValue 记录图元最大z值
     * @param zValue 图元z值
     */
    void setMaxZValue(qreal zValue);

    /**
     * @brief getMaxZValue　获取图元最大z值
     */
    qreal getMaxZValue();

//    void updateItemsMgr();


    void blockMouseMoveEvent(bool b);
    bool isBlockMouseMoveEvent();

    /**
     * @brief recordItemsInfoToCmd 记录图元的信息
     */
    void recordItemsInfoToCmd(const QList<CGraphicsItem *> &items, bool isUndo);
    void finishRecord(bool doRedoCmd = false);

signals:
    /**
     * @brief signalAttributeChanged 发送属性栏更改的信号
     * @param flag
     * @param primitiveType 图元类型
     */
    void signalAttributeChanged(bool flag, int primitiveType);
    /**
     * @brief signalChangeToSelect 发送工具栏切换为选择的信号
     * @param showTitle 是否显示select工具的顶层标题(也就是"画板"标题文字)
     */
    void signalChangeToSelect(bool showTitle = false);
    /**
     * @brief signalQuitCutMode 退出裁剪模式
     */
    void signalQuitCutAndChangeToSelect();

    /**
     * @brief itemMoved 移动
     * @param item
     * @param pos
     */
    void itemMoved(QGraphicsItem *item, QPointF pos);

    /**
     * @brief itemAdded 增加图元
     * @param item
     */
    void itemAdded(QGraphicsItem *item, bool pushToStack = true);

    /**signalItemAdded
     * @brief itemRotate 旋转图元
     * @param item
     * @param oldAngle
     */
    void itemRotate(QGraphicsItem *item, const qreal oldAngle);

    /**
     * @brief itemResize 更改图元大小
     * @param item
     * @param handle
     * @param beginPos
     * @param endPos
     * @param bShiftPress
     * @param bALtPress
     */
    void itemResize(CGraphicsItem *item, CSizeHandleRect::EDirection handle, QRectF beginRect, QPointF endPos, bool bShiftPress, bool bALtPress);

    /**
     * @brief itemPropertyChange 图元属性修改
     * @param item
     * @param pen
     * @param brush
     * @param bPenChange
     * @param bBrushChange
     */
    void itemPropertyChange(CGraphicsItem *item, QPen pen, QBrush brush, bool bPenChange, bool bBrushChange);

    /**
     * @brief itemRectXRediusChange 矩形圆角属性修改
     * @param xRedius 圆角半径
     * @param bChange
     */
    void itemRectXRediusChange(CGraphicsRectItem *item, int xRedius, bool bChange);
    /**
     * @brief itemPolygonPointChange 多边形边数更改
     * @param item
     * @param oldNum
     */
    void itemPolygonPointChange(CGraphicsPolygonItem *item, int oldNum);

    /**
     * @brief itemPolygonalStarPointChange 星形边数更改
     * @param item
     * @param oldNum
     * @param oldRadius
     */
    void itemPolygonalStarPointChange(CGraphicsPolygonalStarItem *item, int oldNum, int oldRadius);

    /**
     * @brief itemBlurChange 模糊图元属性更改
     * @param item
     * @param effect
     * @param blurWidth
     */
    void itemBlurChange(CGraphicsMasicoItem *item, int effect, int blurWidth);

    /**
     * @brief itemPenTypeChange 画笔图元更改
     * @param item
     * @param oldType
     */
    void itemPenTypeChange(CGraphicsPenItem *item, bool isStart, ELineType oldType);

    /**
     * @brief itemLineTypeChange 线图元属性更改
     * @param item
     * @param type
     */
    void itemLineTypeChange(CGraphicsLineItem *item, bool isStart, ELineType endType);

    /**
     * @brief signalUpdateCutSize 更新裁剪的大小
     */
    void signalUpdateCutSize();

    /**
     * @brief signalUpdateTextFont 更新文字字体
     */
    void signalUpdateTextFont();

    /**
     * @brief signalUpdateColorPanelVisible 更新调色板显示
     * @param pos
     */
    void signalUpdateColorPanelVisible(QPoint pos);

    /**
     * @brief signalSceneCut 裁剪场景
     * @param newRect
     */
    void signalSceneCut(QRectF newRect);

    /**
     * @brief signalIsModify 是否更改
     * @param newRect
     */
    void signalIsModify(bool isModify);

public slots:
    /**
     * @brief drawToolChange 切换绘图工具
     * @param type
     */
//    void drawToolChange(int type, bool clearSections = true);

    /**
     * @brief changeMouseShape 切换鼠标形状
     * @param type
     */
    void changeMouseShape(EDrawToolMode type);

    /**
     * @brief clearMutiSelectedState 清除多选状态
     */
    void clearMutiSelectedState();

    /**
     * @brief setDrawForeground 是否绘制前景元素(前景主要绘制了框选矩形,高亮,模糊图元截图时不需要绘制这些)
     */
    void setDrawForeground(bool b);

    /**
     * @brief setDrawForeground 是否绘制前景元素
     */
    bool isDrawedForeground();


    void doLeave();

protected:

    /**
     * @brief mousePressEvent 鼠标按下事件
     * @param mouseEvent
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    /**
     * @brief mouseMoveEvent 鼠标移动事件
     * @param mouseEvent
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    /**
     * @brief mouseReleaseEvent 鼠标放开事件
     * @param mouseEvent
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;


    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    virtual bool event(QEvent *event) override;

    /**
     * @brief drawItems 绘制所有图元
     * @param painter
     * @param numItems
     * @param items
     * @param options
     * @param widget
     */
    virtual void drawItems(QPainter *painter, int numItems,
                           QGraphicsItem *items[],
                           const QStyleOptionGraphicsItem options[],
                           QWidget *widget = nullptr) override;

    virtual void drawForeground(QPainter *painter, const QRectF &rect) override;

    virtual void keyReleaseEvent(QKeyEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

public:
    void refreshLook(const QPointF &pos = QPointF());
    /*
     * @breaf: clearHighlight 清除高亮图元路径
    */
    void clearHighlight();

    void setHighlightHelper(const QPainterPath &path);

    QPainterPath hightLightPath();

private:
    /**
     * @brief initScene 初始化一个新的场景
     */
    void initScene();

private:
    CDrawParamSigleton *m_drawParam;//数据

    bool m_bIsEditTextFlag;

    QCursor m_drawMouse;
    QCursor m_lineMouse;
    QCursor m_pengatonMouse;
    QCursor m_rectangleMouse;
    QCursor m_roundMouse;
    QCursor m_starMouse;
    QCursor m_triangleMouse;
    QCursor m_textMouse;
    QCursor m_brushMouse;
    QCursor m_blurMouse;
    qreal m_maxZValue;

    CGraphicsItemSelectedMgr *m_pGroupItem;
    CGraphicsItemHighLight *m_pHighLightItem;

    bool dbCLicked = false;

    bool blockMouseMoveEventFlag = false;

    bool blockMscUpdate = false;

    bool bDrawForeground = true;

    QPainterPath _highlight;

    /* 文字可编辑光标 */
    QCursor m_textEditCursor;
};

#endif // CDRAWSCENE_H
