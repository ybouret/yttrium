//! \file

#ifndef Y_Lingo_Lexical_Scanner_Included
#define Y_Lingo_Lexical_Scanner_Included 1

#include "y/lingo/lexical/rule.hpp"
#include "y/lingo/pattern/dictionary.hpp"
#include "y/lingo/lexical/action/error.hpp"

#include "y/type/proxy.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            //__________________________________________________________________
            //
            //
            // Definitions for Scanner
            //
            //__________________________________________________________________
            typedef Small::BareLightList<const Rule>  RList; //!< alias
            typedef RList::NodeType                   RNode; //!< alias
            typedef Memory::Wad<RList,Memory::Dyadic> RMaps; //!< alias
            typedef const Action_ *                   Report;

            //__________________________________________________________________
            //
            //
            //
            //! Scanner to produce Units from a set of Rules
            //
            //
            //__________________________________________________________________
            class Scanner :
            public Quantized,
            public Entity,
            public RMaps,
            public Proxy<const Rules>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const unsigned                     CHARS = 256; //!< alias
                typedef ArkPtr<String,Scanner>            Pointer;     //!< alias
                

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from name and shared dictionary
                template <typename CAPTION>
                explicit Scanner(const CAPTION    &_name,
                                 const Dictionary &_dict) noexcept :
                Entity(_name,AsCaption),
                RMaps(CHARS),
                rules(),
                rlist(lead()),
                error(),
                dict(_dict)
                {
                    initialize();
                }

                //! cleanup
                virtual ~Scanner() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! compile any expression with internal dictionary
                template <typename RX> inline
                Pattern * compile(const RX &rx) {
                    return dict.compile(rx);
                }

                //! record a new rule
                void operator()(Rule * const rule);


                Unit * run(Source &source,
                           Report &report) const;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules         rules; //!< list of rules
                RList * const rlist; //!< map of rules from their first chars

                void                     initialize() noexcept;       //!< setup map
                virtual ConstInterface & surrogate() const noexcept;  //!< [Proxy] rules

                Unit * findError(Source &source,
                                 Report &report) const;

            public:
                //______________________________________________________________
                //
                //
                //  Members
                //
                //______________________________________________________________
                const Error error; //!< report error
                Dictionary  dict;  //!< shared dictionary
            };

        }

    }

}

#endif
