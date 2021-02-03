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
#include "ccutwidget.h"
#include "drawshape/cdrawscene.h"
#include "widgets/toolbutton.h"
#include "utils/cvalidator.h"
#include "drawshape/cdrawparamsigleton.h"
#include "frame/cviewmanagement.h"
#include "frame/cgraphicsview.h"
#include "drawshape/cdrawscene.h"
#include "service/cmanagerattributeservice.h"

#include <DLabel>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

const int BTN_SPACING = 6;
const int SEPARATE_SPACING = 5;
const int PUSHBUTTON_FONT_SIZE = 12;
const int TEXT_SIZE = 12;

CCutWidget::CCutWidget(DWidget *parent)
    : DWidget(parent)
{
    initUI();
    initConnection();

}

CCutWidget::~CCutWidget()
{

}

void CCutWidget::updateCutSize()
{
    QSize size = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getCutSize();
    m_widthEdit->setText(QString::number(size.width()));
    m_heightEdit->setText(QString::number(size.height()));
}

void CCutWidget::clearAllChecked()
{
    m_scaleBtn1_1->setChecked(false);
    m_scaleBtn2_3->setChecked(false);
    m_scaleBtn8_5->setChecked(false);
    m_scaleBtn16_9->setChecked(false);
    m_freeBtn->setChecked(false);
    m_originalBtn->setChecked(false);
}

void CCutWidget::changeButtonTheme()
{
    m_sepLine->updateTheme();
}

void CCutWidget::updateButtonStatus()
{
    // [0] update size
    updateCutSize();

    // [1] set all unchecked button
    clearAllChecked();

    // [2] set current checked button
    ECutType current = CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->getCutType();
    switch (current) {
    case ECutType::cut_1_1: {
        m_scaleBtn1_1->setChecked(true);
        break;
    }
    case ECutType::cut_2_3: {
        m_scaleBtn2_3->setChecked(true);
        break;
    }
    case ECutType::cut_8_5: {
        m_scaleBtn8_5->setChecked(true);
        break;
    }
    case ECutType::cut_16_9: {
        m_scaleBtn16_9->setChecked(true);
        break;
    }
    case ECutType::cut_free: {
        m_freeBtn->setChecked(true);
        break;
    }
    case ECutType::cut_original: {
        m_originalBtn->setChecked(true);
        break;
    }
    default: {
        m_freeBtn->setChecked(true);
    }
    }
}

