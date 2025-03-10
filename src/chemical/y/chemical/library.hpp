
//! \file


#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/chemical/type/latchable.hpp"
#include "y/associative/suffix/set.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<const String,Species::Pointer> LibraryType;

        class Library :
        public Proxy<const LibraryType>,
        public Assembly,
        public Serializable,
        public Latchable
        {
        public:
            static const char * const CallSign;

            explicit Library();
            virtual ~Library() noexcept;

            template <typename NAME> inline
            const Species & operator()(const NAME &name)
            {
                const Species::Pointer sp = new Species(name);
                return setup(sp);
            }

            virtual size_t serialize(OutputStream &) const;
            void           readFrom(InputStream &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            Y_PROXY_DECL();
            LibraryType db;

            const Species & setup(const Species::Pointer &sp);

        };


    }

}

#endif

