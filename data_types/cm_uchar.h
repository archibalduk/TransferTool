#ifndef CM_UCHAR_H
#define CM_UCHAR_H

#include <QString>
#include <QVariant>

#pragma warning(disable: 4103)
#pragma pack(1)

// --- Unsigned char wrapper --- //
class CM_UChar
{
protected:
    unsigned char m_Value;

public:
    CM_UChar();

    // Get data
    unsigned short get() const;
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value);
};


#endif // CM_UCHAR_H
