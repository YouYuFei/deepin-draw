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
#ifndef BIGCOLORBUTTON_H
#define BIGCOLORBUTTON_H

#include <DPushButton>
#include <QPainter>
#include <QPaintEvent>

#include "globaldefine.h"

DWIDGET_USE_NAMESPACE

class BigColorButton : public DPushButton
{
    Q_OBJECT
public:
    BigColorButton(DWidget *parent = nullptr);
    ~BigColorButton();

    void setColor(QColor color, EChangedPhase phase = EChanged);
//    void setColorIndex(int index);
//    void updateConfigColor();
//    void resetChecked();
    void setIsMultColorSame(bool isMultColorSame);

signals:
    void colorChanged(const QColor &color, EChangedPhase phase);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);

    void paintLookStyle(QPainter *painter, bool isMult = false);

private:
    QColor m_color;
    bool m_isHover;
    bool m_isChecked;
    bool m_isMultColorSame;//多选图元，颜色不一致时使用,单选的时候默认为true
};

#endif // BIGCOLORBUTTON_H
