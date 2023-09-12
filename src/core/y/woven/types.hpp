
//! \file

#ifndef Y_WOVEn_Types_Included
#define Y_WOVEn_Types_Included 1

#include "y/apex/ortho/family.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        typedef Apex::Ortho::Vector                QVector; //!< alias
        typedef Apex::Ortho::Family                QFamily; //!< alias
        typedef Functor<void,TL1(const QVector &)> QSurvey; //!< alias

    }

}

#endif
