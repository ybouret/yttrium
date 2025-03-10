
//! \file


#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<const String,Species::Pointer> LibraryType;

        class Library : public Proxy<const LibraryType>
        {
        public:
            static const char * const CallSign;

            explicit Library();
            virtual ~Library() noexcept;

            template <typename NAME> inline
            const Species & operator()(const NAME &name)
            {
                const Species::Pointer         sp = new Species(name);
                const String &                 id = sp.key();
                {
                    const Species::Pointer * const pp = db.search(id);
                    if(0!=pp) return **pp;
                }
                if(!db.insert(sp)) throwFailedInsertion();
                return *sp;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            Y_PROXY_DECL();
            LibraryType db;

            void throwFailedInsertion() const;
        };


    }

}

#endif

