
#include "y/lingo/pattern/dictionary.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Dictionary:: ~Dictionary() noexcept
        {
        }

        Dictionary:: Dictionary()
        {
        }
        


        static const char Name[] = "Lingo::Dictionary";

        void Dictionary:: operator()(const String &name, Pattern * const p)
        {
            const PatternPtr pp = p;
            if(!insert(name,p))
                throw Specific::Exception(Name, "multiple '%s", name.c_str());
        }

        void Dictionary:: operator()(const char * const name, Pattern * const p)
        {
            const String _(name);
            (*this)(_,p);
        }

        Pattern * Dictionary:: operator()(const String &name)
        {
            const PatternPtr * const pp = search(name);
            if(!pp)
                throw Specific::Exception(Name, "no '%s", name.c_str());
            return (**pp).clone();
        }

        Pattern * Dictionary:: operator()(const char * const name)
        {
            const String _(name);
            return (*this)(_);
        }



    }

}
