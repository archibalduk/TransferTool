#include "reputation.h"

// --- Static data --- //
QVector<unsigned int> *Reputation::s_Values = Reputation::initValues();

// --- Default constructor -- //
Reputation::Reputation() :
    CM_Short(1)
{

}

// --- Set from QVariant --- //
void Reputation::set(const QVariant &value)
{
    QString tmp = value.toString();

    if(tmp.isEmpty() || tmp.contains("-"))
        return;

    short i = static_cast<short>(value.toInt());

    if(i < 1)
        i = 1;
    else if(i > 200)
        i = 200;

    m_Value = i;
}

// --- Initialise transfer value <--> reputation table --- //
QVector<unsigned int> *Reputation::initValues()
{
    QVector<unsigned int> *v = new QVector<unsigned int>;
    v->reserve(201);    // 0 - 200

    v->append(0);       // Rep = 0
    v->append(0);       // Rep = 1
    v->append(35000);   // Rep = 2
    v->append(100000);
    v->append(150000);
    v->append(200000);
    v->append(300000);
    v->append(350000);
    v->append(400000);
    v->append(450000);
    v->append(475000);
    v->append(525000);
    v->append(575000);
    v->append(625000);
    v->append(700000);
    v->append(750000);
    v->append(775000);
    v->append(825000);
    v->append(875000);
    v->append(950000);
    v->append(1000000);
    v->append(1100000);
    v->append(1200000);
    v->append(1300000);
    v->append(1400000);
    v->append(1500000);
    v->append(1600000);
    v->append(1700000);
    v->append(1800000);
    v->append(1900000);
    v->append(2000000);
    v->append(2100000);
    v->append(2100000);
    v->append(2200000);
    v->append(2200000);
    v->append(2300000);
    v->append(2300000);
    v->append(2400000);
    v->append(2400000);
    v->append(2500000);
    v->append(2500000);
    v->append(2600000);
    v->append(2600000);
    v->append(2700000);
    v->append(2700000);
    v->append(2800000);
    v->append(2800000);
    v->append(2900000);
    v->append(2900000);
    v->append(3000000);
    v->append(3000000);
    v->append(3100000);
    v->append(3100000);
    v->append(3200000);
    v->append(3400000);
    v->append(3500000);
    v->append(3600000);
    v->append(3700000);
    v->append(3900000);
    v->append(4000000);
    v->append(4200000);
    v->append(4200000);
    v->append(4500000);
    v->append(4500000);
    v->append(4700000);
    v->append(4700000);
    v->append(4800000);
    v->append(5000000);
    v->append(5200000);
    v->append(5200000);
    v->append(5400000);
    v->append(5400000);
    v->append(5500000);
    v->append(5800000);
    v->append(5800000);
    v->append(6000000);
    v->append(6000000);
    v->append(6200000);
    v->append(6300000);
    v->append(6500000);
    v->append(6500000);
    v->append(6700000);
    v->append(6700000);
    v->append(6800000);
    v->append(7000000);
    v->append(7000000);
    v->append(7200000);
    v->append(7200000);
    v->append(7500000);
    v->append(7500000);
    v->append(7500000);
    v->append(8000000);
    v->append(8000000);
    v->append(8500000);
    v->append(8500000);
    v->append(9000000);
    v->append(9000000);
    v->append(9500000);
    v->append(9500000);
    v->append(10000000);
    v->append(10000000);
    v->append(10500000);
    v->append(11000000);
    v->append(12000000);
    v->append(12000000);
    v->append(12500000);
    v->append(13000000);
    v->append(13500000);
    v->append(14000000);
    v->append(14500000);
    v->append(15000000);
    v->append(15500000);
    v->append(16000000);
    v->append(17000000);
    v->append(17000000);
    v->append(17500000);
    v->append(18000000);
    v->append(18500000);
    v->append(19000000);
    v->append(20000000);
    v->append(20000000);
    v->append(21000000);
    v->append(22000000);
    v->append(23000000);
    v->append(24000000);
    v->append(25000000);
    v->append(25500000);
    v->append(27000000);
    v->append(28000000);
    v->append(29000000);
    v->append(30000000);
    v->append(31500000);
    v->append(32000000);
    v->append(33000000);
    v->append(35000000);
    v->append(35000000);
    v->append(36000000);
    v->append(37500000);
    v->append(38000000);
    v->append(40000000);
    v->append(40000000);
    v->append(40500000);
    v->append(42000000);
    v->append(44000000);
    v->append(44000000);
    v->append(45000000);
    v->append(48000000);
    v->append(48000000);
    v->append(48000000);
    v->append(49500000);
    v->append(50000000);
    v->append(52000000);
    v->append(52000000);
    v->append(54000000);
    v->append(54000000);
    v->append(55000000);
    v->append(56000000);
    v->append(60000000);
    v->append(60000000);
    v->append(60000000);
    v->append(60000000);
    v->append(64000000);
    v->append(64000000);
    v->append(64000000);
    v->append(64000000);
    v->append(65000000);
    v->append(70000000);
    v->append(70000000);
    v->append(70000000);
    v->append(70000000);
    v->append(70000000);
    v->append(72000000);
    v->append(72000000);
    v->append(75000000);
    v->append(75000000);
    v->append(75000000);
    v->append(80000000);
    v->append(80000000);
    v->append(80000000);
    v->append(80000000);
    v->append(80000000);
    v->append(81000000);
    v->append(85000000);
    v->append(85000000);
    v->append(90000000);
    v->append(90000000);
    v->append(100000000);
    v->append(100000000);
    v->append(100000000);
    v->append(100000000);
    v->append(100000000);
    v->append(110000000);
    v->append(110000000);
    v->append(110000000);
    v->append(110000000);
    v->append(117000000);
    v->append(120000000);
    v->append(128000000);
    v->append(180000000);
    v->append(180000000);
    v->append(180000000);   // Rep = 200

    return v;
}

// --- Set from transfer value --- //
bool Reputation::setFromTransferValue(const unsigned int &euros)
{
    // Temporary rep value
    short r = 200;  // Use 200 as this will be used if the loop below fails to find a match

    // Find nearest match
    const short count = static_cast<short>(s_Values->size());
    for(short i = 0; i < count; ++i) {

        if(euros < s_Values->at(i)) {
            r = i-1;
            break;
        }
    }

    // Use a rep of 1 if the temporary figure is less than 1
    if(r > 0) {
        m_Value = r;
        return true;
    }
    else {
        m_Value = 1;
        return false;
    }
}

// --- Set from transfer value (int convenience function) --- //
bool Reputation::setFromTransferValue(const int &euros)
{
    // Abort if a negative value was passed
    if(euros < 0)
        return false;

    const unsigned int i = static_cast<unsigned int>(euros);
    return this->setFromTransferValue(i);
}

// --- Set from transfer value (QVariant convenience function) --- //
bool Reputation::setFromTransferValue(const QVariant &euros)
{
    // Abort if a negative value was passed
    if(euros.toString().isEmpty() || euros.toInt() < 0)
        return false;

    const unsigned int i = euros.toUInt();
    return this->setFromTransferValue(i);
}
