#ifndef TEXTURE_H
#define TEXTURE_H

#include "dolphin/gx/GXStruct.h"

enum TexCode {
    TEX_OK = 1,                  /*<@brief Texture loaded successfully */
    TEX_UNLOADED = 0,            /*<@brief Texture hasn't been loaded yet */
    TEX_ERR_FILE = -1,           /*<@brief Could not open texture file */
    TEX_ERR_READ = -2,           /*<@brief Error occured while reading the texture file */
    TEX_ERR_INVALID_FORMAT = -3, /*<@brief The format of the texture file is not supported */
    TEX_ERR_MEM = -4,            /*<@brief Not enough memory to load the texture */
};

struct TexHeader {
    u32 magic;
    u32 format;
    u32 width;
    u32 height;
};

struct Texture {
    TexHeader header;
    u8* data;
    GXTexObj _texObj;
    TexCode loadCode;
};

/**
 * @brief Loads a texture from a file on the DVD.
 *
 * @param[in] path Path to the texture file on the DVD.
 * @param[out] tex Texture object containing the fetched texture.
 * @returns A TexCode code signaling the success/failure of the loading.
 */
TexCode load_texture(const char* path, Texture* tex);
/**
 * @brief Loads a texture at an offset from a file on the DVD.
 *
 * @param[in] path Path to the texture file on the DVD.
 * @param[out] tex Texture object containing the fetched texture.
 * @param[in] offset Offset in the file to start loading the texture from.
 * @returns A TexCode code signaling the success/failure of the loading.
 */
TexCode load_texture_offset(const char* path, Texture* tex, u32 offset);
void free_texture(Texture* tex);

void setupRendering();

#endif // TEXTURE_H
