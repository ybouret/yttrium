

//! \file

#ifndef Y_Memory_Exclusive_Impl_Included
#define Y_Memory_Exclusive_Impl_Included 1

#include "y/memory/studio.hpp"

//! implemented values for a given class
#define Y_Studio(CLASS,LIFE_TIME,STARTING) \
template <> const char * const               Yttrium:: Studio<CLASS>:: CallSign = "Studio<" #CLASS ">"; \
template <> const Yttrium::AtExit::Longevity Yttrium:: Studio<CLASS>:: LifeTime = (LIFE_TIME);          \
template <> const size_t                     Yttrium:: Studio<CLASS>:: Starting = (STARTING)

//! implement operator new/delete for  given class
#define Y_EXCLUSIVE_IMPL(CLASS) \
/**/  void * CLASS:: operator new(size_t blockSize) {                                       \
/**/    static Yttrium::Studio<CLASS> &mgr = Yttrium:: Studio<CLASS>:: Single:: Instance(); \
/**/    mgr.criticalCheck(blockSize,Yttrium:: Studio<CLASS>:: CallSign);                    \
/**/    Y_LOCK(Yttrium::Studio<CLASS>::Single::Access);                                     \
/**/    return mgr.zacquire(); \
/**/  }\
void  CLASS:: operator delete(void *blockAddr, size_t blockSize) noexcept {                 \
/**/    if(0==blockAddr) return;                                                            \
/**/    static Yttrium::Studio<CLASS> &mgr = Yttrium:: Studio<CLASS>:: Single:: Location(); \
/**/    mgr.criticalCheck(blockSize,Yttrium:: Studio<CLASS>:: CallSign);                    \
/**/    Y_LOCK(Yttrium::Studio<CLASS>::Single::Access);                                     \
/**/    mgr.zrelease(blockAddr);                                                            \
/**/  }


#endif

