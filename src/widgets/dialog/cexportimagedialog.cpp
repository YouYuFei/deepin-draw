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
#include "cexportimagedialog.h"
#include "widgets/csvglabel.h"

#include <DFileDialog>
#include <DDialog>
#include <DMessageBox>

#include <QFormLayout>
#include <QImageWriter>
#include <QStandardPaths>
#include <QPdfWriter>
#include <QPainter>
#include <QCloseEvent>
#include <QDebug>
#include <QDateTime>
#include <QAbstractButton>
#include "application.h"
#include  "dialog.h"

const QSize DIALOG_SIZE = QSize(380, 280);
const QSize LINE_EDIT_SIZE = QSize(250, 35);

CExportImageDialog::CExportImageDialog(DWidget *parent)
    : DDialog(parent)
{
    initUI();
    initConnection();
}

CExportImageDialog::~CExportImageDialog()
{

}

void CExportImageDialog::showMe()
{
    m_fileNameEdit->setText(tr("Unnamed.jpg"));


    if (m_savePathCombox->count() == Other + 1) {
        m_savePathCombox->blockSignals(true);
        m_savePathCombox->removeItem(Other);
    }
    m_savePathCombox->blockSignals(false);

    m_savePathCombox->setCurrentIndex(Pictures);
    m_formatCombox->setCurrentIndex(JPG);
    m_qualitySlider->setValue(100);


    slotOnSavePathChange(Pictures);
    slotOnFormatChange(JPG);
    slotOnQualityChanged(m_qualitySlider->value());

    show();
}

int CExportImageDialog::getImageType() const
{
    return  m_formatCombox->currentIndex();
}

QString CExportImageDialog::getImageFormate() const
{
    return  m_saveFormat;
}

int CExportImageDialog::getQuality() const
{
    return m_quality;
}

void CExportImageDialog::initUI()
{
    drawApp->setWidgetAccesibleName(this, "Export dialog");
    setFixedSize(DIALOG_SIZE);
    setModal(true);

    setContentsMargins(0, 0, 0, 0);

    CSvgLabel *logoLable = new CSvgLabel(":/theme/common/images/logo.svg", this);
    logoLable->setFixedSize(QSize(32, 32));


    logoLable->move(10, 9);
    logoLable->setAlignment(Qt::AlignLeft);

    setWindowTitle(tr("Export"));;


    m_fileNameEdit = new DLineEdit(this);
    drawApp->setWidgetAccesibleName(m_fileNameEdit, "Export name line editor");
    m_fileNameEdit->setFixedSize(LINE_EDIT_SIZE);
    m_fileNameEdit->setClearButtonEnabled(false);
    m_fileNameEdit->lineEdit()->setValidator(new QRegExpValidator(QRegExp("[^\\\\*\?|<>\"//]+")));

    m_savePathCombox = new DComboBox(this);
    drawApp->setWidgetAccesibleName(m_savePathCombox, "Export path comboBox");
    m_savePathCombox->insertItem(Pictures, tr("Pictures"));
    m_savePathCombox->insertItem(Documents, tr("Documents"));
    m_savePathCombox->insertItem(Downloads, tr("Downloads"));
    m_savePathCombox->insertItem(Desktop, tr("Desktop"));
    m_savePathCombox->insertItem(Videos, tr("Videos"));
    m_savePathCombox->insertItem(Music, tr("Music"));
    m_savePathCombox->insertItem(UsrSelect, tr("Select other directories"));
    m_savePathCombox->setFixedSize(LINE_EDIT_SIZE);


    m_formatCombox = new DComboBox(this);
    drawApp->setWidgetAccesibleName(m_formatCombox, "Export format comboBox");
    m_formatCombox->insertItem(JPG, tr("jpg"));
    m_formatCombox->insertItem(PNG, tr("png"));
    m_formatCombox->insertItem(BMP, tr("bmp"));
    m_formatCombox->insertItem(TIF, tr("tif"));
    m_formatCombox->insertItem(PDF, tr("pdf"));
    m_formatCombox->setFixedSize(LINE_EDIT_SIZE);

    m_qualitySlider = new DSlider(Qt::Horizontal, this);
    drawApp->setWidgetAccesibleName(m_qualitySlider, "Export quality slider");
    m_qualitySlider->setMinimum(1);
    m_qualitySlider->setMaximum(100);
    m_qualitySlider->setValue(100);
    m_qualitySlider->setFixedSize(QSize(120, LINE_EDIT_SIZE.height()));

    m_qualityLabel = new DLabel(this);
    m_qualityLabel->setFixedSize(QSize(45, LINE_EDIT_SIZE.height()));

    QHBoxLayout *qualityHLayout = new QHBoxLayout;
    qualityHLayout->setMargin(0);
    qualityHLayout->setSpacing(0);
    qualityHLayout->addSpacing(3);
    qualityHLayout->addWidget(m_qualitySlider);
    qualityHLayout->addSpacing(20);
    qualityHLayout->addWidget(m_qualityLabel);

    DWidget *contentWidget = new DWidget(this);
    drawApp->setWidgetAccesibleName(contentWidget, "Export content widget");
//    contentWidget->setStyleSheet("background-color: rgb(255, 0, 0);");
    contentWidget->setContentsMargins(0, 0, 0, 0);
    QFormLayout *fLayout = new QFormLayout(contentWidget);
    fLayout->setFormAlignment(Qt::AlignJustify);
    fLayout->setHorizontalSpacing(10);
    fLayout->addRow(tr("Name:"), m_fileNameEdit);
    fLayout->addRow(tr("Save to:"), m_savePathCombox);
    fLayout->addRow(tr("Format:"), m_formatCombox);
    fLayout->addRow(tr("Quality:"), qualityHLayout);
    addContent(contentWidget);

    addButton(tr("Cancel"), false, DDialog::ButtonNormal);
    m_saveBtnId = addButton(tr("Save"), true, DDialog::ButtonRecommend);

    m_questionDialog = new DDialog(this);
    m_questionDialog->setIcon(QIcon::fromTheme("dialog-warning"));
    m_questionDialog->setModal(true);
    m_questionDialog->addButtons(QStringList() << tr("Cancel") << tr("Replace"));
    m_questionDialog->setFixedSize(400, 170);

    setOnButtonClickedClose(false);

//    setLayout(titleLayout);
}

