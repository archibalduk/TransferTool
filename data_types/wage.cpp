#include "wage.h"

// --- Default constructor --- //
Wage::Wage() :
    CM_Int(100)
{

}

// --- Set wage from world reputation --- //
void Wage::setFromWorldReputation(const short &rep)
{
    if(rep > 110) {
        m_Value = (rep - 100) * 1000;
    }
    else if(rep > 0) {
        m_Value = rep * 100;
    }

    // Ensure that any wages are at least 100
    if(rep > 0 && m_Value < 100) {
        m_Value = 100;
    }
}
