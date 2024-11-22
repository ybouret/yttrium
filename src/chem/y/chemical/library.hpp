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
        class Library : public Proxy<const SpeciesSet>,
        public Assembly,
        public Serializable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef SpeciesSet::ConstIterator ConstIterator; //!< alias
            static const char * const         CallSign;      //!< "Chemical::Library"

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
            // Interfacer
            //
            //__________________________________________________________________
            virtual size_t serialize(OutputStream &) const; //!< [Serializable] write species

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void           load(InputStream &); //!< if empty, from serialized


            //! on-the-fly query/get species
            const Species & operator()(const String &sid, const int z);
            

        private:
            SpeciesSet species;

            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual ConstInterface & surrogate() const noexcept;
            const Species &          mustInsert(Species * const);
        };
    }

}

#endif

