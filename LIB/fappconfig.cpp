/* usage:
 * 1) derive from this (see: example APPConf.cpp in DE project!)
 * define a user properties:
 *   use one underline char for space in param name (Ex.: folder_name will transform to: folder name)
 *   use double underline char for param suffix in param name (Ex.: time__mS will transform to: time, mS)
 *   use individual RESET routines for each property!
 *   and finaly, use USER true for each property!
 *   Examles:
     Q_PROPERTY(int Port_timeout__mS MEMBER m_portTimeout RESET resetPortTimeout NOTIFY configChanged USER true)
     Q_PROPERTY(QString QML_library_path MEMBER m_libPluginsPath RESET resetLibPluginsPath NOTIFY configChanged USER true)
     Q_PROPERTY(bool Option_bool MEMBER m_abool NOTIFY configChanged USER true)
     Q_PROPERTY(enumTst EnumTst MEMBER m_enum NOTIFY configChanged USER true)
*/

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QApplication>
#include <limits.h>

#include "zvvlib.h"
#include "zvvenum.h"
#include "LogTree.h"
#include "jconfig.h"
#include "fappconfig.h"

fAppConfig::fAppConfig(QWidget *parent, const QString &conffname, const QString &jobject_name)
: QDialog(parent)
, m_cfg_fname(conffname)
, m_jobject_name(jobject_name)
, topLayout(0)
{
    m_jconfig = new JConfig(this, conffname); //NOTE: if config file is not found then it will be created!!!
}

void fAppConfig::showUI()
{
    if (topLayout) {
        updateUI();
    }
    else
    {
        createUI();
    }
    int res = exec();
    if (res == QDialog::Accepted) {
        updateConfigFileAndObject_fromUI();
    }
}

void fAppConfig::updateConfigFileAndObject_fromUI()
{
    const QMetaObject* mo = metaObject();
    QVariantMap vmap;
    QMapIterator<int, QWidget*> i(user_props_map);
    while (i.hasNext()) {
        i.next();
        bool isok = false;
        QWidget* vcontrol = i.value();
        auto p = mo->property(i.key());
        if (p.isEnumType()) {
            isok = p.write(this, ((QComboBox*)vcontrol)->currentData());
            vmap.insert(QString(p.name()), QVariant::fromValue<QString>(((QComboBox*)vcontrol)->currentText()));
        }
        else {
            switch (p.type()) {
            case QVariant::Int:
                isok = p.write(this, QVariant::fromValue<int>(((QSpinBox*)vcontrol)->value()));
                vmap.insert(QString(p.name()), p.read(this));
                break;
            case QVariant::Bool:
                isok = p.write(this, QVariant::fromValue<bool>(((QCheckBox*)vcontrol)->checkState() == Qt::Checked));
                vmap.insert(QString(p.name()), p.read(this));
                break;
            default:
                isok = p.write(this, QVariant::fromValue<QString>(((QLineEdit*)vcontrol)->text()));
                vmap.insert(QString(p.name()), p.read(this));
            }
        }
        if (!isok) {
            //err...
        }
    } //while QMapIterator

    bool IsOk = m_jconfig->save(vmap, m_jobject_name);
    if (!IsOk)
    {
        Log::addError(tr("Config: %1").arg(m_jconfig->lastException()->eMessage));
    }
}

void fAppConfig::setConfig_fname(const QString &cfg_fname)
{
    m_cfg_fname = cfg_fname;
}

QString fAppConfig::toAbsolutePath(const QString &relativeOrAbsolutePath)
{
    if (QDir::isAbsolutePath(relativeOrAbsolutePath)) {
        return relativeOrAbsolutePath;
    }
    QDir dr(qApp->applicationDirPath().append('/').append(relativeOrAbsolutePath));
    if (dr.exists()) {
        return dr.absolutePath();
    }
    return relativeOrAbsolutePath;
}

void fAppConfig::loadUserProperies(const QString &jfolder)
{
    m_jconfig->readToUserProperties(this, metaObject(), jfolder);
}

void fAppConfig::setJconfig(JConfig *jconfig)
{
    m_jconfig = jconfig;
}

