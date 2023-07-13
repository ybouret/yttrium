
//! \file

#ifndef Y_Memory_Exclusive_Decl_Included
#define Y_Memory_Exclusive_Decl_Included 1


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
static inline void * operator new    [](const size_t) noexcept {return 0;}      \
static void   operator delete [](void *, const size_t ) noexcept;               \
static void *operator new(size_t, void *)    noexcept;                          \
static void  operator delete(void *, void *) noexcept;


#endif

