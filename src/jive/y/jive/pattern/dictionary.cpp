
#include "y/jive/pattern/dictionary.hpp"
#include "y/associative/suffix/map.hpp"
#include "y/ptr/arc.hpp"
#include "y/jive/pattern.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {

        typedef SuffixMap<String,Motif> Dict;

        class Dictionary:: Code : public Object, public Dict
        {
        public:
            static const char * const CallSign;

            explicit Code() : Object(), Dict() {}
            virtual ~Code() noexcept           {}


            void mustInsert(const String &label, const Motif &motif)
            {
                if(!insert(label,motif))
                    throw Specific::Exception(CallSign,"multiple pattern '%s'", label() );
            }

            std::ostream & show(std::ostream &os) const
            {
                os << '{';
                if(size())
                {
                    Dict::ConstIterator it=begin();
                    os << it->key;
                    for(++it;it!=end();++it)
                    {
                        os << ',' << it->key;
                    }
                }

                return os << '}';
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        const char * const Dictionary::Code::CallSign = "Jive::Dictionary";

        Dictionary:: Dictionary() :
        code( new Code() )
        {
        }

        Dictionary:: ~Dictionary() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        void Dictionary:: operator()(const char    *name,
                                     const Pattern *p)
        {
            assert(0!=p);
            const Motif  motif(p);
            const String label(name);
            code->mustInsert(label,motif);
        }

        void Dictionary:: operator()(const String & label, const Pattern *p)
        {
            const Motif  motif(p);
            code->mustInsert(label,motif);
        }

        std::ostream & operator<<(std::ostream &os, const Dictionary &dict)
        {
            return dict.code->show(os);
        }

        Pattern * Dictionary:: create(const String &name)
        {
            assert(0!=code);
            const Motif *pp =code->search(name);
            if(!pp) return 0;
            return (*pp)->clone();
        }

        Pattern * Dictionary:: create(const char *name)
        {
            const String  _(name);
            return create(_);
        }


    }

}

