#ifndef DESETTINGS_H
#define DESETTINGS_H

#include "config.h"
#include "MainWindow.h"

#include <QObject>

#ifndef SETTINGS_JSON_NAME
#define SETTINGS_JSON_NAME "DESettings"
#endif

class DESettings : public Config
{
    Q_OBJECT

    Q_PROPERTY(int MainWindowWidth READ MainWindowWidth WRITE setMainWindowWidth RESET resetMainWindowWidth)
    Q_PROPERTY(int MainWindowHeight READ MainWindowHeight WRITE setMainWindowHeight RESET resetMainWindowHeight)

public:
    DESettings(MainWindow *parent, const char *configFName);
    ~DESettings();

    int MainWindowWidth() {return mainWindowWidth;}
    void setMainWindowWidth(int width) {mainWindowWidth = width;}

    int MainWindowHeight() {return mainWindowHeight;}
    void setMainWindowHeight(int width) {mainWindowHeight = width;}


signals:

public slots:

public:

private:
    const char * const SettingsJsonName = SETTINGS_JSON_NAME;

    void resetMainWindowWidth(void) {mainWindowWidth = 800;}
    void resetMainWindowHeight(void) {mainWindowHeight = 600;}

    QVariantMap configmap;

    bool isToBeSaved = false;
    int mainWindowWidth;
    int mainWindowHeight;
    MainWindow *Parent;

};

#endif // DESETTINGS_H
