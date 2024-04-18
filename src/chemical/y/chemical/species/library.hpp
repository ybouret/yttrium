//! \file

#ifndef Y_Chemical_Species_Library_Included
#define Y_Chemical_Species_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/proxy.hpp"
#include "y/chemical/type/entities.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! associative string/species
        //
        //______________________________________________________________________
        typedef SuffixSet<String,Species::Handle> SpeciesDB;

        //______________________________________________________________________
        //
        //
        //
        //! Libary of (topLevel) species
        //
        //
        //______________________________________________________________________
        class Library : public Proxy<SpeciesDB>, public Entities, public Identifiable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Library"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();           //!< setup empty
            virtual ~Library() noexcept;  //!< cleanup

            friend std::ostream & operator<<(std::ostream &, const Library &);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< CallSign


            //__________________________________________________________________
            //
            //!  return new/existing species, checking matching chargess
            /**
             \param name name of the species
             \param z    charge of the species
             */
            //__________________________________________________________________
            template <typename NAME> inline
            const Species & operator()(const NAME &name,
                                       const int   z)
            {
                const size_t          sindex = sdb.size()+1;
                const Species::Handle handle = new Species(name,z,sindex);
                return manage(handle);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            SpeciesDB sdb;
            virtual ConstInterface & surrogate() const noexcept;
            const Species &          manage(const Species::Handle &);

        };

    }

}

#endif

