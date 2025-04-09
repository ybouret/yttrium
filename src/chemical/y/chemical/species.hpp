//! \file


#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/indexed.hpp"
#include "y/chemical/formula.hpp"
#include "y/chemical/type/charge.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Species description
        //
        //
        //______________________________________________________________________
        class Species :
        public Quantized,
        public Formula,
        public Charge,
        public Indexed,
        public Counted,
        public Serializable,
        public GraphViz:: Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<const String,const Species> Pointer; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! NAME = text, String, Formula
            template <typename NAME> inline
            explicit Species(const NAME  &description,
                             const size_t i) :
            Formula(description),
            Charge(),
            Indexed( toText(Coerce(z)), i  ),
            html(    toHTML() )
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
            virtual size_t   serialize(OutputStream &fp) const;          //!< univocal formula
            static Species * ReadFrom(InputStream &fp, const size_t id); //!< retrieve from serialized

            //! emit GraphViz code
            void   viz(OutputStream &       fp,
                       const String * const color     = 0,
                       const bool           conserved = true) const;

            static SignType Compare(const Species &, const Species &) noexcept;
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const XName html; //!< HTML label

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

