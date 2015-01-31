#ifndef __LAUNCHTIME_WINDOW__
#define __LAUNCHTIME_WINDOW__ 1

#include "Launchtime.h"
#include <QtGui/QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <memory>

class LaunchtimeWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit LaunchtimeWindow(QWidget *parent = 0);
    ~LaunchtimeWindow();

public slots:

    void quit();
    void connectButtonClicked();

private:

    void fillMidiDevicesBox();

    std::unique_ptr<launchtime::Launchtime> mLaunchtime;

    QComboBox *mMidiDevicesBox;
    QLineEdit *mLaunchtimeNameLine;
    QSpinBox *mLaunchtimeChannel;
    QPushButton *mConnectButton;
    QFormLayout *mLayout;
    QWidget *mWindow;
};

#endif /* __LAUNCHTIME_WINDOW__ */

