#ifndef LOGTREE_H
#define LOGTREE_H

#include <QDockWidget>
#include <QTreeWidget>

#ifndef PATH_SEPARATOR_CHAR
#define PATH_SEPARATOR_CHAR '|'
#endif

class Log : public QDockWidget
{
    Q_OBJECT
public:
    Log(QMainWindow *parent = 0);

    static QTreeWidgetItem * Add(const QString &message);
    static QTreeWidgetItem * Add(const QString &message, const QString &icon);
    static QTreeWidgetItem * Add(QStringList complexMessage, const QString &icon);
    static QTreeWidgetItem * Add(QTreeWidgetItem *parent, const QString &message, const QString &icon);

//    static QTreeWidgetItem * addError(const QString &message);
//    static QTreeWidgetItem * addWarning(const QString &message);
//    static QTreeWidgetItem * addInfo(const QString &message);

    static QTreeWidgetItem * AddDebugInfo(const QString &message);

    static QString HexToString(const QByteArray &source, const QString &delimeter = "%1-");

    static const char PathSeparator = PATH_SEPARATOR_CHAR;

    inline static QTreeWidget* TreePtr() { return Log::treePtr; }

signals:

public slots:
    static void* /*QTreeWidgetItem**/ addError(const QString &message);
    static void* /*QTreeWidgetItem**/ addWarning(const QString &message);
    static void* /*QTreeWidgetItem**/ addInfo(const QString &message);

private slots:
    void ClearLog(QTreeWidgetItem *item, int column);
private:
    QTreeWidget* tree;

    static int DebugInfoCounter;
    static QTreeWidget* treePtr;

    //inline static void setTreePtr(QTreeWidget* treeptr) { Log::treePtr = treeptr; }
};

#endif // LOGTREE_H
