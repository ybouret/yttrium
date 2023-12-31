
//! \file

#ifndef Y_Memory_Exclusive_Decl_Included
#define Y_Memory_Exclusive_Decl_Included 1

#include "y/config/starting.hpp"

//______________________________________________________________________________
//
//
//! declare new/delete operators
//
//______________________________________________________________________________
#define Y_EXCLUSIVE_DECL()                                                      \
public:                                                                         \
/* using Studio */ static void * operator new(const size_t );                   \
/* using Studio */ static void   operator delete(void *,const size_t) noexcept; \
private:                                                                        \
static void * operator new    [](const size_t);                                 \
static void   operator delete [](void *, const size_t ) noexcept;               \
static void * operator new(size_t, void *);                                     \
static void   operator delete(void *, void *) noexcept


#endif

