#include "index.h"
#include "data_types/string.h"

// --- Static data --- //
QVector<Index> Index::db;
char Index::s_Header[INDEX_HEADER_SIZE];


/* ==================== */
/*      Index Data      */
/* ==================== */

// --- Default constructor --- //
Index::Index()
{

}


/* ================= */
/*      Get Data     */
/* ================= */

// --- File Name --- //
QString Index::getFileName()
{
    return String(Filename).get();
}

// --- Offset --- //
int Index::getOffset()
{
    return Offset;
}

// --- Record count --- //
int Index::getRecordCount()
{
    return TableSize;
}


/* ================= */
/*      Set Data     */
/* ================= */

// --- Count/size and offset --- //
void Index::set(const int size, const int offset)
{
    TableSize = size;
    Offset = offset;
}
