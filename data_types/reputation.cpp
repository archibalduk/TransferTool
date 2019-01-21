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
    v->append(25000);   // Rep = 1
    v->append(50000);   // Rep = 2
    v->append(75000);
    v->append(100000);
    v->append(125000);
    v->append(150000);
    v->append(175000);
    v->append(200000);
    v->append(225000);
    v->append(250000);
    v->append(275000);
    v->append(300000);
    v->append(325000);
    v->append(350000);
    v->append(375000);
    v->append(400000);
    v->append(425000);
    v->append(450000);
    v->append(475000);
    v->append(500000);
    v->append(550000);
    v->append(600000);
    v->append(650000);
    v->append(700000);
    v->append(750000);
    v->append(800000);
    v->append(850000);
    v->append(900000);
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
    v->append(2200000);
    v->append(2300000);
    v->append(2400000);
    v->append(2500000);
    v->append(2600000);
    v->append(2700000);
    v->append(2800000);
    v->append(2900000);
    v->append(3000000);
    v->append(3100000);
    v->append(3200000);
    v->append(3300000);
    v->append(3400000);
    v->append(3500000);
    v->append(3600000);
    v->append(3700000);
    v->append(3800000);
    v->append(3900000);
    v->append(4000000);
    v->append(4100000);
    v->append(4200000);
    v->append(4300000);
    v->append(4400000);
    v->append(4500000);
    v->append(4600000);
    v->append(4700000);
    v->append(4800000);
    v->append(4900000);
    v->append(5000000);
    v->append(5100000);
    v->append(5200000);
    v->append(5300000);
    v->append(5400000);
    v->append(5500000);
    v->append(5600000);
    v->append(5700000);
    v->append(5800000);
    v->append(5900000);
    v->append(6000000);
    v->append(6100000);
    v->append(6200000);
    v->append(6300000);
    v->append(6400000);
    v->append(6500000);
    v->append(6600000);
    v->append(6700000);
    v->append(6800000);
    v->append(6900000);
    v->append(7000000);
    v->append(7100000);
    v->append(7200000);
    v->append(7300000);
    v->append(7400000);
    v->append(7500000);
    v->append(7600000);
    v->append(7700000);
    v->append(7800000);
    v->append(7900000);
    v->append(8000000);
    v->append(8200000);
    v->append(8400000);
    v->append(8600000);
    v->append(8800000);
    v->append(9000000);
    v->append(9200000);
    v->append(9400000);
    v->append(9600000);
    v->append(9800000);
    v->append(10000000);
    v->append(10500000);
    v->append(11000000);
    v->append(11500000);
    v->append(12000000);
    v->append(12500000);
    v->append(13000000);
    v->append(13500000);
    v->append(14000000);
    v->append(14500000);
    v->append(15000000);
    v->append(15500000);
    v->append(16000000);
    v->append(16500000);
    v->append(17000000);
    v->append(17500000);
    v->append(18000000);
    v->append(18500000);
    v->append(19000000);
    v->append(19500000);
    v->append(20000000);
    v->append(20500000);
    v->append(21000000);
    v->append(21500000);
    v->append(22000000);
    v->append(22500000);
    v->append(23000000);
    v->append(23500000);
    v->append(24000000);
    v->append(24500000);
    v->append(25000000);
    v->append(25500000);
    v->append(26000000);
    v->append(26500000);
    v->append(27000000);
    v->append(27500000);
    v->append(28000000);
    v->append(28500000);
    v->append(29000000);
    v->append(29500000);
    v->append(30000000);
    v->append(30500000);
    v->append(31000000);
    v->append(31500000);
    v->append(32000000);
    v->append(32500000);
    v->append(33000000);
    v->append(33500000);
    v->append(34000000);
    v->append(34500000);
    v->append(35000000);
    v->append(35500000);
    v->append(36000000);
    v->append(36500000);
    v->append(37000000);
    v->append(37500000);
    v->append(38000000);
    v->append(38500000);
    v->append(39000000);
    v->append(39500000);
    v->append(40000000);
    v->append(42000000);
    v->append(44000000);
    v->append(46000000);
    v->append(48000000);
    v->append(50000000);
    v->append(52000000);
    v->append(54000000);
    v->append(56000000);
    v->append(58000000);
    v->append(60000000);
    v->append(62000000);
    v->append(64000000);
    v->append(66000000);
    v->append(68000000);
    v->append(70000000);
    v->append(72000000);
    v->append(74000000);
    v->append(76000000);
    v->append(78000000);
    v->append(80000000);
    v->append(82000000);
    v->append(84000000);
    v->append(86000000);
    v->append(88000000);
    v->append(90000000);
    v->append(92000000);
    v->append(94000000);
    v->append(96000000);
    v->append(98000000);
    v->append(100000000);   // Rep = 200

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
