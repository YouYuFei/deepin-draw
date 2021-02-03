/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     RenRan
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
#ifndef CALPHACONTROLWIDGET_H
#define CALPHACONTROLWIDGET_H

#include <DWidget>
#include <DSlider>
#include <DLabel>
#include <DLineEdit>

#include "globaldefine.h"

DWIDGET_USE_NAMESPACE

class CAlphaControlWidget : public DWidget
{
    Q_OBJECT
public:
    explicit CAlphaControlWidget(DWidget *parent = nullptr);

    /**
     * @brief setAlpha　通过颜色setAlpha值更新控件
     * @param internalChanged　由内发生的变化
     */
    void setAlpha(int alpha, bool internalChanged = true);

    /**
     * @brief alphaValue　当前的alpha值
     */
    int alpha();

signals:
    /**
     * @brief alphaChanged　alpha值改变信号
     */
    void alphaChanged(int apl, EChangedPhase phase);

private:
    /**
     * @brief initUI　初始化ＵＩ
     */
    void initUI();
    /**
     * @brief initConnection　初始化连接
     */
    void initConnection();

private:
    DSlider *m_alphaSlider;
    DLineEdit *m_alphaLabel;
};

#endif // CALPHACONTROLWIDGET_H
