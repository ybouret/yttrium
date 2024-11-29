//! \file

#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! database of species for Library
        //
        //
        //______________________________________________________________________
        class Ingredients : public Species::Set, public Assembly
        {
        public:
            explicit Ingredients();                   //!< setup empty
            virtual ~Ingredients() noexcept;          //!< cleanup
            void mustInsert(const Species::Handle &); //!< insert new species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ingredients);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Database of Species
        //
        //
        //______________________________________________________________________
        class Library : public Proxy<const Ingredients>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Ingredients::ConstIterator ConstIterator; //!< alias
            static const char * const          CallSign;      //!< "Chemical::Library"
            static const int                   PMIN = -12;    //!< Cmin = 10^PMIN
            static const int                   PMAX =   2;    //!< Cmax = 10^PMAX
            static real_t                      RanP( Random::Bits &ran ) noexcept; //!< ran(PMIN:PMAX)
            static xReal                       RanC( Random::Bits &ran ) noexcept; //!< 10^RanP(ran)

            //! C = RanC(ran) but below probaZero, and change sign below probaNegative
            static void                        Conc(XWritable &  C,
                                                    Random::Bits &ran,
                                                    const real_t probaZero     = 0,
                                                    const real_t probaNegative = 0) noexcept;



            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Library();           //!< setup empty
            virtual ~Library() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Library);     //!< display info

            //__________________________________________________________________
            //
            //
            // Methdods
            //
            //__________________________________________________________________

            //! get/create species from its formula
            const Species & get(const Formula &);

            //! get/create species from string,text, formula...
            template <typename DATA> inline
            const Species & operator()(const DATA &data)
            {
                const Formula formula(data);
                return get(formula);
            }

            template <typename T> inline
            T & Id(T &x) noexcept { return x; }

            template <typename ARRAY> inline
            std::ostream & operator()(std::ostream &os,
                                      const char * const pfx,
                                      ARRAY & arr,
                                      const char * const sfx) const
            {
                if(db.size()<=0) os << "{}";
                else
                {
                    os << '{' << std::endl;
                    for(ConstIterator it=db.begin();it!=db.end();++it)
                    {
                        const Species &sp = **it;
                        db.print(os, pfx, sp, sfx, Justify::Left);
                        os << " = ";
                        os << sp(arr,TopLevel);
                        os << std::endl;
                    }
                    os << '}';
                }
                return os;
            }

            static inline String ToReal(const xReal &x) {
                return Formatted::Get("%.15g", real_t(x) );
            }

            template <typename ARRAY, typename TRANSFORM> inline
            std::ostream & operator()(std::ostream &os,
                                      const char * const pfx,
                                      ARRAY & arr,
                                      const char * const sfx,
                                      TRANSFORM    & proc) const
            {
                if(db.size()<=0) os << "{}";
                else
                {
                    os << '{' << std::endl;
                    for(ConstIterator it=db.begin();it!=db.end();++it)
                    {
                        const Species &sp = **it;
                        db.print(os, pfx, sp, sfx, Justify::Left);
                        os << " = ";
                        os << proc(sp(arr,TopLevel));
                        os << std::endl;
                    }
                    os << '}';
                }
                return os;
            }





        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual ConstInterface & surrogate() const noexcept;
            Ingredients db;
        };
    }

}

#endif

