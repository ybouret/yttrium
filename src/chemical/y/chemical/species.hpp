//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/solo.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Species = Entity + charge
        //
        //
        //______________________________________________________________________
        class Species : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            //typedef Small::BareLightList<const Species> BareList; //!< once usage
            //typedef Small::SoloLightList<const Species> SoloList; //!< standalone, multiple usage
            //typedef Small::CoopLightList<const Species> CoopList; //!< shared, multiple usage
            //typedef CoopList::ProxyType                 CoopRepo; //!< memory for CoopList

            typedef ArkPtr<String,Species>              Handle;   //!< alias
            typedef SuffixSet<String,Handle>            Set;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            template <typename NAME> inline
            explicit Species(const NAME  &uid,
                             const int    charge,
                             const size_t topLevel) :
            Entity(uid,topLevel), z(charge)
            {
            }

            //! cleanup
            virtual ~Species() noexcept;

            //! ddisplay
            friend std::ostream & operator<<(std::ostream &, const Species &);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void viz(OutputStream &) const; //!< write GraphViz code

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

        typedef Small::BareLightList<const Species> SList;
        typedef SList::NodeType                     SNode;

    }
}

#endif
