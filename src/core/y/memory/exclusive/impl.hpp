

//! \file

#ifndef Y_Memory_Exclusive_Impl_Included
#define Y_Memory_Exclusive_Impl_Included 1

#include "y/memory/exclusive/studio.hpp"

//______________________________________________________________________________
//
//
//! implement values for a given class
//
//______________________________________________________________________________
#define Y_Studio(CLASS,LIFE_TIME,STARTING) \
template <> const char * const               Yttrium:: Studio<CLASS>:: CallSign = "Studio<" #CLASS ">"; \
template <> const Yttrium::AtExit::Longevity Yttrium:: Studio<CLASS>:: LifeTime = (LIFE_TIME);          \
template <> const size_t                     Yttrium:: Studio<CLASS>:: Starting = (STARTING)

#if defined(Y_INTEL)
#define Y_EXCLUSIVE_IMPL_EXTRA(CLASS) \
void * CLASS:: operator new[](const size_t) {                                               \
/**/    typedef Yttrium::Studio<CLASS> Self;                                                \
/**/    return Self::ThrowUnauthorized(#CLASS,Self::MultipleNew);                           \
/**/  }                                                                                     \
void   CLASS:: operator delete [](void *, const size_t ) noexcept {                         \
/**/    typedef Yttrium::Studio<CLASS> Self;                                                \
/**/    Self::AbortUnauthorized(#CLASS,Self::MultipleDelete);                               \
/**/  }                                                                                     \
void * CLASS:: operator new(size_t, void *) {                                               \
/**/    typedef Yttrium::Studio<CLASS> Self;                                                \
/**/    return Self::ThrowUnauthorized(#CLASS,Self::PlacementNew);                          \
/**/  }                                                                                     \
void   CLASS:: operator delete(void *, void *) noexcept {                                   \
/**/    typedef Yttrium::Studio<CLASS> Self;                                                \
/**/    Self::AbortUnauthorized(#CLASS,Self::PlacementDelete);                              \
/**/  }

#else

#define Y_EXCLUSIVE_IMPL_EXTRA(CLASS)

#endif

//______________________________________________________________________________
//
//
//! implement operator new/delete for  given class
//
//______________________________________________________________________________
#define Y_EXCLUSIVE_IMPL(CLASS) \
/**/  void * CLASS:: operator new(size_t blockSize) {                                       \
/**/    static Yttrium::Studio<CLASS> &mgr = Yttrium:: Studio<CLASS>:: Single:: Instance(); \
/**/    (void)blockSize; assert(blockSize<=mgr.blockSize());                                \
/**/    Y_LOCK(Yttrium::Studio<CLASS>::Single::Access);                                     \
/**/    return mgr.zacquire();                                                              \
/**/  }                                                                                     \
void  CLASS:: operator delete(void *blockAddr, size_t blockSize) noexcept {                 \
/**/    if(0==blockAddr) return;                                                            \
/**/    static Yttrium::Studio<CLASS> &mgr = Yttrium:: Studio<CLASS>:: Single:: Location(); \
/**/    (void)blockSize; assert(blockSize<=mgr.blockSize());                                \
/**/    Y_LOCK(Yttrium::Studio<CLASS>::Single::Access);                                     \
/**/    mgr.zrelease(blockAddr);                                                            \
/**/  }                                                                                     \
Y_EXCLUSIVE_IMPL_EXTRA(CLASS)

#endif

