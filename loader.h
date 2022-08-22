#ifndef _LOADER_H_
#define _LOADER_H_

#include "imlib2_common.h"

EAPI char load(ImlibImage * im, ImlibProgressFunction progress,
               char progress_granularity, char immediate_load);
EAPI char save(ImlibImage *im, ImlibProgressFunction progress,
               char progress_granularity);
EAPI void formats(ImlibLoader *l);

#endif /* _LOADER_H_ */