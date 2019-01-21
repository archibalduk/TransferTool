#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QVariantList>

// --- Application settings --- //
class Settings
{
public:
    // Constructor/Destructor
    Settings();
    ~Settings();

    // Get data
    int count();

    // Get setting
    QVariant get(unsigned int pos);
    QString text(unsigned int pos);

    // Set setting
    void set(const bool &value, unsigned int pos);
    void set(const double &value, unsigned int pos);
    void set(const int &value, unsigned int pos);
    void set(const QString &text, unsigned int pos);

private:
    // Settings
    QVariantList m_Settings;

    // Settings file
    static QString s_FileName;

    // Version
    static unsigned int s_Version;
};

#endif // SETTINGS_H
