
//! \file


#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chem/species.hpp"
#include "y/chem/type/entities.hpp"
#include "y/chem/type/defs.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/proxy.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //! internal database for library
        //______________________________________________________________________
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const        CallSign;       //!< Chemical::Library
            typedef SpeciesDB::ConstIterator ConstIterator;  //!< alias
            static const int                 Log10Max =   1; //!< Cmax = 10^Log10Max
            static const int                 Log10Min = -30; //!< Cmin = 10^Log10Min
            static double                    Conc(Random::Bits &) noexcept; //!< 10^(rand(Log10Min,Log10Max))
            

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();                //!< setup
            explicit Library(const Library &); //!< copy
            virtual ~Library() noexcept;       //!< cleanup
            Y_OSTREAM_PROTO(Library);          //!< display info

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! on the fly create/get species with name/charge
            template <typename NAME>
            const Species & operator()(const NAME &name, const int z)
            { return query(name,z); }


            //! fill top-level array of Conc(ran)
            template <typename T>
            inline void fill(Writable<T> &C, Random::Bits &ran) const
            {
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    const Species &sp = **it;
                    C[sp.indx[TopLevel]] = Conc(ran);
                }
            }

            //! formatted display of transformed data
            template <typename ARRAY, typename PROC>   inline
            void operator()(std::ostream &os,
                            const char   *pfx,
                            ARRAY &       arr,
                            const char   *sfx,
                            PROC          proc) const
            {
                pfx = Safe(pfx);
                sfx = Safe(sfx);
                Init(os);
                for(ConstIterator it=db.begin();it!=db.end();++it)
                {
                    const Species &sp = **it;
                    pad(os << pfx << sp.name << sfx,sp) << IsEq() << proc(arr[ sp.indx[TopLevel] ]) << std::endl;
                }
                Quit(os);
            }

            //! formatted display
            template <typename ARRAY>   inline
            void operator()(std::ostream &os,
                            const char   *pfx,
                            ARRAY &       arr,
                            const char   *sfx) const
            {

                (*this)(os,pfx,arr,sfx, Conv::Id<typename ARRAY::MutableType>);
            }


        private:
            Y_DISABLE_ASSIGN(Library);
            virtual ConstInterface & surrogate() const noexcept;
            const Species &query(const String &, const int);
            const Species &query(const char   *, const int);

            SpeciesDB db;
            static const char *Safe(const char *ch) noexcept { return ch ? ch : ""; }
            static void        Init(std::ostream &os) { os << "{" << std::endl; }
            static void        Quit(std::ostream &os) { os << "}" << std::endl; }
            static const char *IsEq() noexcept { return " = "; }
        };

    }

}

#endif

