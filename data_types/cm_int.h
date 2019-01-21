#ifndef CM_INT_H
#define CM_INT_H

#include <QString>
#include <QVariant>

#pragma warning(disable: 4103)
#pragma pack(1)

// --- Integer wrapper --- //
class CM_Int
{
protected:
    int m_Value;

public:
    CM_Int(const int &i = 0);

    // Get data
    int get() const;
    QString getText() const;

    // Set data
    void set(const int &i);
    void set(const QVariant &value, double exchangeRate = 1.0);
};

#endif // CM_INT_H
