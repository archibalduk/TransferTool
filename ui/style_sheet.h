#ifndef STYLE_SHEET_H
#define STYLE_SHEET_H

#include <QApplication>
#include <QFile>
#include <QString>
#include <QTextStream>

class StyleSheet
{
private:
    QString filename;

    void getDefaultStyleSheet(QString &s);
    void save(QString &s);

public:
    StyleSheet() : filename("stylesheet.css") {}
    void get();    
};

#endif // STYLE_SHEET_H
