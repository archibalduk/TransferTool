#ifndef CM_CHAR_H
#define CM_CHAR_H

#include <QString>
#include <QVariant>

#pragma warning(disable: 4103)
#pragma pack(1)

// --- Char wrapper --- //
class CM_Char
{
protected:
    char m_Value;

public:
    CM_Char();

    // Get data
    short get() const;
    QString getText() const;

    // Set data
    void set(int i);
};

#endif // CM_CHAR_H
