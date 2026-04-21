#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned short imagic;
    unsigned short type;
    unsigned short dim;
    unsigned short sizeX;
    unsigned short sizeY;
    unsigned short sizeZ;
    unsigned char *data;
} IMAGE;

IMAGE *ImageLoad(char *fileName);

#endif