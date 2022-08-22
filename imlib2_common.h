#ifndef _IMLIB2_COMMON_H_
#define _IMLIB2_COMMON_H_

/* most of this file comes directly from imlib2 source code */

#include <Imlib2.h>

#define UNSET_FLAGS(flags, f) ((flags) &= ~(f))
#define SET_FLAGS(flags, f) ((flags) |= (f))

typedef struct _imlibimage              ImlibImage;
# ifdef BUILD_X11
typedef struct _imlibimagepixmap        ImlibImagePixmap;
# endif
typedef struct _imlibborder             ImlibBorder;
typedef struct _imlibloader             ImlibLoader;
typedef struct _imlibimagetag           ImlibImageTag;

typedef int (*ImlibProgressFunction)(ImlibImage *im, char percent,
                                     int update_x, int update_y,
                                     int update_w, int update_h);
typedef void (*ImlibDataDestructorFunction)(ImlibImage *im, void *data);

enum _iflags
{
   F_NONE              = 0,
   F_HAS_ALPHA         = (1 << 0),
   F_UNLOADED          = (1 << 1),
   F_UNCACHEABLE       = (1 << 2),
   F_ALWAYS_CHECK_DISK = (1 << 3),
   F_INVALID           = (1 << 4),
   F_DONT_FREE_DATA    = (1 << 5),
   F_FORMAT_IRRELEVANT = (1 << 6),
   F_BORDER_IRRELEVANT = (1 << 7),
   F_ALPHA_IRRELEVANT  = (1 << 8)
};

typedef enum   _iflags                  ImlibImageFlags;

struct _imlibborder
{
   int left, right, top, bottom;
};

struct _imlibimagetag
{
   char           *key;
   int             val;
   void           *data;
   void          (*destructor)(ImlibImage *im, void *data);
   ImlibImageTag  *next;
};

struct _imlibimage
{
   char             *file;
   int               w, h;
   DATA32           *data;
   ImlibImageFlags   flags;
   time_t            moddate;
   ImlibBorder       border;
   int               references;
   ImlibLoader      *loader;
   char             *format;
   ImlibImage       *next;
   ImlibImageTag    *tags;
   char             *real_file;
   char             *key;
};

# ifdef BUILD_X11
struct _imlibimagepixmap
{
   int               w, h;
   Pixmap            pixmap, mask;
   Display          *display;
   Visual           *visual;
   int               depth;
   int               source_x, source_y, source_w, source_h;
   Colormap          colormap;
   char              antialias, hi_quality, dither_mask;
   ImlibBorder       border;
   ImlibImage       *image;
   char             *file;
   char              dirty;
   int               references;
   DATABIG           modification_count;
   ImlibImagePixmap *next;
};
# endif

struct _imlibloader
{
   char         *file;
   int           num_formats;
   char        **formats;
   void         *handle;
   char        (*load)(ImlibImage *im,
                       ImlibProgressFunction progress,
                       char progress_granularity, char immediate_load);
   char        (*save)(ImlibImage *im,
                       ImlibProgressFunction progress,
                       char progress_granularity);
   ImlibLoader  *next;
};

# define IMAGE_HAS_ALPHA(im) ((im)->flags & F_HAS_ALPHA)
# define IMAGE_IS_UNLOADED(im) ((im)->flags & F_UNLOADED)
# define IMAGE_IS_UNCACHEABLE(im) ((im)->flags & F_UNCACHEABLE)
# define IMAGE_ALWAYS_CHECK_DISK(im) ((im)->flags & F_ALWAYS_CHECK_DISK)
# define IMAGE_IS_VALID(im) (!((im)->flags & F_INVALID))
# define IMAGE_FREE_DATA(im) (!((im)->flags & F_DONT_FREE_DATA))

# define SET_FLAG(flags, f) ((flags) |= (f))
# define UNSET_FLAG(flags, f) ((flags) &= (~f))

# define IMAGE_DIMENSIONS_OK(w, h) \
   ( ((w) > 0) && ((h) > 0) && \
     ((unsigned long long)(w) * (unsigned long long)(h) <= (1ULL << 29) - 1) )

EAPI ImlibImageTag *__imlib_GetTag(ImlibImage *im, const char *key);

#endif /* _IMLIB2_COMMON_H_ */