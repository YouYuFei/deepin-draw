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
#ifndef CDRAWTOOLMANAGERSIGLETON_H
#define CDRAWTOOLMANAGERSIGLETON_H

#include "globaldefine.h"
#include <QHash>
class IDrawTool;
class CDrawToolManagerSigleton
{
private :
    static CDrawToolManagerSigleton *m_pInstance;
    CDrawToolManagerSigleton();

public :
    static CDrawToolManagerSigleton *GetInstance();

    void insertDrawTool(EDrawToolMode mode, IDrawTool *tool);
    IDrawTool *getDrawTool(EDrawToolMode mode) const;

private:
    QHash <EDrawToolMode, IDrawTool *> m_hashDrawTool;
};

#endif // CDRAWTOOLMANAGERSIGLETONH_H
