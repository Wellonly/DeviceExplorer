#ifndef APPCONF_H
#define APPCONF_H

#include <stdbool.h>
#include <QObject>

#include "fappconfig.h"

#ifndef APP_JCONFIG_FOLDER_NAME
#define APP_JCONFIG_FOLDER_NAME "DEConfig"
#endif

class DEConfig : public fAppConfig
{
    Q_OBJECT
    /* NOTE: each property name has a special meaning! See: fappconfig.cpp!!! */
    Q_PROPERTY(int Port_timeout__mS MEMBER m_portTimeout RESET resetPortTimeout NOTIFY configChanged USER true)
    Q_PROPERTY(QString QML_applications_path MEMBER m_appPluginsPath RESET resetAppPluginsPath NOTIFY configChanged USER true)
    Q_PROPERTY(QString QML_library_path MEMBER m_libPluginsPath RESET resetLibPluginsPath NOTIFY configChanged USER true)
    Q_PROPERTY(QString Offline_storage_path MEMBER m_offlineStoragePath RESET resetOfflineStoragePath NOTIFY configChanged USER true)

    Q_PROPERTY(bool Option_bool MEMBER m_abool NOTIFY configChanged USER true)
    Q_PROPERTY(enumTst EnumTst MEMBER m_enum NOTIFY configChanged USER true)

public:
    DEConfig(QWidget *parent = 0, const QString& conffname = QString(), const QString& jfolder = QString(APP_JCONFIG_FOLDER_NAME));

    enum enumTst {
        enumTst0 = 0,
        enumTst1 = 111,
        enumTst2 = 22,
        enumTst3 = 3,
    };
    Q_ENUM(enumTst)

    int getPortTimeout() const;
    QString getAppPluginsPath() const;
    QString getLibPluginsPath() const;
    QString getOfflineStoragePath() const;

signals:
    void configChanged();

private:
    int m_portTimeout = 200;
    QString m_appPluginsPath;
    QString m_libPluginsPath;
    QString m_offlineStoragePath;

    bool m_abool = false;
    enumTst m_enum = enumTst2;

    void resetPortTimeout();
    void resetAppPluginsPath();
    void resetLibPluginsPath();
    void resetOfflineStoragePath();

};

#endif // APPCONF_H
