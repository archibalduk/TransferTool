#include "job.h"

Job::Job()
{

}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Set job --- //
void Job::set(int i)
{
    if(i < INVALID_JOB || i > MEDIA_PUNDIT)
        return;
    else
        m_Value = static_cast<char>(i);
}
