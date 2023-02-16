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

//NOTE: QObject::moveToThread: Widgets cannot be moved to a new thread

#include <QQmlContext>
#include <QQmlEngine>

#include "smo.h"
#include "LogTree.h"
#include "zvvdebug.h"

#include "deconfig.h"

#include "MainWindow.h"
#include "fdockqml.h"

fDockedQml::fDockedQml(QMainWindow *parent, Smo *smo, const QString &idname, const QString &smoqml)
: DEWidget(parent, smo, idname)
{
    m_finfo.setFile(smoqml);

    m_qmlurl.setUrl(smoqml);
    isSmoToBeRefreshed = true;

    DERaise_slot(smo, true); //by this we will call: Form_refresh(smo, true);

}

fDockedQml::~fDockedQml()
{
    QMLWidgetUnload(m_quickWidget);
}

void fDockedQml::quickWidgetStatusChanged(QQuickWidget::Status status)
{
    if (status == QQuickWidget::Error && m_quickWidget) {
        QStringList errors;
        foreach (const QQmlError &error, m_quickWidget->errors())
            errors.append(error.toString());
        Log::addError(errors.join(QStringLiteral(", ")));
    }
}

void fDockedQml::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    Log::addError(message); // statusBar()->showMessage(message);
}

void fDockedQml::Form_refresh(Smo *smo, bool isFormToBeRefreshed)
{
    (void)smo;
    (void)isFormToBeRefreshed;
    //TODO:...
    QDateTime fdate = m_finfo.lastModified();
    m_finfo.refresh();
    if (m_finfo.lastModified() != fdate || !m_quickWidget) {
        if (m_quickWidget) {
            QMLWidgetUnload(m_quickWidget);
            m_quickWidget = 0;
        }
        if (QFile::exists(m_finfo.absoluteFilePath())) {
            QMLWidgetLoad(m_qmlurl);
        }
    }
}

QQuickWidget* fDockedQml::QMLWidgetLoad(const QUrl &url)
{
    m_quickWidget = new QQuickWidget;
    connect(m_quickWidget, &QQuickWidget::statusChanged, this, &fDockedQml::quickWidgetStatusChanged);
    connect(m_quickWidget, &QQuickWidget::sceneGraphError, this, &fDockedQml::sceneGraphError);

    QQmlContext* ctx = m_quickWidget->rootContext();
    QQmlEngine* eng = ctx->engine();

// set up configs...
    DEConfig* cfg = qobject_cast<DEConfig*>(((MainWindow*)parentWidget())->getAppConf());
    if (cfg) {
        QString libPath = cfg->getLibPluginsPath();
        if (!libPath.isEmpty()) eng->addPluginPath(libPath);
        QString offlineStoragePath = cfg->getOfflineStoragePath();
        if (!offlineStoragePath.isEmpty()) eng->setOfflineStoragePath(offlineStoragePath);
    }

    debug_func("offlineStoragePath:" << eng->offlineStoragePath());
    debug_func("pluginPathList:" << ctx->engine()->pluginPathList());
// end set up configs

// setContextProperty section...
    Smo* smo = smoRef();
    ctx->setContextProperty("smo", QVariant::fromValue(smo));

//end setContextProperty section

    m_quickWidget->resize(minimumSize());
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    m_quickWidget->setSource(url);
    setWidget(m_quickWidget);
    return m_quickWidget;
}

void fDockedQml::QMLWidgetUnload(QQuickWidget* quickWidget)
{
    if (quickWidget) {
        disconnect(quickWidget);
        quickWidget->deleteLater(); // delete quickWidget;
//        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

