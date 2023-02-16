/* application specific set of properties, derived from fAppConfig!!!
*/

#include "jconfig.h"
#include "deconfig.h"


DEConfig::DEConfig(QWidget *parent, const QString &conffname, const QString &jfolder)
: fAppConfig(parent, conffname, jfolder)
{
    loadUserProperies(jfolder);
}

void DEConfig::resetPortTimeout()
{
    setSpinnerRangeAndStep(&m_portTimeout, 100, 2000, 100);
}

void DEConfig::resetAppPluginsPath()
{
    setDirectory(&m_appPluginsPath, "Select applications plugins path...");
}

void DEConfig::resetLibPluginsPath()
{
    setDirectory(&m_libPluginsPath, "Select library plugins path...");
}

void DEConfig::resetOfflineStoragePath()
{
    setDirectory(&m_offlineStoragePath, "Select offline storage path...");
}

int DEConfig::getPortTimeout() const
{
    return m_portTimeout;
}

QString DEConfig::getAppPluginsPath() const
{
    return m_appPluginsPath;
}

QString DEConfig::getLibPluginsPath() const
{
    return m_libPluginsPath;
}

QString DEConfig::getOfflineStoragePath() const
{
    return m_offlineStoragePath;
}

