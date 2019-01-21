#include "vlookup_hash.h"

// Qt headers
#include <QFile>


/* ================================= */
/*      Additional VLookup Hash      */
/* ================================= */

// --- Constructor --- //
VLookupHash::VLookupHash(QString filename, char type) :
    m_FileName(filename),
    m_Type(type),
    m_MatchedCount(0),
    m_UnmatchedCount(0)
{

}

// --- Constructor (using type) --- //
VLookupHash::VLookupHash(char type) :
    m_MatchedCount(0),
    m_UnmatchedCount(0)
{
    this->set(type);
}

/* ====================== */
/*      Default Data      */
/* ====================== */

// --- Use default data if available --- //
void VLookupHash::initDefaultData(Spreadsheet &s)
{
    // Use default data or return if none available for the type
    switch(m_Type) {

    case CLUBS:
        s.addHeader("Find:", "Replace with:");
        this->initDefaultClubData(s);
        break;

    case NATIONS:        
        s.addHeader("Find:", "Replace with:");
        this->initDefaultNationData(s);
        break;

    case CLUBS_PREFIX_SUFFIX:
        s.addHeader("Prefix/Suffix name:");
        this->initDefaultClubPrefixSuffixData(s);
        break;

    default:
        return;
    }

    // Save default data
    s.write(m_FileName);
}

// --- Default data: Clubs --- //
void VLookupHash::initDefaultClubData(Spreadsheet &/*s*/)
{

}

// --- Default data: Club prefixes/suffixes --- //
void VLookupHash::initDefaultClubPrefixSuffixData(Spreadsheet &s)
{
    s.add("afc");
    s.add("fc");
}

// --- Default data: Nations --- //
void VLookupHash::initDefaultNationData(Spreadsheet &s)
{
    s.add("American Virgin Islands", "US Virgin Islands");
    s.add("Antigua and Barbuda", "Antigua & Barbuda");
    s.add("Bonaire", "_none");
    s.add("Botsuana", "Botswana");
    s.add("Cape Verde", "Cape Verde Islands");
    s.add("Cayman-Inseln", "Cayman Islands");
    s.add("Chinese Taipei (Taiwan)", "Chinese Taipei");
    s.add("Congo DR", "Democratic Republic of Congo");
    s.add("Congo", "The Congo");
    s.add("Cookinseln", "Cook Islands");
    s.add("Cote d'Ivoire", "Ivory Coast");
    s.add("Falkland Islands", "_none");
    s.add("Faroe Island", "Faroe Islands");
    s.add("Federated States of Micronesia", "_none");
    s.add("French Guiana", "_none");
    s.add("Greenland", "_none");
    s.add("Guadeloupe", "_none");
    s.add("Guine", "Guinea");
    s.add("Hongkong", "Hong Kong");
    s.add("Ireland", "Republic of Ireland");
    s.add("Kiribati", "_none");
    s.add("Korea North", "North Korea");
    s.add("Korea South", "South Korea");
    s.add("Macao", "Macau");
    s.add("Macedonia", "FYR of Macedonia");
    s.add("Mariana Islands", "_none");
    s.add("Marshall Islands", "_none");
    s.add("Martinique", "_none");
    s.add("Monaco", "_none");
    s.add("Nauru", "_none");
    s.add("Netherlands", "Holland");
    s.add("Neukaledonien", "New Caledonia");
    s.add("Niue", "_none");
    s.add("Osttimor", "Timor");
    s.add("Palästina", "Palestine");
    s.add("Palau", "_none");
    s.add("Philippines", "The Philippines");
    s.add("Réunion", "_none");
    s.add("Saint-Martin", "_none");
    s.add("Sao Tome and Principe", "São Tomé & Principe");
    s.add("Sint Maarten", "_none");
    s.add("Southern Sudan", "South Sudan");
    s.add("St. Kitts &Nevis", "St Kitts & Nevis");
    s.add("St. Lucia", "Saint Lucia");
    s.add("St. Vincent & Grenadinen", "St Vincent & The Grenadines");
    s.add("Suriname", "Surinam");
    s.add("Tibet", "_none");
    s.add("Trinidad and Tobago", "Trinidad & Tobago");
    s.add("Turks- and Caicosinseln", "Turks and Caicos Islands");
    s.add("Tuvalu", "_none");
    s.add("Vatican", "_none");
    s.add("Zanzibar", "_none");
}


