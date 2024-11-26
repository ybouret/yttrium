//! \file

#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class SpeciesDB : public Species::Set, public Assembly
        {
        public:
            explicit SpeciesDB();
            virtual ~SpeciesDB() noexcept;

            void mustInsert(const Species::Handle &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SpeciesDB);
        };

        class Library : public Proxy<const SpeciesDB>
        {
        public:
            typedef SpeciesDB::ConstIterator ConstIterator;
            static const char * const CallSign;
            explicit Library();
            virtual ~Library() noexcept;
            Y_OSTREAM_PROTO(Library);

            const Species & get(const Formula &);

            template <typename DATA> inline
            const Species & operator()(const DATA &data)
            {
                const Formula formula(data);
                return get(formula);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual ConstInterface & surrogate() const noexcept;
            SpeciesDB db;
        };
    }

}

#endif

