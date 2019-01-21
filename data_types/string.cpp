#include "string.h"

// Qt headers
#include <QRegExp>

// --- Static data --- //
QVector<wchar_t> String::s_AccentedCharacters = String::getAccentedCharacterVector();
QVector<wchar_t> String::s_UnaccentedCharacters = String::getUnaccentedCharacterVector();
QVector<wchar_t> String::s_EncodedCharacters = String::getEncodedCharacterVector();
QVector<wchar_t> String::s_UtfCharacters = String::getUtfCharacterVector();
bool String::s_IgnoreAccents = true;


/* =============== */
/*      String     */
/* =============== */

// --- Default constructor --- //
String::String() :
    m_Text()
{
}

// --- Constructor (QString) --- //
String::String(const QString text) :
    m_Text(text)
{

}

// --- Constructor (char array / SI Font / CM database text) --- //
String::String(const char text[])
{
    m_Text = QString::fromLatin1(text);

    const int size = s_EncodedCharacters.size();
    for(int i = 0; i < size; ++i)
        m_Text.replace(s_EncodedCharacters[i], s_UtfCharacters[i]);
}


/* ================= */
/*      Get data     */
/* ================= */

// --- Get string --- //
QString String::get()
{
    return m_Text;
}

// --- Get date string formatted for matching against --- //
QString String::getMatchDateString()
{
    QString text = m_Text.toLower();

    // Replace slashes with dots
    text.replace("/", ".");
    text.replace("\\", ".");

    // Do not process if the size of the date is too small (x.x.xxxx = 8 characters minimum)
    if(text.size() < 8)
        return text;

    // Add preceding zero if the day number is a single digit
    // e.g. 3.04.1990 -> 03.04.1990
    if(text.at(1) == QStringLiteral("."))
        text.prepend(QString::number(0));

    // Add preceding zero if the month number is a single digit
    // e.g. 03.4.1990 -> 03.04.1990
    if(text.at(4) == QStringLiteral("."))
        text.insert(3, QString::number(0));

    return text;
}

// --- Get string formatted for matching against --- //
QString String::getMatchString()
{
    // Lower case
    QString text = m_Text.toLower();

    // Remove accents
    if(s_IgnoreAccents) {
        const int size = s_AccentedCharacters.size();
        for(int i = 0; i < size; ++i)
            text.replace(s_AccentedCharacters[i], s_UnaccentedCharacters[i]);
    }

    // Replace commas and semi-colons with underscores (for CSV compatibility)
    text.replace(",", "_");
    text.replace(";", "_");

    return text;
}

// --- Get string formatted with no accents --- //
QString String::getNoAccentsString()
{
    // Lower case
    QString text = m_Text;

    // Remove accents
    if(s_IgnoreAccents) {
        const int size = s_AccentedCharacters.size();
        for(int i = 0; i < size; ++i)
            text.replace(s_AccentedCharacters[i], s_UnaccentedCharacters[i]);
    }

    // Replace commas and semi-colons with underscores (for CSV compatibility)
    text.replace(",", "_");
    text.replace(";", "_");

    return text;
}

// --- Convert the string to a char array / SI Font / CM database text --- //
void String::toSIFont(char *SITextArray, int length)
{
    QString tmp(m_Text);

    if(tmp.size() >= length)
        tmp.chop(length - 1);

    const int size = s_UtfCharacters.size();
    for(int i = 0; i < size; ++i)
        m_Text.replace(s_UtfCharacters[i], s_EncodedCharacters[i]);

    // Fill the SI Font char array with zeros and then copy the processed string
    memset(SITextArray,'\x00', length);
    strncpy_s(SITextArray, length, tmp.toLatin1().constData(), tmp.size());
}


/* ============================== */
/*      String Conversion Data    */
/* ============================== */

// --- Get accented text vector --- //
QVector<wchar_t> String::getAccentedCharacterVector()
{
    QVector<wchar_t> v;

    v.push_back(0x00DF); // ß - Lowercase
    v.push_back(0x00E0); // à - Lowercase
    v.push_back(0x00E1); // á - Lowercase
    v.push_back(0x00E2); // â - Lowercase
    v.push_back(0x00E3); // ã - Lowercase
    v.push_back(0x00E4); // ä - Lowercase
    v.push_back(0x00E5); // å - Lowercase
    v.push_back(0x00E6); // æ - Lowercase
    v.push_back(0x00E7); // ç - Lowercase
    v.push_back(0x00E8); // è - Lowercase
    v.push_back(0x00E9); // é - Lowercase
    v.push_back(0x00EA); // ê - Lowercase
    v.push_back(0x00EB); // ë - Lowercase
    v.push_back(0x00EC); // ì - Lowercase
    v.push_back(0x00ED); // í - Lowercase
    v.push_back(0x00EE); // î - Lowercase
    v.push_back(0x00EF); // ï - Lowercase
    v.push_back(0x00F0); // ð - Lowercase
    v.push_back(0x00F1); // ñ - Lowercase
    v.push_back(0x00F2); // ò - Lowercase
    v.push_back(0x00F3); // ó - Lowercase
    v.push_back(0x00F4); // ô - Lowercase
    v.push_back(0x00F5); // õ - Lowercase
    v.push_back(0x00F6); // ö - Lowercase
    v.push_back(0x00F8); // ø - Lowercase
    v.push_back(0x00F9); // ù - Lowercase
    v.push_back(0x00FA); // ú - Lowercase
    v.push_back(0x00FB); // û - Lowercase
    v.push_back(0x00FC); // ü - Lowercase
    v.push_back(0x00FD); // ý - Lowercase
    v.push_back(0x00FE); // þ - Lowercase
    v.push_back(0x00FF); // ÿ - Lowercase
    v.push_back(0x010D); // c - Caron - Lowercase
    v.push_back(0x010F); // d - Caron - Lowercase
    v.push_back(0x011B); // e - Caron - Lowercase
    v.push_back(0x0148); // n - Caron - Lowercase
    v.push_back(0x0159); // r - Caron - Lowercase
    v.push_back(0x0161); // s - Caron - Lowercase
    v.push_back(0x016F); // u - Ring Above - Lowercase
    v.push_back(0x017E); // z - Caron - Lowercase

    return v;
}

