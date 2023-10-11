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

            typedef Functor<void,TL1(const Token&)> TerminalCallback;
            typedef Functor<void,TL1(size_t)>       InternalCallback;

            enum Translation
            {
                Permissive,
                Restricted
            };

            class Translator : public Analyzer
            {
            public:
                static const char * const CallSign;
                
                explicit Translator();
                virtual ~Translator() noexcept;

                template <typename ID, typename HOST, typename METH> inline
                void forTerminal(const ID &id, HOST &host, METH meth)
                {
                    assert(0!=meth);
                    const Tag              tag(id);
                    const TerminalCallback tcb(&host,meth);
                    submit(tag,tcb);
                }


                template <typename ID, typename HOST, typename METH> inline
                void forInternal(const ID &id, HOST &host, METH meth)
                {
                    assert(0!=meth);
                    const Tag              tag(id);
                    const InternalCallback icb(&host,meth);
                    submit(tag,icb);
                }

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

        }

    }

}

#endif
