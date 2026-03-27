#include <QDebug>
#include "ftkInterface.h"

uint64 g_deviceSN = 0;

void deviceCallback(uint64 sn, void* user, ftkDeviceType type)
{
    qDebug() << "Device Found! SN:" << sn;
    g_deviceSN = sn;
}

void runAtracsys()
{
    qDebug() << "Initializing Atracsys SDK...";

    ftkLibrary lib = ftkInit();

    if (!lib)
    {
        qDebug() << "❌ SDK Initialization Failed!";
        return;
    }

    qDebug() << "✅ SDK Initialized Successfully!";
    qDebug() << "Scanning for devices...";

    if (ftkEnumerateDevices(lib, deviceCallback, nullptr) != ftkError::FTK_OK)
    {
        qDebug() << "❌ Device Enumeration Failed!";
        ftkClose(&lib);
        return;
    }

    if (g_deviceSN == 0)
    {
        qDebug() << "❌ No device connected!";
    }
    else
    {
        qDebug() << "✅ Device connected successfully!";
    }

    ftkClose(&lib);
    qDebug() << "SDK Closed Successfully.";
}
