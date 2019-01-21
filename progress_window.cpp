#include "progress_window.h"

// --- Static setting: Hide progress window --- //
bool ProgressWindow::s_HideWindow = false;

/* ========================= */
/*      Progress Window      */
/* ========================= */

// --- Default constructor --- //
ProgressWindow::ProgressWindow(QString t, unsigned int r, bool ok) :
    counter(0),
    title(t),
    useOkButton(ok)
{
    // Construct progress dialog window
    win = new QProgressDialog(title, "OK", 0, max, 0, Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    win->setWindowTitle(title);
    win->setLabelText("Processing...");

    // Dimensions
    win->setMinimumWidth(250);

    // Initialise the progress counter to zero
    win->setValue(0);

    // Set range and refresh threshold
    this->setRange(r+2);

    // Hide the OK button if appropriate
    if(!useOkButton)
        win->setCancelButton(0);

    // Window behaviour
    win->setAutoReset(false);
    win->setModal(true);

    // Display the progress dialog window (if not hidden)
    if(s_HideWindow == false)
        win->show();
    else    // Else don't show the window and reset the Hide Window setting for next time
        s_HideWindow = false;
}


/* =========================== */
/*      Progress Controls      */
/* =========================== */

// --- Complete the progress bar --- //
void ProgressWindow::complete(QString message)
{
    win->setValue(max);
    win->setLabelText(message);

    if(useOkButton == false)
        win->close();

    QApplication::processEvents();
}

// --- Hide the progress window --- //
void ProgressWindow::hide()
{
    s_HideWindow = true;
    win->accept();
}

// --- Increment the progress bar --- //
void ProgressWindow::increment(QString label)
{
    win->setLabelText(label);
    win->setValue(++counter);

    QApplication::processEvents();
}

// --- Increment the progress bar without updating the label text --- //
void ProgressWindow::incrementSilent(int i)
{
    counter += i;
    win->setValue(counter);
    QApplication::processEvents();
}

// --- Increment the progress bar but only refresh every 1 percent --- //
void ProgressWindow::incrementStep()
{
    ++counter;
    win->setValue(counter);

    if(step % counter == 0)
        QApplication::processEvents();
}

// --- Increment the progress bar but only refresh every 1 percent --- //
void ProgressWindow::incrementSteps(unsigned int i)
{
    counter += i;
    win->setValue(counter);

    if(step % counter == 0)
        QApplication::processEvents();
}

// --- Set the progress bar to a specific value but only refresh every 1 percent --- //
void ProgressWindow::setSteps(unsigned int i)
{
    win->setValue(i);

    if(step % i == 0)
        QApplication::processEvents();
}

// --- Set the progress bar to a specific value --- //
void ProgressWindow::setValue(int i)
{
    win->setValue(i);
    QApplication::processEvents();
}


/* =========================== */
/*      Progress Settings      */
/* =========================== */

// --- Setup the progress bar ready to start --- //
void ProgressWindow::set(QString t, unsigned int r, bool ok)
{
    // Initialise the progress counter to zero
    win->setValue(0);
    QApplication::processEvents();

    setTitle(t);
    setRange(r+2);

    useOkButton = ok;
}

// --- Set the max range --- //
void ProgressWindow::setRange(unsigned int r)
{
    // Set range
    max = r+2;
    win->setRange(0, max);

    // Step refresh threshold
    if(max > 2)
        step = max / 50;    // 100/50 = Update every 2 per cent.
    else
        step = 1;
}

/* ===================== */
/*      Close Event      */
/* ===================== */

// --- Reset the dialog window on close --- //
void ProgressWindow::closeEvent(QCloseEvent *event)
{
    if(counter == max) {
        win->setValue(0);
        event->accept();
    }
    else
        event->ignore();
}
