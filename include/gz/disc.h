#ifndef DISK_H
#define DISK_H

#include "dolphin/dvd.h"

int dvd_read(DVDFileInfo*, void*, int, int);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#endif // DISK_H