/* ================== */
/*      File I/O      */
/* ================== */

// --- Load a spreadsheet containing vlookup data and add to a hash --- //
void VLookupHash::load(QHash<QString, int> &hash)
{
    Spreadsheet s;
    s.setSilent(true);

    // Initialise default data if file could not be opened
    if(!s.read(m_FileName)) {
        this->initDefaultData(s);
    }

    // Reset counters
    m_MatchedCount = 0;
    m_UnmatchedCount = 0;

    // Add the data
    const int size = s.data()->size();
    for(int i = 0; i < size; ++i) {
        int id = hash.value(s.matchString(i, REPLACE), NO_MATCH);

        if(id != NO_MATCH) {
            hash.insert(s.matchString(i, FIND), id);
            ++m_MatchedCount;
        }
        else {
            ++m_UnmatchedCount;
        }
    }
}

// --- Load a spreadsheet containing vlookup prefix/suffix data and add to a vector --- //
void VLookupHash::load(QVector<QString> &vector)
{
    Spreadsheet s;
    s.setSilent(true);

    // Initialise default data if file could not be opened
    if(!s.read(m_FileName)) {
        this->initDefaultData(s);
    }

    // Add the data
    const int size = s.data()->size();
    for(int i = 0; i < size; ++i) {
        vector.push_back(s.matchString(i, PREFIX_SUFFIX));
    }

    // Set counters
    m_MatchedCount = size;
    m_UnmatchedCount = 0;
}


/* ================ */
/*      Lookup      */
/* ================ */

// --- Look up a hash using prefix/suffix strings --- //
int VLookupHash::lookupPrefixSuffix(const QString &targetString,
                                    QVector<QString> &vectorPrefixSuffix, QHash<QString, int> &hash,
                                    const int noMatchValue)
{
    // Buffers
    int result = noMatchValue;
    QString target;

    // Iterate over vector
    const int size = vectorPrefixSuffix.size();
    for(int i = 0; i < size; ++i) {

        QString prefix = QString("%1 ").arg(vectorPrefixSuffix.at(i));
        QString suffix = QString(" %1").arg(vectorPrefixSuffix.at(i));

        // Add suffix
        target = QString("%1%2").arg(targetString).arg(suffix);
        result = hash.value(target, noMatchValue);

        if(result != noMatchValue)
            break;

        // Add prefix
        target = QString("%1%2").arg(prefix).arg(targetString);
        result = hash.value(target, noMatchValue);

        if(result != noMatchValue)
            break;

        // String size
        const int stringSize = targetString.size();
        const int prefixSuffixSize = prefix.size(); // Both prefix and suffix are the same string size

        // Remove suffix
        if(targetString.endsWith(suffix)) {
            target = targetString.left(stringSize - prefixSuffixSize);
            result = hash.value(target, noMatchValue);

            if(result != noMatchValue)
                break;
        }

        // Remove prefix
        else if(targetString.startsWith(prefix)) {
            target = targetString.right(stringSize - prefixSuffixSize);
            result = hash.value(target, noMatchValue);

            if(result != noMatchValue)
                break;
        }
    }

    return result;
}



/* ================== */
/*      Set Data      */
/* ================== */

// --- Set data --- //
void VLookupHash::set(QString filename, char type)
{
    m_FileName = filename;
    m_Type = type;
}

// --- Set data (based upon type) --- //
void VLookupHash::set(char type)
{
    m_Type = type;

    switch(m_Type) {
    case CLUBS:
        m_FileName = "vlookup_clubs.csv";
        break;

    case CLUB_COMPS:
        m_FileName = "vlookup_club_comps.csv";
        break;

    case NATIONS:
        m_FileName = "vlookup_nations.csv";
        break;

    case CLUBS_PREFIX_SUFFIX:
        m_FileName = "vlookup_clubs_prefix_suffix.csv";
        break;

    default:
        return;
    }
}
