//! \file

#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/assembly.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef SuffixSet<String,Species::Ptr> SpeciesSet; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Library: database of species
        //
        //
        //______________________________________________________________________

        class Library : public Proxy<const SpeciesSet>, public Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign;              //!< "Chemical::Library"
            typedef SpeciesSet::ConstIterator ConstIterator; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            explicit Library();           //!< setup
            virtual ~Library() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Library);     //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! record/recall species
            template <typename ID>
            const Species & operator()(const ID &name,
                                       const int z)
            {
                Species::Ptr sp = new Species(name,z,db.size()+1);
                return tryInsert(sp);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            SpeciesSet db;

            virtual ConstInterface & surrogate() const noexcept;
            const Species &          tryInsert(const Species::Ptr &sp);
        };
    }

}

#endif
