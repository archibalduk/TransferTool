#include "style_sheet.h"

// --- Get style sheet from a css file --- //
void StyleSheet::get()
{
    QString s;
    QFile f(filename);

    // Check for an existing css file
    if(f.open(QIODevice::ReadOnly)) {
        s = QLatin1String(f.readAll());
    }
    // Use default settings if no existing css file is found
    else {
        this->getDefaultStyleSheet(s);
        this->save(s);
    }

    qApp->setStyleSheet(s);
}

// --- Save style sheet to a css file --- //
void StyleSheet::save(QString &s)
{
    QFile f(filename);

    if(f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream stream(&f);
        stream << s;
        f.close();
    }
}

// --- Default style sheet --- //
void StyleSheet::getDefaultStyleSheet(QString &s)
{
    s =     "/* Global font styling */" "\n"
            "* {" "\n"
            "\t" "font-family: \"Segoe UI\", Verdana, Geneva, sans-serif;" "\n"
            "}\n"
            "\n"
            "/* GROUP BOXES: */" "\n"
            "#TextSettingGroup {" "\n"
            "\t" "font-weight: bold;" "\n"
            "}\n"
            "/* TEXT: */" "\n"
            "#TextSettingTitle {" "\n"
            "\t" "font-weight: bold;" "\n"
            "\t" "margin: 0px 0px 0px 0px;" "\n"
            "\t" "padding: 0px;" "\n"
            "}\n"
            "#TextSettingDescription {" "\n"
            "\t" "margin: 0px;" "\n"
            "\t" "padding: 0px;" "\n"
            "}\n"
            "#TextSummaryLarge {" "\n"
            "\t" "font-size: 12pt;" "\n"
            "\t" "font-family: \"Segoe UI Light\", Verdana, Geneva, sans-serif;" "\n"
            "\t" "color: #000000;" "\n"
            "}\n"
            "/* SUMMARY BOXES: */" "\n"
            "#UI_SummaryError {" "\n"
            "\t" "background-color: #e74c3c;" "\n"
            "\t" "border: 5px solid #e74c3c;" "\n"
            "\t" "border-radius: 6px;" "\n"
            "\t" "color: #ffffff;" "\n"
            "}\n"
            "#UI_SummaryErrorText {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 18pt;" "\n"
            "\t" "font-family: \"Segoe UI Light\", Verdana, Geneva, sans-serif;" "\n"
            "}\n"
            "#UI_SummaryErrorValue {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 24pt;" "\n"
            "}\n"
            "#UI_SummaryInfo {" "\n"
            "\t" "background-color: #3498db;" "\n"
            "\t" "border: 5px solid #3498db;" "\n"
            "\t" "border-radius: 6px;" "\n"
            "\t" "color: #ffffff;" "\n"
            "}\n"
            "#UI_SummaryInfoText {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 18pt;" "\n"
            "\t" "font-family: \"Segoe UI Light\", Verdana, Geneva, sans-serif;" "\n"
            "}\n"
            "#UI_SummaryInfoValue {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 24pt;" "\n"
            "}\n"
            "#UI_SummarySuccess {" "\n"
            "\t" "background-color: #2ecc71;" "\n"
            "\t" "border: 3px solid #2ecc71;" "\n"
            "\t" "border-radius: 6px;" "\n"
            "\t" "color: #ffffff;" "\n"
            "}\n"
            "#UI_SummarySuccessText {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 18pt;" "\n"
            "\t" "font-family: \"Segoe UI Light\", Verdana, Geneva, sans-serif;" "\n"
            "}\n"
            "#UI_SummarySuccessValue {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 24pt;" "\n"
            "}\n"
            "#UI_SummarySubtle {" "\n"
            "\t" "background-color: #f5f5f5;" "\n"
            "\t" "border: 5px solid #f5f5f5;" "\n"
            "\t" "border-radius: 6px;" "\n"
            "\t" "color: #ffffff;" "\n"
            "}\n"
            "#UI_SummarySubtleText {" "\n"
            "\t" "color: #000000;" "\n"
            "\t" "font-size: 18pt;" "\n"
            "\t" "font-family: \"Segoe UI Light\", Verdana, Geneva, sans-serif;" "\n"
            "}\n"
            "#UI_SummarySubtleValue {" "\n"
            "\t" "color: #000000;" "\n"
            "\t" "font-size: 24pt;" "\n"
            "}\n"
            "#UI_SummaryWarning {" "\n"
            "\t" "background-color: #f39c12;" "\n"
            "\t" "border: 5px solid #f39c12;" "\n"
            "\t" "border-radius: 6px;" "\n"
            "\t" "color: #ffffff;" "\n"
            "}\n"
            "#UI_SummaryWarningText {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 18pt;" "\n"
            "\t" "font-family: \"Segoe UI Light\", Verdana, Geneva, sans-serif;" "\n"
            "}\n"
            "#UI_SummaryWarningValue {" "\n"
            "\t" "color: #ffffff;" "\n"
            "\t" "font-size: 24pt;" "\n"
            "}";
}
