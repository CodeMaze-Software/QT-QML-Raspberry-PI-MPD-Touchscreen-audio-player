#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <multimediaplayer.h>

class AudioDevice : public MultimediaPlayer
{
    int currentMode;

public:
     AudioDevice(QString ipAddress, int port, QString pass, int currentMode);

signals:

};

#endif // AUDIODEVICE_H