void CCutWidget::initUI()
{
    DLabel *sizeLabel = new DLabel(this);
    sizeLabel->setText(tr("Dimensions"));
    QFont ft;
    ft.setPixelSize(TEXT_SIZE);
    sizeLabel->setFont(ft);

    m_widthEdit = new DLineEdit(this);
    m_widthEdit->setText(QString::number(800));
    m_widthEdit->setClearButtonEnabled(false);
    m_widthEdit->setFixedWidth(60);
    m_widthEdit->lineEdit()->setValidator(new CIntValidator(10, 9999, this));
    m_widthEdit->setFont(ft);

    DLabel *multiLabel = new DLabel(this);
    multiLabel->setText(tr("x"));

    m_heightEdit = new DLineEdit(this);
    m_heightEdit->setText(QString::number(600));
    m_heightEdit->setClearButtonEnabled(false);
    m_heightEdit->setFixedWidth(60);
    m_heightEdit->lineEdit()->setValidator(new CIntValidator(10, 9999, this) );
    m_heightEdit->setFont(ft);

    m_SizeAddAction = new QAction(this);
    m_SizeAddAction->setShortcut(QKeySequence(Qt::Key_Up));
    this->addAction(m_SizeAddAction);

    m_SizeReduceAction = new QAction(this);
    m_SizeReduceAction->setShortcut(QKeySequence(Qt::Key_Down));
    this->addAction(m_SizeReduceAction);

    DLabel *scaleLabel = new DLabel(this);
    scaleLabel->setText(tr("Aspect ratio"));

    scaleLabel->setFont(ft);

    QFont pushBtnFont;
    pushBtnFont.setPixelSize(PUSHBUTTON_FONT_SIZE);

    m_scaleBtn1_1 = new DPushButton(this);
    m_scaleBtn1_1->setText("1:1");
    m_scaleBtn1_1->setFont(pushBtnFont);

    m_scaleBtn2_3 = new DPushButton(this);
    m_scaleBtn2_3->setText("2:3");
    m_scaleBtn2_3->setFont(pushBtnFont);

    m_scaleBtn8_5 = new DPushButton(this);
    m_scaleBtn8_5->setText("8:5");
    m_scaleBtn8_5->setFont(pushBtnFont);

    m_scaleBtn16_9 = new DPushButton(this);
    m_scaleBtn16_9->setText("16:9");
    m_scaleBtn16_9->setFont(pushBtnFont);

    m_freeBtn = new DPushButton(this);
    //m_freeBtn->setText(tr("自由"));
    m_freeBtn->setText(tr("Free"));
    m_freeBtn->setFont(pushBtnFont);

    m_originalBtn = new DPushButton(this);
    //m_originalBtn->setText(tr("原始"));
    m_originalBtn->setText(tr("Original"));
    m_originalBtn->setFont(pushBtnFont);

    m_scaleBtn1_1->setCheckable(true);
    m_scaleBtn2_3->setCheckable(true);
    m_scaleBtn8_5->setCheckable(true);
    m_scaleBtn16_9->setCheckable(true);
    m_freeBtn->setCheckable(true);
    m_originalBtn->setCheckable(true);

    m_freeBtn->setChecked(true);

    m_sepLine = new SeperatorLine(this);

    m_cutBtn = new DPushButton(this);
    m_cutBtn->setMaximumSize(QSize(38, 38));
    m_cutBtn->setIcon(QIcon::fromTheme("ddc_cutting_normal"));
    m_cutBtn->setIconSize(QSize(48, 48));

    m_cancelBtn = new DPushButton(this);
    m_cancelBtn->setMaximumSize(QSize(38, 38));
    m_cancelBtn->setIcon(QIcon::fromTheme("ddc_cancel_normal"));
    m_cancelBtn->setIconSize(QSize(48, 48));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(BTN_SPACING);
    layout->addStretch();
    layout->addWidget(sizeLabel);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_widthEdit);
    layout->addWidget(multiLabel);
    layout->addWidget(m_heightEdit);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(scaleLabel);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_scaleBtn1_1);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_scaleBtn2_3);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_scaleBtn8_5);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_scaleBtn16_9);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_freeBtn);
    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_originalBtn);

    layout->addSpacing(SEPARATE_SPACING);
    layout->addWidget(m_sepLine);
    layout->addWidget(m_cancelBtn);
    layout->addWidget(m_cutBtn);

    layout->addStretch();
    setLayout(layout);
}

