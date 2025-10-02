#include "gz/disc.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "SSystem/SComponent/c_lib.h"

int dvd_read(DVDFileInfo* file_info, void* data, int size, int offset) {
    const u32 buf_size = 0x20;
    u8* buf = new (-32) u8[buf_size];
    int read_bytes = 0;

    while (size > 0) {
        int result = DVDReadPrio(file_info, buf, buf_size, (offset & ~(buf_size - 1)), 2);
        if (result <= 0) {
            break;
        }
        int rem_size = buf_size - (offset & (buf_size - 1));
        cLib_memCpy((void*)((u32)data + read_bytes), buf + (offset & (buf_size - 1)),
               MIN(rem_size, size));
        read_bytes += MIN(rem_size, size);
        size -= rem_size;
        offset += rem_size;
    }
    delete[] buf;
    return read_bytes;
}
