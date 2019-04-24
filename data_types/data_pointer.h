#ifndef DATA_POINTER_H
#define DATA_POINTER_H

#include <QString>
#include <QVariant>

#pragma warning(disable: 4103)
#pragma pack(1)

// --- Integer id/pointer base class --- //
class DataPointer
{
protected:
    //int id;

    // Default text
    static QString s_IdentifierNoneText;
    static QString s_IdentifierOutOfRangeText;
    static QString s_NoneText;
    static QString s_OutOfRangeText;

public:
    DataPointer();
    DataPointer(const int i);

    // Get data
    int get() const;
    QString getText() const;
    bool isNone() const;
    bool isPointer() const;
    int id;

    // Set data
    void clear();
    void set(const int &i);
    void set(const QVariant &value);
    void setNoCheck(const QVariant &value);

    // Operator overloading
    bool operator ==(const int &b) const;
};

#endif // DATA_POINTER_H