void CCutWidget::initConnection()
{
    connect(m_scaleBtn1_1, &DPushButton::clicked, this, [ = ]() {
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(ButtonClickAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(cut_1_1);
        CManagerAttributeService::getInstance()->doCut();

        clearAllChecked();
        m_scaleBtn1_1->setChecked(true);

        this->updateCutSize();
    });

    connect(m_scaleBtn2_3, &DPushButton::clicked, this, [ = ]() {
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(ButtonClickAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(cut_2_3);
        CManagerAttributeService::getInstance()->doCut();

        clearAllChecked();
        m_scaleBtn2_3->setChecked(true);

        this->updateCutSize();
    });

    connect(m_scaleBtn8_5, &DPushButton::clicked, this, [ = ]() {
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(ButtonClickAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(cut_8_5);
        CManagerAttributeService::getInstance()->doCut();

        clearAllChecked();
        m_scaleBtn8_5->setChecked(true);

        this->updateCutSize();
    });

    connect(m_scaleBtn16_9, &DPushButton::clicked, this, [ = ]() {
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(ButtonClickAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(cut_16_9);
        CManagerAttributeService::getInstance()->doCut();

        clearAllChecked();
        m_scaleBtn16_9->setChecked(true);

        this->updateCutSize();
    });

    connect(m_freeBtn, &DPushButton::clicked, this, [ = ]() {
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(ButtonClickAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(cut_free);

        clearAllChecked();
        m_freeBtn->setChecked(true);

        CManagerAttributeService::getInstance()->doCut();
    });

    connect(m_originalBtn, &DPushButton::clicked, this, [ = ]() {
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(ButtonClickAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(cut_original);
        CManagerAttributeService::getInstance()->doCut();

        clearAllChecked();
        m_originalBtn->setChecked(true);

        this->updateCutSize();
    });

    connect(m_widthEdit, &DLineEdit::focusChanged, this, [ = ](bool isfocus) {
        emit signalCutLineEditIsfocus(isfocus);
    });

    connect(m_heightEdit, &DLineEdit::focusChanged, this, [ = ](bool isfocus) {
//        qDebug() << "@@@@@@@@@@@@@HeightFocus=" << isfocus;
        emit signalCutLineEditIsfocus(isfocus);
    });

    connect(m_widthEdit, &DLineEdit::editingFinished, this, [ = ]() {

        if (m_widthEdit->lineEdit()->hasFocus()) {
            m_widthEdit->lineEdit()->clearFocus();
            return ;
        }

        activeFreeMode();

        int w = m_widthEdit->text().trimmed().toInt();
        int h = m_heightEdit->text().trimmed().toInt();
        if (w > 4096) {
            w = 4096;
            m_widthEdit->setText(QString::number(w));
        }
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(LineEditeAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutSize(QSize(w, h));
        CManagerAttributeService::getInstance()->doCut();

    });

    connect(m_heightEdit, &DLineEdit::editingFinished, this, [ = ]() {

        if (m_heightEdit->lineEdit()->hasFocus()) {
            m_heightEdit->lineEdit()->clearFocus();
            return;
        }

        activeFreeMode();

        int w = m_widthEdit->text().trimmed().toInt();
        int h = m_heightEdit->text().trimmed().toInt();
        if (h > 4096) {
            h = 4096;
            m_heightEdit->setText(QString::number(h));

        }
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutAttributeType(LineEditeAttribute);
        CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutSize(QSize(w, h));
        CManagerAttributeService::getInstance()->doCut();

    });

    connect(m_cutBtn, &DPushButton::clicked, this, [ = ]() {

        if (m_widthEdit->lineEdit()->hasFocus()) {
            m_widthEdit->lineEdit()->clearFocus();
        }

        if (m_heightEdit->lineEdit()->hasFocus()) {
            m_heightEdit->lineEdit()->clearFocus();
        }

        if (nullptr != CManageViewSigleton::GetInstance()->getCurView()->scene()) {
            auto curScene = static_cast<CDrawScene *>(CManageViewSigleton::GetInstance()->getCurView()->scene());
            curScene->doCutScene();
            CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCurrentDrawToolMode(selection);
            CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(ECutType::cut_done);
            emit curScene->signalChangeToSelect();
        }
    });

    connect(m_cancelBtn, &DPushButton::clicked, this, [ = ]() {
        if (nullptr != CManageViewSigleton::GetInstance()->getCurView()->scene()) {
            auto curScene = static_cast<CDrawScene *>(CManageViewSigleton::GetInstance()->getCurView()->scene());
            curScene->quitCutMode();
            CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCurrentDrawToolMode(selection);
            CManageViewSigleton::GetInstance()->getCurView()->getDrawParam()->setCutType(ECutType::cut_done);
            emit curScene->signalChangeToSelect();
        }
    });

    connect(m_SizeAddAction, &QAction::triggered, this, [ = ](bool) {
        if (m_widthEdit->lineEdit()->hasFocus()) {
            int widthSizeNum = m_widthEdit->lineEdit()->text().trimmed().toInt();
            widthSizeNum++;
            if (widthSizeNum > 4096) {
                return ;
            }
            QString text = QString::number(widthSizeNum);
            m_widthEdit->setText(text);
            emit m_widthEdit->lineEdit()->textEdited(text);
        } else if (m_heightEdit->lineEdit()->hasFocus()) {
            int heightSizeNum = m_heightEdit->lineEdit()->text().trimmed().toInt();
            heightSizeNum++;
            if (heightSizeNum > 4096) {
                return ;
            }
            QString text = QString::number(heightSizeNum);
            m_heightEdit->setText(text);
            emit m_heightEdit->lineEdit()->textEdited(text);
        }
    });

    connect(m_SizeReduceAction, &QAction::triggered, this, [ = ](bool) {
        if (m_widthEdit->lineEdit()->hasFocus()) {
            int widthSizeNum = m_widthEdit->lineEdit()->text().trimmed().toInt();
            widthSizeNum--;
            if (widthSizeNum < 10) {
                return ;
            }
            QString text = QString::number(widthSizeNum);
            m_widthEdit->setText(text);
            emit m_widthEdit->lineEdit()->textEdited(text);
        } else if (m_heightEdit->lineEdit()->hasFocus()) {
            int heightSizeNum = m_heightEdit->lineEdit()->text().trimmed().toInt();
            heightSizeNum--;
            if (heightSizeNum < 10) {
                return ;
            }
            QString text = QString::number(heightSizeNum);
            m_heightEdit->setText(text);
            emit m_heightEdit->lineEdit()->textEdited(text);
        }
    });
}

void CCutWidget::activeFreeMode()
{
    if (!m_freeBtn->isChecked()) {
        emit m_freeBtn->clicked(true);
    }
}
