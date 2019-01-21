#include "squad.h"

Squad::Squad() :
    CM_Char()
{
    m_Value = INVALID_SQUAD;
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Set job --- //
void Squad::set(int i)
{
    if(i < INVALID_SQUAD || i > NATION_B_SQUAD)
        return;
    else
        m_Value = static_cast<char>(i);
}
