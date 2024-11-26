
//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/indexed.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/chemical/formula.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Species is a charged entity
        //
        //
        //______________________________________________________________________
        class Species : public Indexed, public Counted, public Formula
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<const String,const Species>   Handle;   //!< alias
            typedef SuffixSet<const String,const Handle> Set;      //!< alias
            static const char * const                    CallSign; //!< "Chemical::Species"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! build with XCode, Formula, string...
            template <typename DATA> inline
            Species(const DATA &data, const size_t topIndex) :
            Indexed(topIndex),
            Formula(data)
            {

            }

            //! cleanup
            virtual ~Species() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }

}

#endif

