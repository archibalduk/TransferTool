#ifndef YEAR_H
#define YEAR_H

#include <QString>

// --- Year --- //
class Year
{
private:
    short m_Year;

public:
    // Constructor
    Year();

    // Get data
    short get();
    QString getMatchText(int &yearAdjustment);

    // Set data
    void validate(const short year);
};

#endif // YEAR_H
