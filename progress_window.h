#ifndef PROGRESS_WINDOW_H
#define PROGRESS_WINDOW_H

#include <QApplication>
#include <QCloseEvent>
#include <QProgressDialog>
#include <QString>

#pragma warning(disable: 4103)  // Ignore pragma pack warnings. Packing is needed for compatibility with CM and EHM
#pragma pack(1)

class ProgressWindow
{
private:
    QProgressDialog *win;

    unsigned int counter;
    unsigned int max;
    unsigned int step;
    bool useOkButton;    
    QString title;

    static bool s_HideWindow;

public:
    // Default constructor
    ProgressWindow(QString t = "Progress Dialog Window", unsigned int r = 100, bool ok = false);

    // Default destructor
    ~ProgressWindow() {}

    // Progress controls
    void abort() { win->cancel(); }
    void complete(QString message = "Process Complete");
    void hide();
    void increaseRange(unsigned int r) { setRange(max + r);}
    void increment(QString label = "Processing. Please Wait...");
    void incrementSilent(int i = 1);
    void incrementStep();
    void incrementSteps(unsigned int i);
    void setSteps(unsigned int i);
    void setValue(int i);

    // Progress settings
    static void hideWindow(bool b = true) { s_HideWindow = b; }
    void set(QString t, unsigned int r = 100, bool ok = false);
    void setLabel(QString label) { win->setLabelText(label); }
    void setRange(unsigned int r);
    void setTitle(QString t) { title = t; win->setWindowTitle(title); }

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PROGRESS_WINDOW_H
