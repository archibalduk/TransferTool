#ifndef STRING_H
#define STRING_H

#include <QString>
#include <QVector>

#pragma warning(disable: 4103)
#pragma pack(1)


// --- QString <--> SI String wrapper/interface class --- //
class String
{
private:
    // Text string
    QString m_Text;

    // Settings
    static bool s_IgnoreAccents;

    // Static char conversion data
    static QVector<wchar_t> getAccentedCharacterVector();
    static QVector<wchar_t> getUnaccentedCharacterVector();
    static QVector<wchar_t> getEncodedCharacterVector();
    static QVector<wchar_t> getUtfCharacterVector();
    static QVector<wchar_t> s_AccentedCharacters;
    static QVector<wchar_t> s_UnaccentedCharacters;
    static QVector<wchar_t> s_EncodedCharacters;
    static QVector<wchar_t> s_UtfCharacters;

public:
    // Constructor
    String();
    String(const QString text);
    String(const char text[]);

    // Get data
    QString get();
    QString getMatchDateString();
    QString getMatchString();
    QString getNoAccentsString();
    void toSIFont(char *SITextArray, int length);

    // Settings
    static void ignoreAccents(bool ignore = true);
    static void useAccents(bool use = true);
};

#endif // STRING_H
