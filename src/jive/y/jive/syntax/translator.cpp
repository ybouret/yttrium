
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

            const char * const Translator:: CallSign = "Translator";

            void Translator:: onTerminal(const String &name, const Token &data)
            {
                TerminalCallback *cb = tdb.search(name);
                if(cb)
                {
                    (*cb)(data);
                }
                else
                {
                    switch(how)
                    {
                        case Permissive: Analyzer::onTerminal(name,data); break;
                        case Restricted:
                            throw Specific::Exception(CallSign,"no callback for terminal '%s'", name.c_str());
                    }
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

                    switch(how)
                    {
                        case Permissive: Analyzer::onInternal(name,size);; break;
                        case Restricted:
                            throw Specific::Exception(CallSign,"no callback for internal '%s'", name.c_str());
                    }
                }
            }

            void Translator:: submit(const Tag &tag, const TerminalCallback &tcb)
            {
                if( !tdb.insert(*tag,tcb) )
                    throw Specific::Exception("Jive::Translator","multiple callback for terminal '%s'", tag->c_str());
            }


            void Translator:: submit(const Tag &tag, const InternalCallback &icb)
            {
                if( !idb.insert(*tag,icb) )
                    throw Specific::Exception("Jive::Translator","multiple callback for terminal '%s'", tag->c_str());
            }

            void Translator:: initialize()
            {

            }

            void Translator:: translate(const XNode &root, const Translation flag)
            {
                how = flag;
                initialize();
                run(root);
            }


        }

    }

}

