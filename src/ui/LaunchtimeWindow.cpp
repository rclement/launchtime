#include "LaunchtimeWindow.h"
#include "MidiInput.h"
#include <QCoreApplication>
#include <QString>
#include <string>
#include <vector>

LaunchtimeWindow::LaunchtimeWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mLaunchtime = std::unique_ptr<launchtime::Launchtime>(new launchtime::Launchtime());

    mMidiDevicesBox = new QComboBox();
    fillMidiDevicesBox();

    mLaunchtimeNameLine = new QLineEdit("Launchtime");

    mConnectButton = new QPushButton("Connect");
    connect(mConnectButton, SIGNAL(clicked()), this, SLOT(connectButtonClicked()));

    mLayout = new QFormLayout();
    mLayout->addRow("Launchpad Device", mMidiDevicesBox);
    mLayout->addRow("Launchtime Name", mLaunchtimeNameLine);
    mLayout->addRow(mConnectButton);

    mWindow = new QWidget();
    mWindow->setLayout(mLayout);

    setCentralWidget(mWindow);
    setWindowTitle("Launchtime");

}

LaunchtimeWindow::~LaunchtimeWindow()
{
}

void LaunchtimeWindow::quit()
{
    QCoreApplication::exit();
}

void LaunchtimeWindow::connectButtonClicked()
{
    if (mLaunchtime->isConnected())
    {
        if (mLaunchtime->disconnect())
        {
            mConnectButton->setText("Connect");
        }
    }
    else
    {
        std::string launchpadName = mMidiDevicesBox->currentText().toStdString();
        std::string launchtimeName = mLaunchtimeNameLine->text().toStdString();

        if (mLaunchtime->connect(launchpadName, launchtimeName))
        {
            mConnectButton->setText("Disconnect");
        }
    }
}

void LaunchtimeWindow::fillMidiDevicesBox()
{
    std::vector< std::string > midiDevices = mLaunchtime->availableMidiDevices();

    for (unsigned int i = 0; i < midiDevices.size(); i++)
    {
        QString midiname = QString::fromStdString(midiDevices[i]);
        mMidiDevicesBox->insertItem(i, midiname);
    }
}

