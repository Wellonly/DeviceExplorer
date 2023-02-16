#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

#include "c_cpp_macros.h"
#include "LogTree.h"
#include "treelib.h"
#include "zvvlib.h"

using namespace TreeLib;

QTreeWidget* Log::treePtr = 0;
int Log::DebugInfoCounter = 0;
//Log * Log::treePtr = 0;

Log::Log(QMainWindow *parent) : QDockWidget(parent)
{
    setObjectName("MainLog");
    setWindowTitle("Log");

    setMinimumSize(150,50);

    Log::treePtr = tree = new QTreeWidget(this);
    tree->setHeaderHidden(true);

    setWidget(tree);

    connect(tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(ClearLog(QTreeWidgetItem*,int)));
//    show();
}

/// cMess format: "nodeText[|child1NodeText[|child2NodeText]]
QTreeWidgetItem *Log::Add(const QString& message)
{
    return Add(message, QString("info"));
}

QTreeWidgetItem *Log::Add(const QString& message, const QString& icon)
{
    if (message.isEmpty()) {
        return Add(QStringList() << "NULL", icon);
    }
    return Add(message.split(Log::PathSeparator), icon);
}

QTreeWidgetItem *Log::Add(QStringList complexMessage, const QString& icon)
{
    QTreeWidgetItem *newNode;
    QTreeWidgetItem *node = 0;
    foreach (QString mess, complexMessage) {
        if (node) {
            newNode = TreeLib::FindChildNodeOnText(node, mess);
            if (newNode == 0) newNode = Add(node, mess, iif(icon.isEmpty(), "info", icon));
            node = newNode;
        }
        else { //initional...
            if (treePtr) { //if Log initialized...
                node = FindTopLevelNodeOnText(treePtr, mess);
                if (node == 0) {
                    node = new QTreeWidgetItem; //(QTreeWidgetItem::UserType)
                    node->setText(0, mess);
                    node->setIcon(0, zvvlib::getIcon(iif(icon.isEmpty(), "info", icon)));
                    treePtr->addTopLevelItem(node);
                    node->setExpanded(true); //expand only after adding!!!
                }
            }
            else {
                QMessageBox::information(0, "Log system...", "Log has not initialized!!!");
                return 0;
            }
        }
    }
    return node;
}

QTreeWidgetItem *Log::Add(QTreeWidgetItem *parent, const QString& message, const QString& icon)
{
    QTreeWidgetItem *newNode = new QTreeWidgetItem;
    newNode->setExpanded(true);
    newNode->setText(0, message);
    newNode->setIcon(0, zvvlib::getIcon(icon));
    parent->addChild(newNode);
    return newNode;
}

void Log::ClearLog(QTreeWidgetItem *item, int column)
{
    qDebug() << "ClearLog: " << QDateTime::currentDateTime().toString();
    if (item->text(column).isEmpty()) {
        Log::addInfo("DO NOT click everywhere!!!");
    } else {
        this->tree->clear();
    }
}

QTreeWidgetItem *Log::AddDebugInfo(const QString& message)
{
#ifdef QT_DEBUG
    return Log::Add(QString("Debug%1%2 %3").arg(PATH_SEPARATOR_CHAR).arg(QDateTime::currentDateTime().toString(DateTime_format_full)).arg(message), "info");
#else
    (void)message;
    return 0;
#endif
}

QString Log::HexToString(const QByteArray &source, const QString& delimeter)
{
    QString ret = "";
    for(int i = 0; i < source.length(); ++i) {
        ret = ret + delimeter.arg((uchar)source.at(i), 2, 16, QChar('0'));
    }
    return ret;
}

void* Log::addInfo(const QString& message)
{
    return Log::Add(QString("Information%1%2 %3").arg(PATH_SEPARATOR_CHAR).arg(QDateTime::currentDateTime().toString(DateTime_format_full)).arg(message), "info");
}

void* Log::addWarning(const QString& message)
{
    return Log::Add(QString("Warnings%1%2 %3").arg(PATH_SEPARATOR_CHAR).arg(QDateTime::currentDateTime().toString(DateTime_format_full)).arg(message), "warning");
}

void *Log::addError(const QString& message)
{
    return Log::Add(QString("Errors%1%2 %3").arg(PATH_SEPARATOR_CHAR).arg(QDateTime::currentDateTime().toString(DateTime_format_full)).arg(message), "error");
}
