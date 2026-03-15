#ifndef UMBRA_USB_H
#define UMBRA_USB_H

#include <dolphin/types.h>

/// Based off of ttyd-tools https://github.com/PistonMiner/ttyd-tools/blob/master/ttyd-tools/rel/source/ug.cpp

namespace umbra {

/// Checks if there is a USB Gecko 
bool usb_probe(int chan);

int usb_send(int chan, const void *data, int len);

int usb_recv(int chan, void *data, int len);

}

#endif // UMBRA_USB_H
