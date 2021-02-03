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
#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <DPushButton>

DWIDGET_USE_NAMESPACE

class ToolButton : public DPushButton
{
    Q_OBJECT
public:
    ToolButton(DWidget *parent = 0)
    {
        Q_UNUSED(parent);
        setFixedSize(24, 24);
        setCheckable(true);
    }

    ToolButton(QString text, DWidget *parent = 0)
    {
        Q_UNUSED(parent);
        setFixedSize(24, 24);
        setCheckable(true);
        setText(text);
    }
    ~ToolButton() {}

};

#endif // TOOLBUTTON_H

