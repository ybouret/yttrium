#include "y/lingo/pattern/dictionary.hpp"
#include "y/system/exception.hpp"
#include "y/type/nullify.hpp"

#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Dictionary:: Code : public Object, public Counted
        {
        public:
            typedef Small::BareHeavyList<const String> Labels;
            typedef Labels::NodeType                   LNode;

            inline explicit Code() noexcept :
            Object(),
            Counted(),
            labels(), motifs()
            {
            }

            inline virtual ~Code() noexcept
            {
            }

            Pattern * query(const String &name) const noexcept
            {
                Pattern *motif = motifs.head;
                for(const LNode *label=labels.head;label;label=label->next,motif=motif->next)
                {
                    assert(label);
                    assert(motif);
                    if( name == **label ) return motif;
                }
                return 0;
            }

            Labels    labels;
            Patterns  motifs;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        Dictionary:: ~Dictionary() noexcept
        {
            assert(0!=code);
            if(code->liberate())
                Nullify(code);
        }

        Dictionary:: Dictionary() : code( new Code() )
        {
            code->withhold();
        }

        Dictionary:: Dictionary(const Dictionary &dict) noexcept :
        code( dict.code )
        {
            code->withhold();
        }

        std::ostream & operator<<(std::ostream &os, const Dictionary &dict)
        {
            assert(0!=dict.code);
            os << '[';
            for(const Dictionary::Code::LNode *label = dict.code->labels.head;label;label=label->next)
            {
                os << " '" << **label << "'";
            }
            os << ' ' << ']';
            return os;
        }

        static const char Name[] = "Lingo::Dictionary";

        void Dictionary:: define(const String &name, Pattern * const p)
        {
            assert(0!=code);
            assert(code->motifs.size==code->labels.size);
            code->motifs.pushTail(p);
            try
            {
                if( code->query(name) ) throw Specific::Exception(Name,"multiple '%s'", name.c_str());
                code->labels << name;
                assert(code->motifs.size==code->labels.size);
            }
            catch(...)
            {
                delete code->motifs.popTail();
                assert(code->motifs.size==code->labels.size);
                throw;
            }
        }

        void Dictionary:: define(const char * const name, Pattern * const p)
        {
            const String _(name);
            define(_,p);
        }

        Pattern * Dictionary:: operator()(const String &name) const
        {
            const Pattern * const p = code->query(name);
            if(!p) throw Specific::Exception(Name, "no '%s'", name.c_str());
            return p->clone();
        }

        Pattern * Dictionary:: operator()(const char * const name) const
        {
            const String _(name);
            return (*this)(_);
        }



    }

}
