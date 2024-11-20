//! \file

#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/proxy.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! base class for database of species
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String,const Species::Handle> SpeciesSet;

        //______________________________________________________________________
        //
        //
        //
        //! Library is an assembly of species
        //
        //
        //______________________________________________________________________
        class Library : public Assembly, public Proxy<const SpeciesSet>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();          //!< setup empty
            virtual ~Library() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Library);    //!< display
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! on-the-fly query/get species
            const Species & operator()(const String &sid, const int z);
            

        private:
            SpeciesSet species;

            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual ConstInterface & surrogate() const noexcept;
        };
    }

}

#endif

