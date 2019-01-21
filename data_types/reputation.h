#ifndef REPUTATION_H
#define REPUTATION_H

#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)

#include "cm_short.h"

// --- Reputation (short) --- //
class Reputation : public CM_Short
{
private:
    // Transfer value <--> reputation table
    static QVector<unsigned int> *s_Values;
    static QVector<unsigned int> *initValues();

public:
    // Constructor
    Reputation();

    // Set data
    void set(const QVariant &value);
    bool setFromTransferValue(const int &euros);
    bool setFromTransferValue(const unsigned int &euros);
    bool setFromTransferValue(const QVariant &euros);
};

#endif // REPUTATION_H
