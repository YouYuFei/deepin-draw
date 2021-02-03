/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     JiangChangli
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
#include "drawdialog.h"
#include "utils/baseutils.h"
#include "drawshape/cdrawparamsigleton.h"
#include "application.h"

#include <QHBoxLayout>
#include <DLabel>
#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
#include <QApplication>

DrawDialog::DrawDialog(DWidget *parent)
    : Dialog(parent)
{
    setModal(true);

    drawApp->setWidgetAccesibleName(this, "Notice save dialog");

    setIcon(QPixmap(":/theme/common/images/deepin-draw-64.svg"));

    addButton(tr("Cancel"), false, DDialog::ButtonNormal);
    addButton(tr("Discard"), false, DDialog::ButtonNormal);
    addButton(tr("Save"), true, DDialog::ButtonRecommend);

    // Input content
    DLabel *title = new DLabel(tr("Save the current contents?"), this);
    title->setWordWrap(true);
    title->setAlignment(Qt::AlignCenter);

    DWidget *w = new DWidget(this);
    w->setFixedHeight(this->height() - 80);

    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    layout->addStretch();
    layout->addWidget(title);
    layout->addStretch();
    addContent(w);
}


void  DrawDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        this->close();
    }
}

