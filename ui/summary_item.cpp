#include "summary_item.h"


/* ====================== */
/*      Summary Item      */
/* ====================== */

// --- Default constructor --- //
SummaryItem::SummaryItem(QWidget *parent, unsigned char type) :
    QWidget(parent)
{
    this->setFixedSize(ITEM_WIDTH, ITEM_HEIGHT);

    m_Text = new QLabel(this);
    m_Text->setWordWrap(true);
    m_Text->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    m_Value = new QLabel(this);
    m_Value->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    this->setType(type);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_Value);
    layout->addWidget(m_Text);
    this->setLayout(layout);
}

// --- Set text label --- //
void SummaryItem::setText(const QString &t)
{
    m_Text->setText(t);
}

// --- Set value label --- //
void SummaryItem::setValue(const int &i)
{
    m_Value->setText(QLocale().toString(i));
}

// --- Set value label --- //
void SummaryItem::setValue(const QString &t)
{
    m_Value->setText(t);
}

// --- Set item type --- //
void SummaryItem::setType(unsigned char type)
{
    switch(type) {
    case INFO:
        this->setObjectName("UI_SummaryInfo");
        m_Text->setObjectName("UI_SummaryInfoText");
        m_Value->setObjectName("UI_SummaryInfoValue");
        break;

    case SUCCESS:
        this->setObjectName("UI_SummarySuccess");
        m_Text->setObjectName("UI_SummarySuccessText");
        m_Value->setObjectName("UI_SummarySuccessValue");
        break;

    case WARNING:
        this->setObjectName("UI_SummaryWarning");
        m_Text->setObjectName("UI_SummaryWarningText");
        m_Value->setObjectName("UI_SummaryWarningValue");
        break;

    case ERROR:
        this->setObjectName("UI_SummaryError");
        m_Text->setObjectName("UI_SummaryErrorText");
        m_Value->setObjectName("UI_SummaryErrorValue");
        break;

    case SUBTLE:
        this->setObjectName("UI_SummarySubtle");
        m_Text->setObjectName("UI_SummarySubtleText");
        m_Value->setObjectName("UI_SummarySubtleValue");
        break;

    default:
        this->setObjectName("UI_SummaryInfo");
        m_Text->setObjectName("UI_SummaryInfoText");
        m_Value->setObjectName("UI_SummaryInfoValue");
        break;
    }
}
