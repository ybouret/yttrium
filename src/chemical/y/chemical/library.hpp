
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
            static const int                   PMIN = -10;     //!< Cmin = 10^PMIN
            static const int                   PMAX =  1;      //!< Cmax = 10^PMAX
            static xreal_t   Concentration(Random::Bits &) noexcept; //!< 10^ran(PMIN,PMAX)

            //! fill array with random concentrations
            static void      Concentrations(XWritable    &,
                                            Random::Bits &,
                                            const real_t probaZero = 0,
                                            const real_t probaNeg  = 0) noexcept;


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

            //! display associated TopLevel array
            template <typename ARRAY> inline
            std::ostream & show(std::ostream &os,
                                const char *pfx,
                                ARRAY      &arr,
                                const char *sfx) const
            {
                os << "{" << std::endl;
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    if(pfx) os << pfx;
                    const Species &sp = **it;
                    os << sp.name;
                    if(sfx) os << sfx;
                    pad(os,sp) << " = " << sp(arr,TopLevel);
                    os << std::endl;
                }
                os << "}";
                return os;
            }

            //! display associated TopLevel proc(array)
            template <typename ARRAY, typename PROC> inline
            std::ostream & show(std::ostream &os,
                                const char *pfx,
                                ARRAY      &arr,
                                const char *sfx,
                                PROC &      proc) const
            {
                os << "{" << std::endl;
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    if(pfx) os << pfx;
                    const Species &sp = **it;
                    os << sp.name;
                    if(sfx) os << sfx;
                    pad(os,sp) << " = " << proc(sp(arr,TopLevel));
                    os << std::endl;
                }
                os << "}";
                return os;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            Y_PROXY_DECL();
            LibraryType db;

            const Species & setup(const Species::Pointer &sp);

        };


    }

}

#endif