void CExportImageDialog::initConnection()
{
    connect(m_savePathCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotOnSavePathChange(int)), Qt::QueuedConnection);
    connect(m_formatCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotOnFormatChange(int)));
    connect(this, SIGNAL(buttonClicked(int, const QString &)), this, SLOT(slotOnDialogButtonClick(int, const QString &)));
    connect(m_qualitySlider, SIGNAL(valueChanged(int)), this, SLOT(slotOnQualityChanged(int)));
    connect(m_questionDialog, SIGNAL(buttonClicked(int, const QString &)), this,
            SLOT(slotOnQuestionDialogButtonClick(int, const QString &)));

    //设置的文件名为空时应该要设置保存按钮为disable
    connect(m_fileNameEdit, &DLineEdit::textChanged, this, [ = ](const QString & text) {

        QString newText = text;

        //        newText.remove(dApp->fileNameRegExp());

        //        m_fileNameEdit->blockSignals(true);
        //        m_fileNameEdit->setText(newText);
        //        m_fileNameEdit->blockSignals(false);

        bool isEmpty = newText.isEmpty();

        if (m_saveBtnId != -1) {
            QAbstractButton *pBtn = getButton(m_saveBtnId);

            if (pBtn != nullptr) {
                pBtn->setEnabled(!isEmpty);
            }
        }
    });

}

void CExportImageDialog::slotOnSavePathChange(int index)
{
    switch (index) {
    case Pictures:
        m_savePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
        break;
    case Documents:
        m_savePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        break;
    case Downloads:
        m_savePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        break;
    case Desktop:
        m_savePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        break;
    case Videos:
        m_savePath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        break;
    case Music:
        m_savePath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
        break;
    case UsrSelect:
        showDirChoseDialog();
        break;
    case Other:
        m_savePath = m_savePathCombox->itemText(index);
        break;
    default:
        m_savePath = "";
        break;
    }
}

