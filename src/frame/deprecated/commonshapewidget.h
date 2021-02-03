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
#ifndef FILLSHAPEWIDGET_H
#define FILLSHAPEWIDGET_H

#include <DWidget>
#include <DSpinBox>
#include <DLabel>
#include "drawshape/globaldefine.h"

DWIDGET_USE_NAMESPACE


class BorderColorButton;
class BigColorButton;
class CSideWidthWidget;
class SeperatorLine;
class CManageViewSigleton;
class CSpinBox;

class CommonshapeWidget : public DWidget
{
    Q_OBJECT
public:
    CommonshapeWidget(DWidget *parent = nullptr);
    ~CommonshapeWidget();
    /**
     * @brief changeButtonTheme 根据主题改变按钮主题
     */
    void changeButtonTheme();
    /**
     * @brief setRectXRedius 设置圆角矩形半径
     */
    void setRectXRedius(int redius);
    /**
     * @brief setRectXRediusSpinboxVisible 设置圆角矩形半径是否可见
     */
    void setRectXRediusSpinboxVisible(bool visible);
    /**
     * @brief updateMultCommonShapWidget 更新多选时属性栏
     * @param propertys 要显示的控件和数值
     */
    void updateMultCommonShapWidget(QMap<EDrawProperty, QVariant> propertys, bool write2cache = true);

signals:
    /**
     * @brief resetColorBtns　重置颜色按钮
     */
    void resetColorBtns();

    /**
     * @brief signalCommonShapeChanged 属性变化信号
     */
    void signalCommonShapeChanged();
    /**
     * @brief signalRectRediusChanged 圆角矩形半径变化信号
     */
    void signalRectRediusChanged(int);
    /**
     * @brief signalRectRediusIsfocus　圆角矩形半径焦点变化信号
     */
    void signalRectRediusIsfocus(bool);

public slots:
    /**
     * @brief updateCommonShapWidget 更新属性栏
     */
    void updateCommonShapWidget();
    /**
     * @brief slotRectRediusChanged 圆角矩形半径变化信号
     */
    void slotRectRediusChanged(int redius);
    /**
     * @brief slotSideWidthChoosed 描边粗细变化信号
     */
    void slotSideWidthChoosed(int width);

private:
    BigColorButton *m_fillBtn;
    BorderColorButton *m_strokeBtn;
    CSideWidthWidget *m_sideWidthWidget;
    SeperatorLine *m_sepLine;
    CSpinBox *m_rediusSpinbox;
    DLabel *m_rediusLable;

private:
    /**
     * @brief initUI 初始化ＵＩ
     */
    void initUI();
    /**
     * @brief initConnection 初始化连接
     */
    void initConnection();
    /**
     * @brief getBtnPosition　获取按钮位置
     * @param btn
     * @return
     */
    QPoint getBtnPosition(const DPushButton *btn);
};

#endif // FILLSHAPEWIDGET_H
