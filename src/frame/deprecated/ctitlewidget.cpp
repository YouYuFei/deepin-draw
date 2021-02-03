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
#include "ctitlewidget.h"
#include "drawshape/cdrawparamsigleton.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"

#include <QHBoxLayout>
#include <QDebug>

CTitleWidget::CTitleWidget(DWidget *parent)
    : DWidget(parent)
{
    m_title = new DLabel(this);
    m_text = tr("Unnamed");
    m_title->setText(m_text);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addStretch();
    layout->addWidget(m_title);
    layout->addStretch();

    setLayout(layout);
}

CTitleWidget::~CTitleWidget()
{

}

void CTitleWidget::updateTitleWidget()
{
    // 此处标题只显示画板字样
    if (m_text.isEmpty()) {
//        m_title->setText(tr("Draw"));
        m_title->setText("");
    } else {
        m_title->setText(m_text);
    }

//    QString path = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getDdfSavePath();
//    if (path.isEmpty()) {
//        m_title->setText(tr("Draw"));
//    } else {
//        int index = path.lastIndexOf("/");
//        QString name = path.mid(index + 1);
//        m_title->setText(name);
    //    }
}

void CTitleWidget::setTittleText(QString tittle)
{
    m_text = tittle;
    updateTitleWidget();
}

