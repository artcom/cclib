#ifndef __CCLIB_PTR_INCLUDED__
#define __CCLIB_PTR_INCLUDED__

#ifdef USE_TR1
#include <tr1/memory>
#define CC_PTR std::tr1::shared_ptr
#else
#include <memory>
#define CC_PTR std::shared_ptr
#endif

#endif
