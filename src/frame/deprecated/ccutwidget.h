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
#ifndef CCUTWIDGET_H
#define CCUTWIDGET_H

#include "widgets/seperatorline.h"
#include <DWidget>
#include <DLineEdit>
#include <QAction>

DWIDGET_USE_NAMESPACE

class CCutWidget : public DWidget
{
    Q_OBJECT
public:


public:
    explicit CCutWidget(DWidget *parent = nullptr);
    ~CCutWidget();

    /**
     * @brief updateCutSize 更新裁剪尺寸
     */
    void updateCutSize();
    /**
     * @brief clearAllChecked　清除所有按钮选中
     */
    void clearAllChecked();
    /**
     * @brief changeButtonTheme 根据主题改变按钮主题
     */
    void changeButtonTheme();
    /**
     * @brief updateButtonStatus　更新按钮状态
     */
    void updateButtonStatus();

signals:
    /**
     * @brief signalCutAttributeChanged　裁剪属性改变型号
     */
    void signalCutAttributeChanged();
    /**
     * @brief signalCutLineEditIsfocus　裁剪尺寸输入框焦点变化信号
     */
    void signalCutLineEditIsfocus(bool);

public slots:

private:
    DPushButton *m_scaleBtn1_1;
    DPushButton *m_scaleBtn2_3;
    DPushButton *m_scaleBtn8_5;
    DPushButton *m_scaleBtn16_9;

    DPushButton *m_freeBtn;
    DPushButton *m_originalBtn;

    DLineEdit *m_widthEdit;
    DLineEdit *m_heightEdit;

    DPushButton *m_cutBtn;
    DPushButton *m_cancelBtn;
    SeperatorLine *m_sepLine;

    QAction *m_SizeAddAction;
    QAction *m_SizeReduceAction;

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
     * @brief activeFreeMode 激活自由按钮
     */
    void activeFreeMode();
};

#endif // CCUTWIDGET_H
