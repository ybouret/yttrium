
//! \file


#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/chemical/type/latchable.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Species DataBase Base class
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String,Species::Pointer> LibraryType;


        //______________________________________________________________________
        //
        //
        //
        //! Species Database
        //
        //
        //______________________________________________________________________
        class Library :
        public Proxy<const LibraryType>,
        public Assembly,
        public Serializable,
        public Latchable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const          CallSign;       //!< "Library"
            typedef LibraryType::ConstIterator ConstIterator;  //!< alias
            static const int PMIN = -10;
            static const int PMAX =  1;
            static xreal_t   Concentration(Random::Bits &) noexcept;
            static void      Concentrations(XWritable    &,
                                            Random::Bits &,
                                            const real_t probaZero = 0) noexcept;


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();          //!< setup
            virtual ~Library() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Library);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! (create new|query existing) species
            template <typename NAME> inline
            const Species & operator()(const NAME &name)
            {
                const Species::Pointer sp = new Species(name,nextIndex());
                return setup(sp);
            }

            virtual size_t serialize(OutputStream &) const;        //!< [Serializable] all species
            void           readFrom(InputStream &);                //!< retrieve all species
            size_t         nextIndex() const noexcept;             //!< for species creation
            bool           owns(const Species &sp) const noexcept; //!< checkup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            Y_PROXY_DECL();
            LibraryType db;

            const Species & setup(const Species::Pointer &sp);

        };


    }

}

#endif

