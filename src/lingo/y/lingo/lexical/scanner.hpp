//! \file

#ifndef Y_Lingo_Lexical_Scanner_Included
#define Y_Lingo_Lexical_Scanner_Included 1

#include "y/lingo/lexical/rule.hpp"
#include "y/lingo/pattern/dictionary.hpp"

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
                void add(Rule * const rule);

                //! generic rule creation from regular expression
                template <
                typename ID,
                typename RX,
                typename HOST,
                typename METH> inline
                void operator()(const ID & id,
                                const RX & rx,
                                HOST     & host,
                                METH       meth)
                {
                    add( Rule::Create(id, compile(rx), host, meth) );
                }

                Unit * run(Source &source) const;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules         rules; //!< list of rules
                RList * const rlist; //!< map of rules from their first chars

                void                     initialize() noexcept;       //!< setup map
                virtual ConstInterface & surrogate() const noexcept;  //!< [Proxy] rules

                Unit * findError(Source &source) const;

            public:
                //______________________________________________________________
                //
                //
                //  Members
                //
                //______________________________________________________________
                Dictionary  dict;  //!< shared dictionary
            };

        }

    }

}

#endif
