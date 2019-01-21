#include "settings.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>

// --- Static data --- //
QString Settings::s_FileName("settings.dat"); // Settings file name
unsigned int Settings::s_Version = 1002; // Version control. Increase this value each time the file structure changes significantly


/* ================== */
/*      Settings      */
/* ================== */

// --- Default constructor (load settings) --- //
Settings::Settings()
{
    QFile f(s_FileName);
    QDataStream in(&f);

    if(!f.open(QIODevice::ReadOnly))
        return;

    // Read version number
    unsigned int version;
    in >> version;

    // Only read the settings if the version is compatible
    if(version == s_Version)
        in >> m_Settings;

    f.close();
}

// --- Default destructor (save settings) --- //
Settings::~Settings()
{
    QFile f(s_FileName);
    QDataStream out(&f);

    if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    out << s_Version;
    out << m_Settings;

    f.close();
}


/* ===================== */
/*      Get Data      */
/* ===================== */

// --- Get size of QVariantList --- //
int Settings::count()
{
    return m_Settings.count();
}


/* ===================== */
/*      Get Setting      */
/* ===================== */

// --- Get setting (variant) --- //
QVariant Settings::get(unsigned int pos)
{
    if(pos < static_cast<unsigned int>(m_Settings.size()))
        return m_Settings[pos];
    else
        return QVariant();
}

// --- Get setting (string) --- //
QString Settings::text(unsigned int pos)
{
    if(pos < static_cast<unsigned int>(m_Settings.size()))
        return m_Settings[pos].toString();
    else
        return QString();
}


/* ===================== */
/*      Set Setting      */
/* ===================== */

// --- Set setting from bool --- //
void Settings::set(const bool &value, unsigned int pos)
{
    // Fill the QVariantList if the pos is beyond the current size of the list
    while(static_cast<unsigned int>(m_Settings.size()) <= pos )
        m_Settings << QVariant();

    m_Settings[pos].setValue(value);
}

// --- Set setting from double --- //
void Settings::set(const double &value, unsigned int pos)
{
    // Fill the QVariantList if the pos is beyond the current size of the list
    while(static_cast<unsigned int>(m_Settings.size()) <= pos )
        m_Settings << QVariant();

    m_Settings[pos].setValue(value);
}

// --- Set setting from int --- //
void Settings::set(const int &value, unsigned int pos)
{
    // Fill the QVariantList if the pos is beyond the current size of the list
    while(static_cast<unsigned int>(m_Settings.size()) <= pos )
        m_Settings << QVariant();

    m_Settings[pos].setValue(value);
}

// --- Set setting from string --- //
void Settings::set(const QString &text, unsigned int pos)
{
    // Fill the QVariantList if the pos is beyond the current size of the list
    while(static_cast<unsigned int>(m_Settings.size()) <= pos )
        m_Settings << QVariant();

    m_Settings[pos].setValue(text);
}
