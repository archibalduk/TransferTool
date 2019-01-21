#include "name.h"

// --- Static data --- //
QVector<CommonName> CommonName::db;
QVector<FirstName> FirstName::db;
QVector<SecondName> SecondName::db;


/* =================== */
/*      Name Data      */
/* =================== */

// --- Default constructor (base class) --- //
Name::Name() :
    Count(0)
{
    StringText.reserve(STANDARD_TEXT_LENGTH);
}

// --- Default constructor (Common name) --- //
CommonName::CommonName() :
    Name()
{
    ID = CommonName::db.size(); // Allocate next ID to item
}

// --- Default constructor (First name) --- //
FirstName::FirstName() :
    Name()
{
    ID = FirstName::db.size(); // Allocate next ID to item
}

// --- Default constructor (Second name) --- //
SecondName::SecondName() :
    Name()
{
    ID = SecondName::db.size(); // Allocate next ID to item
}


/* ================== */
/*      Counters      */
/* ================== */

// --- Decrement count --- //
void Name::decrementCount()
{
    --Count;
}

// --- Increment count --- //
void Name::incrementCount()
{
    if(Count < 127)
        ++Count;
}

// --- Reset count --- //
void Name::resetCount()
{
    Count = 0;
}

// --- Reset all name counters --- //
void CommonName::resetCounters()
{
    for(int i = 0; i < db.size(); ++i)
        db[i].resetCount();
}

// --- Reset all name counters --- //
void FirstName::resetCounters()
{
    for(int i = 0; i < db.size(); ++i)
        db[i].resetCount();
}

// --- Reset all name counters --- //
void SecondName::resetCounters()
{
    for(int i = 0; i < db.size(); ++i)
        db[i].resetCount();
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get name text for models --- //
QString Name::getName()
{
    return StringText;
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Set name (and add if not present) --- //
int CommonName::set(const QString &name, const int &nation)
{
    // Skip any blank names
    if(name.isEmpty())
    {
        return -1;
    }

    // Check for pre-existing name
    const int sz = CommonName::db.size();
    for(int i = 0; i < sz; ++i)
    {
        if(CommonName::db[i].StringText == name && CommonName::db[i].ID == nation)
            return i;
    }

    // Add as a new name
    CommonName n;
    n.Nation.set(nation);

    String str(name);
    str.toSIFont(n.NameText, STANDARD_TEXT_LENGTH);
    n.StringText = str.get();

    CommonName::db.push_back(n);
    return n.ID;
}

// --- Set name (and add if not present) --- //
int FirstName::set(const QString &name, const int &nation)
{
    // Skip any blank names
    if(name.isEmpty())
    {
        return -1;
    }

    // Check for pre-existing name
    const int sz = FirstName::db.size();
    for(int i = 0; i < sz; ++i)
    {
        if(FirstName::db[i].StringText == name && FirstName::db[i].Nation == nation)
            return i;
    }

    // Add as a new name
    FirstName n;
    n.Nation.set(nation);

    String str(name);
    str.toSIFont(n.NameText, STANDARD_TEXT_LENGTH);
    n.StringText = str.get();

    FirstName::db.push_back(n);
    return n.ID;
}

// --- Set name (and add if not present) --- //
int SecondName::set(const QString &name, const int &nation)
{
    // Skip any blank names
    if(name.isEmpty())
    {
        return -1;
    }

    // Check for pre-existing name
    const int sz = SecondName::db.size();
    for(int i = 0; i < sz; ++i)
    {
        if(SecondName::db[i].StringText == name && SecondName::db[i].Nation == nation)
            return i;
    }

    // Add as a new name
    SecondName n;
    n.Nation.set(nation);

    String str(name);
    str.toSIFont(n.NameText, STANDARD_TEXT_LENGTH);
    n.StringText = str.get();

    SecondName::db.push_back(n);
    return n.ID;
}

// --- Set string text for each DB item --- //
void CommonName::setStringText()
{
    const int sz = db.size();
    for(int i = 0; i < sz; ++i) {
        db[i].StringText = String(db[i].NameText).get();
    }
}

// --- Set string text for each DB item --- //
void FirstName::setStringText()
{
    const int sz = db.size();
    for(int i = 0; i < sz; ++i) {
        db[i].StringText = String(db[i].NameText).get();
    }
}

// --- Set string text for each DB item --- //
void SecondName::setStringText()
{
    const int sz = db.size();
    for(int i = 0; i < sz; ++i) {
        db[i].StringText = String(db[i].NameText).get();
    }
}
