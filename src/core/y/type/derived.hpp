//! \file

#ifndef Y_Type_Derived_Included
#define Y_Type_Derived_Included 1

#include "y/config/starting.hpp"


#define Y_Derived_Standard          //!< helper to declare standard constructor
#define Y_Derived_NoExcept noexcept //!< helper to declare noexcept constructor

//! class CLASS : public BASE, public CLASS_
#define Y_Derived(CLASS,BASE,KIND)                   \
/**/  class CLASS : public BASE, public CLASS##_ {   \
/**/    public:                                      \
/**/      inline virtual ~CLASS() noexcept {}        \
/**/      inline explicit CLASS() Y_Derived_##KIND : \
/**/        BASE(), CLASS##_() {}                    \
/**/    private:                                     \
/**/      Y_DISABLE_COPY_AND_ASSIGN(CLASS);          \
/**/  }


#endif

