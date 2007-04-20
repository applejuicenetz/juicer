/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef QAJOPTIONSDIALOG_H
#define QAJOPTIONSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "ui_qajoptionsdialogbase.h"

#include "types.h"

class QAjOptionsDialog : public QDialog, public Ui::QAjOptionsDialogBase
{
    Q_OBJECT

public:
    QAjOptionsDialog( QWidget* parent = 0 );
    ~QAjOptionsDialog();

    AjSettings getAjSettings();
    void setAjSettings( AjSettings settings );

    QString defaultLauncher;

    class IconWidget : public QWidget
    {
      public:
        IconWidget(QString icon, QString text, QWidget* parent = 0) : QWidget(parent)
        {
            iconLabel = new QLabel(this);
            iconLabel->setPixmap(QPixmap(icon));
            iconLabel->setMinimumSize(QSize(32,32));

            textLabel = new QLabel(text, this);
            textLabel->adjustSize();

            QVBoxLayout *layout = new QVBoxLayout;
            layout->addWidget(iconLabel, 0, Qt::AlignHCenter);
            layout->addWidget(textLabel, 0, Qt::AlignHCenter);

            this->setLayout(layout);
            this->adjustSize();
        }
        QLabel *iconLabel, *textLabel;
    };

protected slots:
    void selectIncomingDir();
    void selectTempDir();
    void selectIncomingDirSpecific();
    void selectTempDirSpecific();
    void selectLauncher();
    void specificRadioToggled( bool checked );
};

#endif

