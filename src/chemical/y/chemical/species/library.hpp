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
        class Library : public Proxy<SpeciesDB>, public Entities
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

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

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
                const String         &target = handle->name;
                {
                    const Species::Handle * const query = sdb.search(target);
                    if(query)
                    {
                        const Species &mine = **query;
                        if(mine.z != z) throwChargeMismatch(target);
                        return mine;
                    }
                }
                assert(0==sdb.search(target));
                if(!sdb.insert(handle)) throwFailedToInsert(target);
                return *handle;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            SpeciesDB sdb;
            virtual ConstInterface & surrogate() const noexcept;
            void    throwFailedToInsert(const String &) const;
            void    throwChargeMismatch(const String &) const;
        };

    }

}

#endif

