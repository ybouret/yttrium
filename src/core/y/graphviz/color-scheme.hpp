
//! \file

#ifndef Y_Vizible_Color_Scheme_Included
#define Y_Vizible_Color_Scheme_Included 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    namespace GraphViz
    {

        //! GraphViz Color Scheme
        struct ColorScheme
        {
            const char * const         name;    //!< names
            const size_t               size;    //!< colors per name
            static const ColorScheme   Table[]; //!< global table
            static const size_t        Count;   //!< number of available schemes


            static const ColorScheme * Get(const char *id);             //!< get by name
            static const ColorScheme * Get(const Core::String<char> &); //!< get by name

        };

    }



}

#endif


