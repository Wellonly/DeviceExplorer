/*
 ** This file is part of the DE project.
 ** Copyright 2017 Well <iconn8@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QMainWindow>


#include "smo.h"
#include "dewidget.h"

DEWidget::DEWidget(QMainWindow *parent, Smo *smo, const QString &idname)
: QDockWidget(parent)
, m_smo(smo)
{
    setObjectName(idname); //must be defined!!!
    setMinimumSize(150,150);
    setWindowTitle(Smo::Path(smo, path_absolute));
}

void DEWidget::DEHide_slot()
{
    if (!isFloating()) {
        hide();
    }
}

void DEWidget::DERaise_slot(Smo *smo, bool isFormToBeRefreshed)
{
    m_refreshTime = QDateTime::currentDateTime();
    if (m_smo != smo) {
        m_smo = smo;
        isFormToBeRefreshed = true;
        setWindowTitle(Smo::Path(smo, path_absolute));
    }
    if (isSmoToBeRefreshed && isFormToBeRefreshed) {
        if (smo->isExpanded()) {
            Smo::RefreshChildren(smo);
        }
    }

    Form_refresh(smo, isFormToBeRefreshed);
    show();
}

void DEWidget::Form_refresh(Smo* /*smo*/, bool /*isFormToBeRefreshed*/)
{
}

Smo *DEWidget::smoRef() const
{
    return m_smo;
}

void DEWidget::setSmoRef(Smo *value)
{
    m_smo = value;
}

void DEWidget::updateRefreshTime()
{
    m_refreshTime = QDateTime::currentDateTime();
}

QDateTime DEWidget::refreshTime() const
{
    return m_refreshTime;
}

