#include "staff_classification.h"

/* ============================== */
/*      Staff Classification      */
/* ============================== */

// --- Default constructor --- //
StaffClassification::StaffClassification()
{

}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Is non-player --- //
bool StaffClassification::isNonPlayer()
{
    if(m_Value == NON_PLAYER || m_Value == NON_PLAYER_AND_PLAYER)
        return true;
    else
        return false;
}

// --- Is player --- //
bool StaffClassification::isPlayer()
{
    if(     m_Value == PLAYER ||
            m_Value == NON_PLAYER_AND_PLAYER ||
            m_Value == YOUTH_PLAYER)
        return true;
    else
        return false;
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Set classification --- //
void StaffClassification::set(int i)
{
    if(i < INVALID_CLASSIFICATION || i > SPARE_NON_PLAYER)
        return;
    else
        m_Value = static_cast<char>(i);
}
