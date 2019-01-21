#include "data_pointer.h"

// --- Static data --- //
QString DataPointer::s_IdentifierNoneText = "_none";
QString DataPointer::s_IdentifierOutOfRangeText = "_outofrange";
QString DataPointer::s_NoneText = "[None]";
QString DataPointer::s_OutOfRangeText = "[Error: Out of range]";

// --- Default Constructor --- //
DataPointer::DataPointer() :
    id(-1)
{

}

// --- Constructor --- //
DataPointer::DataPointer(const int i) :
    id(i)
{

}

// --- Get id --- //
int DataPointer::get() const
{
    return id;
}

// --- Get text (generic virtual function) --- //
QString DataPointer::getText() const
{
    return "[Invalid Sub-Class]";
}

// --- Return whether id is none --- //
bool DataPointer::isNone() const
{
    return(id < 0);
}

// --- Return whether id is pointing to an int (i.e. not none) --- //
bool DataPointer::isPointer() const
{
    return(id >= 0);
}

// --- Clear the pointer --- //
void DataPointer::clear()
{
    id = -1;
}

// --- Set id --- //
void DataPointer::set(const int &i)
{
    id = i;
}

// --- Set id --- //
void DataPointer::set(const QVariant &value)
{
    id = value.toInt();
}

// --- Set id without any data validation checks --- //
void DataPointer::setNoCheck(const QVariant &value)
{
    const QString tmp = value.toString();
    const int i = value.toInt();

    if(tmp.isEmpty() || tmp.contains("-"))
        return;
    else if(i < -2)
        id = -1;
    else
        id = i;
}

// --- Comparison operator overload --- //
bool DataPointer::operator ==(const int &b) const
{
    return id == b;
}
