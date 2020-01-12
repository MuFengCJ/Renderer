#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

class Image;
class FrameBuffer;
typedef unsigned char Byte;

/*
*  read/write file
*/
Byte ReadByte(FILE *file);
void ReadBytes(FILE *file, void *buffer, int size);
void WriteBytes(FILE *file, void *buffer, int size);

/*
*  load/save file of certain format
*/
void LoadTGA(FILE *file, Image *image);
void SaveTGA(const Image *image, const char *filePath);

/*
*  blit image data
*/
void blit_image_bgr(Image *src, int buffer_width, int buffer_height, Byte* buffer);
void blit_image_rgb(Image *src, int buffer_width, int buffer_height, Byte* buffer);
void blit_frame_bgr(FrameBuffer* src, int buffer_width, int buffer_height, Byte* buffer);

/*
*  misc functions 
*/
const char *GetExtension(const char *filename);

/*
*  math functions
*/
float Lerp(float d0, float d1, float t);

#endif