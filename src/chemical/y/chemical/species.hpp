//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/indexed.hpp"
#include "y/ptr/ark.hpp"
#include "y/random/bits.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Species: entity with charge
        //
        //
        //______________________________________________________________________
        class Species : public Indexed
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef ArkPtr<String,const Species> Ptr;                  //!< alias
            static const int                     PMIN=-15;             //!< Cmin = 10^PMIN
            static const int                     PMAX=1;               //!< Cmax = 10^PMAX
            static double                        Conc(Random::Bits &); //!< ran(Cmin,Cmax)
          
            //! fill array with random concentrations and zero with given proba
            template <typename ARRAY> static inline
            void Conc(ARRAY &arr, Random::Bits &ran,
                      const double probaZero = 0,
                      const double probaNeg  = 0) {
                for(size_t i=arr.size();i>0;--i)
                {
                    if(ran.to<double>()<probaZero)
                        arr[i] = 0;
                    else
                    {
                        arr[i] = Conc(ran);
                        if(ran.to<double>()<probaNeg) arr[i] = -arr[i];
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //! setup with gobal name, charge and top-level index
            template <typename ID> inline
            explicit Species(const ID    &userName,
                             const int    spCharge,
                             const size_t topLevel) :
            Indexed(userName,topLevel), z(spCharge)
            {
            }

            virtual ~Species() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Species);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            //! node+label+color+style
            void viz(OutputStream &fp,
                     const String &color,
                     const bool    conserved) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const int z; //!< algebraic charge

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };

        typedef Small::BareLightList<const Species> SList; //!< alias
        typedef SList::NodeType                     SNode; //!< alias
        typedef Small::CoopLightList<const Species> SRepo; //!< alias
        typedef SRepo::ProxyType                    SBank; //!< alias
    }
}

#endif

