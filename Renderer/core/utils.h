#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

class Image;
class FrameBuffer;
typedef unsigned char Byte;

/*
*  read/write file
*/
Byte read_byte(FILE *file);
void read_bytes(FILE *file, void *buffer, int size);
void write_bytes(FILE *file, void *buffer, int size);

/*
*  load/save file of certain format
*/
void load_tga(FILE *file, Image *image);
void save_tga(const Image *image, const char *filePath);

/*
*  blit image data
*/
void blit_image_bgr(Image *src, int buffer_width, int buffer_height, Byte* buffer);
void blit_image_rgb(Image *src, int buffer_width, int buffer_height, Byte* buffer);
void blit_frame_bgr(FrameBuffer* src, int buffer_width, int buffer_height, Byte* buffer);

/*
*  misc functions 
*/
const char *get_extension(const char *filename);

/*
*  math functions
*/
double lerp(double d0, double d1, double t);

#endif