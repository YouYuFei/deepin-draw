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
#include "cpicturewidget.h"

#include "drawshape/cdrawparamsigleton.h"
#include "drawshape/globaldefine.h"

#include "frame/cviewmanagement.h"

#include "service/cmanagerattributeservice.h"

#include <QMap>
#include <QHBoxLayout>
#include <QDebug>

#include <DPalette>
#include <DApplicationHelper>
#include <DGuiApplicationHelper>
#include <DPushButton>

DGUI_USE_NAMESPACE

const int BTN_SPACING = 13;

CPictureWidget::CPictureWidget(DWidget *parent)
    : DWidget(parent)
{
    initUI();
    initConnection();
}

CPictureWidget::~CPictureWidget()
{
}

void CPictureWidget::changeButtonTheme()
{

}

void CPictureWidget::setAdjustmentIsEnable(bool isEnable)
{
    m_flipAdjustment->setEnabled(isEnable);
}

void CPictureWidget::setRotationEnable(bool isEnable)
{
    m_leftRotateBtn->setEnabled(!isEnable);
    m_rightRotateBtn->setEnabled(!isEnable);
    m_flipHBtn->setEnabled(!isEnable);
    m_flipVBtn->setEnabled(!isEnable);
}

void CPictureWidget::initUI()
{
    m_leftRotateBtn = new DPushButton(this);
    m_leftRotateBtn->setMaximumSize(QSize(38, 38));
    m_leftRotateBtn->setIcon(QIcon::fromTheme("ddc_contrarotate_normal"));
    m_leftRotateBtn->setIconSize(QSize(48, 48));
    m_leftRotateBtn->setToolTip(tr("Rotate 90° CCW"));

    m_rightRotateBtn = new DPushButton(this);
    m_rightRotateBtn->setMaximumSize(QSize(38, 38));
    m_rightRotateBtn->setIcon(QIcon::fromTheme("ddc_clockwise rotation_normal"));
    m_rightRotateBtn->setIconSize(QSize(48, 48));
    m_rightRotateBtn->setToolTip(tr("Rotate 90° CW"));

    m_flipHBtn = new DPushButton(this);
    m_flipHBtn->setMaximumSize(QSize(38, 38));
    m_flipHBtn->setIcon(QIcon::fromTheme("ddc_flip horizontal_normal"));
    m_flipHBtn->setIconSize(QSize(48, 48));
    m_flipHBtn->setToolTip(tr("Flip horizontally"));

    m_flipVBtn = new DPushButton(this);
    m_flipVBtn->setMaximumSize(QSize(38, 38));
    m_flipVBtn->setIcon(QIcon::fromTheme("ddc_flip vertical_normal"));
    m_flipVBtn->setIconSize(QSize(48, 48));
    m_flipVBtn->setToolTip(tr("Flip vertically"));

    m_flipAdjustment = new DPushButton(this);
    m_flipAdjustment->setMaximumSize(QSize(38, 38));
    m_flipAdjustment->setIcon(QIcon::fromTheme("ddc_flip_adjustment_normal"));
    m_flipAdjustment->setIconSize(QSize(48, 48));
    m_flipAdjustment->setToolTip(tr("Auto fit"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setSpacing(BTN_SPACING);
    layout->addStretch();
    layout->addWidget(m_leftRotateBtn);
    layout->addWidget(m_rightRotateBtn);
    layout->addWidget(m_flipHBtn);
    layout->addWidget(m_flipVBtn);
    layout->addWidget(m_flipAdjustment);
    layout->addStretch();
    setLayout(layout);
}

void CPictureWidget::initConnection()
{
    connect(m_leftRotateBtn, &DPushButton::released, this, [ = ]() {
        CManagerAttributeService::getInstance()->setPictureRotateOrFlip(ERotationType::LeftRotate_90);
    });

    connect(m_rightRotateBtn, &DPushButton::released, this, [ = ]() {
        CManagerAttributeService::getInstance()->setPictureRotateOrFlip(ERotationType::RightRotate_90);
    });

    connect(m_flipHBtn, &DPushButton::released, this, [ = ]() {
        CManagerAttributeService::getInstance()->setPictureRotateOrFlip(ERotationType::FlipHorizontal);
    });

    connect(m_flipVBtn, &DPushButton::released, this, [ = ]() {
        CManagerAttributeService::getInstance()->setPictureRotateOrFlip(ERotationType::FlipVertical);
    });

    connect(m_flipAdjustment, &DPushButton::released, this, [ = ]() {
        CManagerAttributeService::getInstance()->doSceneAdjustment();
        m_flipAdjustment->setEnabled(false);
    });
}