void CExportImageDialog::slotOnFormatChange(int index)
{
    switch (index) {
    case PDF:
    case BMP:
    case TIF:
        m_qualitySlider->setValue(100);
        m_qualitySlider->setEnabled(false);
        break;
    case JPG:
    case PNG:
        m_qualitySlider->setEnabled(true);
        break;
    default:
        break;
    }

    m_saveFormat = m_formatCombox->itemText(index);

    QString name = m_fileNameEdit->text().trimmed();

    if (isHaveSuffix(name)) {
        name = name.mid(0, name.lastIndexOf(".") + 1);
        name += m_saveFormat;
    } else {
        name = name + "." + m_saveFormat;
    }

    m_fileNameEdit->setText(name);
}

void CExportImageDialog::slotOnDialogButtonClick(int index, const QString &text)
{
    Q_UNUSED(text)

    if (index == 1) {
        //  [BUG: 30843] 希望在导出时，名字以点开头的图片，也能给到提示：以点开始会被隐藏
        QString fileName = m_fileNameEdit->text().trimmed();
        if (fileName.startsWith(".")) {
            this->hide();
            Dialog dialog(this);
            dialog.setModal(true);
            dialog.setIcon(QPixmap(":/icons/deepin/builtin/Bullet_window_warning.svg"));
            dialog.setMessage(tr("This file will be hidden if the file name starts with a dot (.). Do you want to hide it?"));
            dialog.addButton(tr("Cancel"), false, DDialog::ButtonNormal);
            dialog.addButton(tr("Confirm"), true, DDialog::ButtonRecommend);
            dialog.showInCenter(this);
            int status = dialog.exec();
            if (status != 1) {
                this->show();
                return;
            }
        }

        QString completePath = getCompleteSavePath();
        // 判断路径是否超过255字符
        if (completePath.toLocal8Bit().length() > 255) {
            Dtk::Widget::DDialog dialog(this);
            dialog.setTextFormat(Qt::RichText);
            dialog.addButton(tr("OK"));
            dialog.setIcon(QIcon(":/icons/deepin/builtin/Bullet_window_warning.svg"));
            dialog.setMessage(tr("The file name is too long"));
            dialog.exec();
            return;
        }

        if (completePath == "") {
            return;
        }
        if (QFileInfo(completePath).exists()) {
            hide();
            showQuestionDialog(completePath);
        } else {
            emit signalDoSave(completePath);
            hide();
        }
    } else if (index == 0) {
        hide();
    }
}

void CExportImageDialog::slotOnQuestionDialogButtonClick(int index, const QString &text)
{
    Q_UNUSED(text);
    if (index == 1) {
        QString completePath = getCompleteSavePath();
        emit signalDoSave(completePath);
    }
    m_questionDialog->hide();
}

void CExportImageDialog::slotOnQualityChanged(int value)
{
    m_qualityLabel->setText(QString("%1%").arg(value));
    m_quality = value;
}

void CExportImageDialog::showDirChoseDialog()
{
    DFileDialog dialog(this);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    dialog.setViewMode(DFileDialog::Detail);
    dialog.setFileMode(DFileDialog::DirectoryOnly);
    if (dialog.exec()) {
        QString fileDir = dialog.selectedFiles().first();
        if (m_savePathCombox->count() < Other + 1) {
            m_savePathCombox->insertItem(Other, fileDir);
        } else {
            m_savePathCombox->setItemText(Other, fileDir);
        }
        m_savePathCombox->setCurrentText(fileDir);
    }
}

void CExportImageDialog::showQuestionDialog(const QString &path)
{
    //“XXX”已经存在，您是否要替换？
    m_questionDialog->setMessage((QString(tr("%1 \n already exists, do you want to replace it?")).arg(path)));
    m_questionDialog->show();
}

bool CExportImageDialog::isHaveSuffix(const QString &src)
{
    bool flag = false;
    if (src.endsWith(".jpg") || src.endsWith(".png") || src.endsWith(".bmp")
            || src.endsWith(".tif") || src.endsWith(".pdf")) {
        flag = true;
    }
    return flag;
}

QString CExportImageDialog::getCompleteSavePath()
{
    QString fileName = m_fileNameEdit->text().trimmed();

    if (fileName.isEmpty() || fileName == "") {
        return "";
    }
    if (!isHaveSuffix(fileName)) {
        fileName = fileName + "." + m_formatCombox->currentText();
    }

    return m_savePath + "/" + fileName;
}
