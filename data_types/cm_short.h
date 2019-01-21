#ifndef CM_SHORT_H
#define CM_SHORT_H

#include <QVariant>

#pragma warning(disable: 4103)
#pragma pack(1)

// --- Short wrapper --- //
class CM_Short
{
protected:
    short m_Value;

public:
    CM_Short(const short &i = 0);

    // Get data
    short get() const;
    QString getText() const;

    // Set data
    void set(int &i);
    void set(short &i);
};

#endif // CM_SHORT_H
