#ifndef SUMMARY_ITEM_H
#define SUMMARY_ITEM_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

// --- Summary Item --- //
class SummaryItem : public QWidget
{
private:
    // Text labels
    QLabel *m_Text;
    QLabel *m_Value;

public:
    // Default constructor
    SummaryItem(QWidget *parent, unsigned char type = 0);

    // Set text and value
    void setText(const QString &t);
    void setValue(const int &i);
    void setValue(const QString &t);

    // Set type / formatting
    void setType(unsigned char type);

    enum ENUM_SUMMARY_ITEM_TYPE {
        INFO,
        SUCCESS,
        WARNING,
        ERROR,
        SUBTLE
    };

    enum ENUM_DIMENSIONS {
        ITEM_HEIGHT = 140,
        ITEM_WIDTH = ITEM_HEIGHT + 40
        //ITEM_WIDTH = ITEM_HEIGHT * 2
    };
};

#endif
