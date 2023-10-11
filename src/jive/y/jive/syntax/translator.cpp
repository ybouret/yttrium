
#include "y/jive/syntax/translator.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            Translator:: ~Translator() noexcept
            {
            }

            Translator:: Translator()
            {
            }

            void Translator:: onTerminal(const String &name, const Token &data)
            {
                TerminalCallback *cb = tdb.search(name);
                if(cb)
                {
                    (*cb)(data);
                }
                else
                {
                    Analyzer::onTerminal(name,data);
                }
            }

            void Translator:: onInternal(const String &name, const size_t size)
            {
                InternalCallback *cb = idb.search(name);
                if(cb)
                {
                    (*cb)(size);
                }
                else
                {
                    Analyzer::onInternal(name,size);
                }
            }

            void Translator:: submit(const Tag &tag, const TerminalCallback &tcb)
            {
                if( !tdb.insert(*tag,tcb) )
                    throw Specific::Exception("Jive::Translator","multiple terminal callback for '%s'", tag->c_str());
            }


            void Translator:: submit(const Tag &tag, const InternalCallback &icb)
            {
                if( !idb.insert(*tag,icb) )
                    throw Specific::Exception("Jive::Translator","multiple internal callback for '%s'", tag->c_str());
            }

            void Translator:: initialize()
            {

            }

            void Translator:: translate(const XNode &root)
            {
                initialize();
                run(root);
            }


        }

    }

}

