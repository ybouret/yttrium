
//! \file


#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chem/species.hpp"
#include "y/chem/type/entities.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<String,Species::Pointer> SpeciesDB;

        //______________________________________________________________________
        //
        //
        //
        //! Library of peristent species
        //
        //
        //______________________________________________________________________
        class Library : public Entities, public Proxy<const SpeciesDB>
        {
        public:
            static const char * const CallSign;

            explicit Library();
            Library(const Library &);
            virtual ~Library() noexcept;

            template <typename NAME>
            const Species & operator()(const NAME &name, const int z)
            { return query(name,z); }


        private:
            Y_DISABLE_ASSIGN(Library);       
            virtual ConstInterface & surrogate() const noexcept;
            const Species &query(const String &, const int);
            const Species &query(const char   *, const int);

            SpeciesDB db;


        };

    }

}

#endif

