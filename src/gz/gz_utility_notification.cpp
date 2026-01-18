#include "gz/gz_utility_notification.h"
#include "SSystem/SComponent/c_counter.h"

gzNotification_c::gzNotification_c() {
    mNumNotifications = 0;
    for (int i = 0; i < NOTIFICATION_MAX; i++) {
        mpNotifications[i] = NULL;
        mStartFrames[i] = 0;
    }
    mType = NOTIFY_INFO;
}

gzNotification_c::gzNotification_c(NotificationType i_type) {
    mNumNotifications = 0;
    for (int i = 0; i < NOTIFICATION_MAX; i++) {
        mpNotifications[i] = NULL;
        mStartFrames[i] = 0;
    }
    mType = i_type;
}

gzNotification_c::~gzNotification_c() {
    for (int i = 0; i < NOTIFICATION_MAX; i++) {
        gzTextBox_free(mpNotifications[i]);
        mpNotifications[i] = NULL;
    }
}

void gzNotification_c::send(const char* message, NotificationType i_notificationType) {
    mType = i_notificationType;
    send(message);
}

void gzNotification_c::send(const char* message) {
    if (mNumNotifications == NOTIFICATION_MAX) {
        // FIFO
        gzTextBox_free(mpNotifications[0]);
        for (int i = 0; i < NOTIFICATION_MAX - 1; i++) {
            mpNotifications[i] = mpNotifications[i + 1];
            mStartFrames[i] = mStartFrames[i + 1];
        }
        mNumNotifications--;
    }


    mpNotifications[mNumNotifications] = gzTextBox_allocate();
    mpNotifications[mNumNotifications]->setString(message);
    mStartFrames[mNumNotifications] = cCt_getFrameCount();
    mNumNotifications++;
}

void gzNotification_c::draw() {
    removeExpired();

    u32 current = cCt_getFrameCount();
    const u32 ttl_frames = 5 * 30;
    const u32 fade_frames = ttl_frames / 10;
    const u32 wait_frames = ttl_frames - fade_frames;

    f32 x_alignment = 0.0f;
    f32 y_alignment = 30.0f;
    f32 slide_duration = 5.0f;

    for (int i = 0; i < mNumNotifications; i++) {
        u32 age = current - mStartFrames[i];
        u32 alpha = 255;
        if (age >= wait_frames) {
            u32 fade_age = age - wait_frames;
            if (fade_age < fade_frames) {
                u32 remaining = fade_frames - fade_age;
                alpha = (remaining * 255) / fade_frames;
            } else {
                alpha = 0;
            }
        }
        // TODO(Pheenoh): fix this
        u32 color;
        switch (mType) {
        case NOTIFY_INFO:
            color = 0xFFFFFF00;
            break;
        case NOTIFY_WARNING:
            color = 0xFFFF0000;
            break;
        case NOTIFY_ERROR:
            color = 0xFF000000;
            break;
        }
        color |= alpha;

        f32 y_offset = 0.0f;
        if (age < slide_duration) {
            f32 time0 = 0.0f;
            f32 value0 = -50.0f;  // Start off-screen
            f32 tan_out0 = 0.0f;
            f32 time1 = slide_duration;
            f32 value1 = 0.0f;
            f32 tan_in1 = 0.0f;

            f32 current_time = (f32)age;
            y_offset = J2DHermiteInterpolation<f32>(current_time, &time0, &value0, &tan_out0, &time1, &value1, &tan_in1);
        }

        f32 y_pos = y_alignment + ((mNumNotifications - 1 - i) * 22.0f) + y_offset;

        mpNotifications[i]->draw(x_alignment, y_pos, color, HBIND_CENTER);
    }
}

void gzNotification_c::removeExpired() {
    u32 current = cCt_getFrameCount();
    u32 ttl_frames = 5 * 30;

    while (mNumNotifications > 0 && (current - mStartFrames[0]) > ttl_frames) {
        gzTextBox_free(mpNotifications[0]);
        for (int i = 0; i < mNumNotifications - 1; i++) {
            mpNotifications[i] = mpNotifications[i + 1];
            mStartFrames[i] = mStartFrames[i + 1];
        }
        mNumNotifications--;
    }
}
