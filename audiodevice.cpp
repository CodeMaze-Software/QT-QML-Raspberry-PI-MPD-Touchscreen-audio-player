#include "audiodevice.h"

AudioDevice::AudioDevice(QString ipAddress, int port, QString pass, int currentMode) : MultimediaPlayer(ipAddress, port, pass)
{
    this->currentMode = currentMode;
}