QString fAppConfig::cfg_fname() const
{
    return m_cfg_fname;
}

JConfig *fAppConfig::jconfig() const
{
    return m_jconfig;
}

void fAppConfig::createUI()
{
    setWindowTitle(QString("Application config: %1").arg(m_cfg_fname));
    setMinimumSize(640, 480);

    topLayout = new QVBoxLayout(this);

    QGridLayout *inputLayout = new QGridLayout();
    topLayout->addLayout(inputLayout);

    int property_line = 0;
    const QMetaObject* mo = metaObject();
    for (int i = 0; i < mo->propertyCount(); i++) {
        auto p = mo->property(i);
        if (p.isUser()) {
            QLabel* label;
            QString pname(p.name());
            if (pname.indexOf("__") > 0)
            { //with suffix...
                label = new QLabel(pname.replace("__", ", ").replace('_', ' '), this);
            }
            else
            { //without suffix...
                label = new QLabel(pname.replace('_', ' '), this);
            }
            inputLayout->addWidget(label, property_line, 0);
            //end label, next value...
            QWidget* vcontrol;
            if (p.isEnumType()) {
                vcontrol = createComboBox(p);
            }
            else {
                switch (p.type()) {
                case QVariant::Int:
                    vcontrol = createSpinBox(p  /*, "may be suffix"*/);
                    break;
                case QVariant::Bool:
                    vcontrol = createCheckBox(p /*, "may be suffix"*/);
                    break;
                default:
                    vcontrol = createLineEdit(p);
                }
            }
            user_props_map.insert(i, vcontrol);
            inputLayout->addWidget(vcontrol, property_line, 1);
            property_line++;
        }
    }

    inputLayout->setColumnStretch(1, 1);

    topLayout->addStretch();

    QHBoxLayout *buttonBox = new QHBoxLayout();
    topLayout->addLayout(buttonBox);

    QPushButton *saveButton = new QPushButton(tr("Save"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    buttonBox->addStretch();
    buttonBox->addWidget(cancelButton);
    buttonBox->addWidget(saveButton);
}

void fAppConfig::updateUI()
{
    const QMetaObject* mo = metaObject();
    QMapIterator<int, QWidget*> i(user_props_map);
    while (i.hasNext()) {
        i.next();
        bool isok;
        QWidget* vcontrol = i.value();
        auto p = mo->property(i.key()); //key==index
        if (p.isEnumType()) {
            isok = updateComboBox((QComboBox*)vcontrol, p);
        }
        else {
            switch (p.type()) {
            case QVariant::Int:
                isok = updateSpinBox((QSpinBox*)vcontrol, p);
                break;
            case QVariant::Bool:
                isok = updateCheckBox((QCheckBox*)vcontrol, p);
                break;
            default:
                isok = updateLineEdit((QLineEdit*)vcontrol, p);
                break;
            }
        }
        if (!isok) {
            //some err...
        }
    } //while QMapIterator
}

/// \brief fAppConfig::takeNewValue():
/// take New Value using property's reset & then recover old value(for undo reasons)
/// \param targetProperty
/// \return
QVariant fAppConfig::takeNewValue(const QMetaProperty &p)
{
    QVariant oldValue = p.read(this);
    p.reset(this);
    QVariant newValue = p.read(this);
    p.write(this, oldValue);
    return newValue;
}

QComboBox *fAppConfig::createComboBox(const QMetaProperty &p)
{
    QComboBox* combo = new QComboBox(this);
    combo->setEditable(false);
    int currv = p.read(this).toInt();
    QMetaEnum en = p.enumerator();
    int icurr = 0;
    for (int i = 0; i < en.keyCount(); i++) {
        int value = en.value(i);
        combo->addItem(QString(en.key(i)), QVariant::fromValue<int>(value));
        if (currv == value) icurr = i;
    }
    combo->setCurrentIndex(icurr);
    return combo;
}

bool fAppConfig::updateComboBox(QComboBox *cbox, const QMetaProperty &p)
{
    int currv = p.read(this).toInt();
    int idx = cbox->findData(QVariant::fromValue(currv));
    if (idx >= 0) {
        cbox->setCurrentIndex(idx);
        return true;
    }
    return false;
}

QCheckBox *fAppConfig::createCheckBox(const QMetaProperty &p, const QString &suffix)
{
    QCheckBox* check = new QCheckBox(suffix, this);
    check->setChecked(p.read(this).toBool());
    return check;
}

bool fAppConfig::updateCheckBox(QCheckBox *check, const QMetaProperty &p)
{
    bool v = p.read(this).toBool();
    check->setChecked(v);
    return true;
}

QLineEdit* fAppConfig::createLineEdit(const QMetaProperty &p)
{
    QLineEdit* line = new QLineEdit(p.read(this).toString(), this);
    if (p.isResettable()) {
        QAction* act = new QAction(zvvlib::getIcon("blocks"), QString(), this);
        line->addAction(act, QLineEdit::TrailingPosition);
        connect(act, &QAction::triggered, [this, line, p](bool b){(void)b; line->setText(takeNewValue(p).toString());});
    }
    return line;
}

bool fAppConfig::updateLineEdit(QLineEdit *line, const QMetaProperty &p)
{
    line->setText(p.read(this).toString());
    return true;
}

/// \brief createSpinBox()
/// \param p
/// \return
QSpinBox *fAppConfig::createSpinBox(const QMetaProperty &p, const QString &suffix)
{
    QSpinBox *result = new QSpinBox(this);
    int initv = p.read(this).toInt();
    if (p.isResettable())
    { // find out: min, max & step...
        p.write(this, QVariant::fromValue<int>(INT_MAX));
        p.reset(this);
        result->setMaximum(p.read(this).toInt());

        p.write(this, QVariant::fromValue<int>(INT_MIN));
        p.reset(this);
        int min = p.read(this).toInt();
        result->setMinimum(min);

        p.write(this, QVariant::fromValue<int>(min + 1));
        p.reset(this);
        result->setSingleStep(p.read(this).toInt() - min);

        p.write(this, QVariant::fromValue<int>(initv)); //restore init value!!!
    }
    result->setSuffix(suffix);
    result->setValue(initv);
    return result;
}

bool fAppConfig::updateSpinBox(QSpinBox *sbox, const QMetaProperty &p)
{
    bool isok = false;
    int initv = p.read(this).toInt(&isok);
    if (isok) {
        sbox->setValue(initv);
    }
    return isok;
}

/// \brief fAppConfig::setSpinnerRangeAndStep() used in a reset of a property!!!
/// \param vptr
/// \param minv
/// \param maxv
/// \param step
void fAppConfig::setSpinnerRangeAndStep(int *vptr, const int minv, const int maxv, const int step)
{
    const int inValue = *vptr;
    if (inValue < minv)
    { //check low range..
        *vptr = minv;
    }
    else if (inValue > maxv)
    { //check high range..
        *vptr = maxv;
    }
    else if (step > 1)
    { //check step...
        int step_remainder = inValue % step;
        if (step_remainder)
        { //
            if (step_remainder > (step / 2)) {
                *vptr = (inValue / step) * step;
            }
            else {
                *vptr = ((inValue /step)+1)*step;
            }
        }
    }
}

/// \brief fAppConfig::setFileNameString
/// \param vptr
/// \param caption
/// \param filter like: tr("Config file (*.conf);; All (*.*)")
/// \param dir
///
void fAppConfig::setFileName(QString *vptr, const QString& caption, const QString& filter, const QString& dir)
{
    QString fileName = QFileDialog::getOpenFileName(this, caption, dir, filter);
    if (!fileName.isEmpty()) {
        vptr->swap(fileName);
    }
}

void fAppConfig::setDirectory(QString *vptr, const QString &caption, const QString &dir, QFileDialog::Options options)
{
    QString dirName = QFileDialog::getExistingDirectory(this, caption, dir, options);
    if (!dirName.isEmpty()) {
        QDir dr(qApp->applicationDirPath());
        QString rdirName = dr.relativeFilePath(dirName);
        if (rdirName.indexOf("..") >= 0) {
            vptr->swap(dirName);
        }
        else {
            vptr->swap(rdirName);
        }
    }
}