// --- Get unaccented text vector --- //
QVector<wchar_t> String::getUnaccentedCharacterVector()
{
    QVector<wchar_t> v;

    v.push_back(0x0073); // ß - Lowercase
    v.push_back(0x0061); // à - Lowercase
    v.push_back(0x0061); // á - Lowercase
    v.push_back(0x0061); // â - Lowercase
    v.push_back(0x0061); // ã - Lowercase
    v.push_back(0x0061); // ä - Lowercase
    v.push_back(0x0061); // å - Lowercase
    v.push_back(0x0061); // æ - Lowercase
    v.push_back(0x0063); // ç - Lowercase
    v.push_back(0x0065); // è - Lowercase
    v.push_back(0x0065); // é - Lowercase
    v.push_back(0x0065); // ê - Lowercase
    v.push_back(0x0065); // ë - Lowercase
    v.push_back(0x0069); // ì - Lowercase
    v.push_back(0x0069); // í - Lowercase
    v.push_back(0x0069); // î - Lowercase
    v.push_back(0x0069); // ï - Lowercase
    v.push_back(0x006F); // ð - Lowercase
    v.push_back(0x006E); // ñ - Lowercase
    v.push_back(0x006F); // ò - Lowercase
    v.push_back(0x006F); // ó - Lowercase
    v.push_back(0x006F); // ô - Lowercase
    v.push_back(0x006F); // õ - Lowercase
    v.push_back(0x006F); // ö - Lowercase
    v.push_back(0x006F); // ø - Lowercase
    v.push_back(0x0075); // ù - Lowercase
    v.push_back(0x0075); // ú - Lowercase
    v.push_back(0x0075); // û - Lowercase
    v.push_back(0x0075); // ü - Lowercase
    v.push_back(0x0079); // ý - Lowercase
    v.push_back(0x0079); // þ - Lowercase
    v.push_back(0x0079); // ÿ - Lowercase
    v.push_back(0x0063); // c - Caron - Lowercase
    v.push_back(0x0064); // d - Caron - Lowercase
    v.push_back(0x0065); // e - Caron - Lowercase
    v.push_back(0x006E); // n - Caron - Lowercase
    v.push_back(0x0072); // r - Caron - Lowercase
    v.push_back(0x0073); // s - Caron - Lowercase
    v.push_back(0x0075); // u - Ring Above - Lowercase
    v.push_back(0x007A); // z - Caron - Lowercase

    return v;
}

// --- Get encoded text vector --- //
QVector<wchar_t> String::getEncodedCharacterVector()
{
    QVector<wchar_t> v;

    v.push_back(0x00BC); // C - Caron - Uppercase
    v.push_back(0x00A0); // c - Caron - Lowercase
    v.push_back(0x00BE); // D - Caron - Uppercase
    v.push_back(0x00BB); // d - Caron - Lowercase
    v.push_back(0x0086); // E - Caron - Uppercase
    v.push_back(0x0090); // e - Caron - Lowercase
    v.push_back(0x00B1); // N - Caron - Uppercase
    v.push_back(0x008F); // n - Caron - Lowercase
    v.push_back(0x00B3); // R - Caron - Uppercase
    v.push_back(0x00A7); // r - Caron - Lowercase
    v.push_back(0x008A); // S - Caron - Uppercase <--- ??? Duplicate?
    v.push_back(0x00BD); // S - Caron - Uppercase
    v.push_back(0x009A); // s - Caron - Lowercase
    v.push_back(0x0087); // U - Ring Above - Uppercase
    v.push_back(0x009F); // u - Ring Above - Lowercase
    v.push_back(0x008E); // Z - Caron - Uppercase
    v.push_back(0x009E); // z - Caron - Lowercase

    return v;
}

// --- Get UTF text vector --- //
QVector<wchar_t> String::getUtfCharacterVector()
{
    QVector<wchar_t> v;

    v.push_back(0x010C); // C - Caron - Uppercase
    v.push_back(0x010D); // c - Caron - Lowercase
    v.push_back(0x010E); // D - Caron - Uppercase
    v.push_back(0x010F); // d - Caron - Lowercase
    v.push_back(0x011A); // E - Caron - Uppercase
    v.push_back(0x011B); // e - Caron - Lowercase
    v.push_back(0x0147); // N - Caron - Uppercase
    v.push_back(0x0148); // n - Caron - Lowercase
    v.push_back(0x0158); // R - Caron - Uppercase
    v.push_back(0x0159); // r - Caron - Lowercase
    v.push_back(0x02A5); // S - Caron - Uppercase <--- ??? Duplicate?
    v.push_back(0x0160); // S - Caron - Uppercase
    v.push_back(0x0161); // s - Caron - Lowercase
    v.push_back(0x016E); // U - Ring Above - Uppercase
    v.push_back(0x016F); // u - Ring Above - Lowercase
    v.push_back(0x017D); // Z - Caron - Uppercase
    v.push_back(0x017E); // z - Caron - Lowercase

    return v;
}


/* ================ */
/*      Settings    */
/* ================ */

// --- Ignore accented characters --- //
void String::ignoreAccents(bool ignore)
{
    s_IgnoreAccents = ignore;
}

// --- Use accented characters --- //
void String::useAccents(bool use)
{
    s_IgnoreAccents = !use;
}
