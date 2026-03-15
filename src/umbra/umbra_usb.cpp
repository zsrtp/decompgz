#include "umbra/umbra_usb.h"
#ifdef PLATFORM_WII
#include <revolution/exi.h>
#include <revolution/card.h>
#else
#include <dolphin/exi.h>
#include <dolphin/card.h>
#endif

namespace umbra {

bool usb_probe(int chan) {
    if (chan != CARD_SLOT_A && chan != CARD_SLOT_B)
        return false;

    if (!EXIProbe(chan))
        return false;

    u32 id;
    if (!EXIGetID(chan, 0, &id))
        return false;
    if (id != 0)
        return false;

    if (!EXILock(chan, 0, NULL))
        return false;
    if (!EXISelect(chan, 0, 5)) {
        EXIUnlock(chan);
        return false;
    }

    u16 cmd = 0x9000;
    if (!EXIImm(chan, &cmd, sizeof(cmd), 2, NULL) || !EXISync(chan)) {
        EXIDeselect(chan);
        EXIUnlock(chan);
        return false;
    }

    EXIDeselect(chan);
    EXIUnlock(chan);
    return (cmd & 0x0fff) == 0x0470;
}

static int usb_transfer(int chan, void *data, int len, bool write) {
    if (chan != CARD_SLOT_A && chan != CARD_SLOT_B)
        return -1;

    if (!EXILock(chan, 0, NULL)) {
        return -1;
    }

    if (!EXISelect(chan, 0, 5)) {
        return -1;
    }

    bool fail = false;

    u8 *p = (u8*)data;

    int xfer_len = 0;
    for (; xfer_len < len; ++xfer_len) {
        u16 cmd;
        if (write)
            cmd = 0xb000 | p[xfer_len] << 4;
        else
            cmd = 0xa000;

        if (!EXIImm(chan, &cmd, sizeof(u16), 2, NULL) || !EXISync(chan)) {
            fail = true;
            break;
        }

        u16 success_mask = write ? 0x0400 : 0x0800;
        if (!(cmd & success_mask)) {
            break;
        }

        if (!write)
            p[xfer_len] = cmd & 0xff;
    }

	EXIDeselect(chan);
	EXIUnlock(chan);

    return fail ? -1 : xfer_len;
}

int usb_send(int chan, const void *data, int len) {
    return usb_transfer(chan, (void*)data, len, true);
}

int usb_recv(int chan, void *data, int len) {
    return usb_transfer(chan, data, len, false);
}

}
