#ifndef FAPPCONFIG_H
#define FAPPCONFIG_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>

class JConfig;
class fAppConfig : public QDialog
{
    Q_OBJECT
public:
    explicit fAppConfig(QWidget *parent = 0, const QString& conffname = QString(), const QString& jobject_name = QString("AppConfig"));

    JConfig *jconfig() const;
    QString cfg_fname() const;

    void setJconfig(JConfig *jconfig);
    void setConfig_fname(const QString &cfg_fname);

    void loadUserProperies(const QString &jfolder);
    QString toAbsolutePath(const QString &relativeOrAbsolutePath);

signals:

public slots:
    void showUI();

protected:
    void setSpinnerRangeAndStep(int* vptr, const int minv, const int maxv, const int step);
    void setFileName(QString* vptr, const QString &caption = QString(), const QString &filter = QString(), const QString &dir = QString());
    void setDirectory(QString* vptr, const QString &caption = QString(), const QString &dir = QString(), QFileDialog::Options options = QFileDialog::ShowDirsOnly);

private:
    JConfig *m_jconfig;
    QString m_cfg_fname;
    QString m_jobject_name;
    QVBoxLayout *topLayout = 0;
    QMap<int, QWidget*> user_props_map;

//func...
    QComboBox* createComboBox(const QMetaProperty &p);
    bool updateComboBox(QComboBox* cbox, const QMetaProperty &p);

    QCheckBox* createCheckBox(const QMetaProperty &p, const QString &suffix = QString());
    bool updateCheckBox(QCheckBox *check, const QMetaProperty &p);

    QLineEdit *createLineEdit(const QMetaProperty &p);
    bool updateLineEdit(QLineEdit *line, const QMetaProperty &p);

    QSpinBox *createSpinBox(const QMetaProperty &p, const QString& suffix = QString());
    bool updateSpinBox(QSpinBox *sbox, const QMetaProperty &p);

    void createUI();
    void updateUI();
    QVariant takeNewValue(const QMetaProperty& p);
    void updateConfigFileAndObject_fromUI();
};

#endif // FAPPCONFIG_H
