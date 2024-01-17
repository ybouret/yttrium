//! \file

#ifndef Y_Jive_Syntax_Translator_Included
#define Y_Jive_Syntax_Translator_Included 1

#include "y/jive/syntax/analyzer.hpp"
#include "y/associative/suffix/map.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {


            typedef Functor<void,TL1(const Token&)> TerminalCallback; //!< alias
            typedef Functor<void,TL1(size_t)>       InternalCallback; //!< alias

            //__________________________________________________________________
            //
            //
            //! Control translation state
            //
            //__________________________________________________________________
            enum Translation
            {
                Permissive, //!< call analyzer if no matching callback
                Restricted  //!< raise an exception if no matching callback
            };

            //__________________________________________________________________
            //
            //
            //
            //! Tanslator by table of callbacks
            //
            //
            //__________________________________________________________________
            class Translator : public Analyzer
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Translator""

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Translator();           //!< setup
                virtual ~Translator() noexcept;  //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! define a callback for a named Terminal
                template <typename ID, typename HOST, typename METH> inline
                void forTerminal(const ID &id, HOST &host, METH meth)
                {
                    assert(0!=meth);
                    const Tag              tag(id);
                    const TerminalCallback tcb(&host,meth);
                    submit(tag,tcb);
                }

                //! define a callaback for a named Internal
                template <typename ID, typename HOST, typename METH> inline
                void forInternal(const ID &id, HOST &host, METH meth)
                {
                    assert(0!=meth);
                    const Tag              tag(id);
                    const InternalCallback icb(&host,meth);
                    submit(tag,icb);
                }

                //! analyze/translate the root AST
                void translate(const XNode &root, const Translation flag);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Translator);
                SuffixMap<String,TerminalCallback> tdb;
                SuffixMap<String,InternalCallback> idb;
                Translation                        how;

                void         submit(const Tag &tag, const TerminalCallback &tcb);
                void         submit(const Tag &tag, const InternalCallback &icb);

                virtual void initialize();
                virtual void onTerminal(const String &, const Token  &);
                virtual void onInternal(const String &, const size_t  );
                
            };

#define Y_Jive_OnTerminal(CLASS,NAME) forTerminal( #NAME, *this, & CLASS:: on##NAME ) //!< helper
#define Y_Jive_OnInternal(CLASS,NAME) forInternal( #NAME, *this, & CLASS:: on##NAME ) //!< helper


        }

    }

}

#endif
