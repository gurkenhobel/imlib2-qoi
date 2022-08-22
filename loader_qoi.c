#define QOI_IMPLEMENTATION

#include "imlib2_common.h"
#include "loader.h"
#include "qoi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *strdup(const char *ptr1)
{
    char *ptr2 = malloc(strlen(ptr1) + 1);
    strcpy(ptr2, ptr1);
    return ptr2;
}

void rgbatobgra(DATA32 *pixels, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        DATA32 rgba = pixels[i];
        pixels[i] = (rgba & 0xff00ff00) | ((rgba & 0x000000ff) << 16) | ((rgba & 0x00ff0000) >> 16);
    }
}

char load(ImlibImage *im, ImlibProgressFunction progress,
          char progress_granularity, char immediate_load)
{

    if (im->data)
    {
        return 0;
    }

    qoi_desc desc;
    DATA32 *pixels = qoi_read(im->real_file, &desc, 0);

    if (!im->loader && !im->data)
    {
        im->w = desc.width;
        im->h = desc.height;

        if (!IMAGE_DIMENSIONS_OK(desc.width, desc.height))
        {
            QOI_FREE(pixels);
            return 0;
        }
        SET_FLAGS(im->flags, F_HAS_ALPHA);
        im->format = strdup("qoi");
    }

    if((!im->data && im->loader) || immediate_load || progress) 
    {
        rgbatobgra(pixels, desc.width, desc.height);
        im->data = pixels;

        if (progress)
            progress(im, 100, 0, 0, desc.width, desc.height);
    }


    return 1;
}

char save(ImlibImage *im, ImlibProgressFunction progress,
          char progress_granularity)
{
    return 0;
}

void formats(ImlibLoader *l)
{
    int i;
    char *list_formats[] = {"qoi"};

    l->num_formats = (sizeof(list_formats) / sizeof(char *));
    l->formats = malloc(sizeof(char *) * l->num_formats);
    for (i = 0; i < l->num_formats; i++)
        l->formats[i] = strdup(list_formats[i]);
}