#ifndef __CCLIB_PTR_INCLUDED__
#define __CCLIB_PTR_INCLUDED__

#ifdef USE_TR1
#include <tr1/memory>
#define CC_PTR std::tr1::shared_ptr
#define CC_DYN_PTR_CAST std::tr1::dynamic_pointer_cast
#else
#include <memory>
#define CC_PTR std::shared_ptr
#define CC_DYN_PTR_CAST std::dynamic_pointer_cast
#endif

#endif
