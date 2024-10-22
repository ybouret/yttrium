#include "y/lingo/pattern/dictionary.hpp"
#include "y/system/exception.hpp"
#include "y/type/nullify.hpp"

#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Dictionary:: Code : public Object
        {
        public:
            typedef Small::BareHeavyList<const String> Labels;
            typedef Labels::NodeType                   LNode;

            inline explicit Code() noexcept : labels(), motifs()
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
            Nullify(code);
        }

        Dictionary:: Dictionary() : code( new Code() )
        {
        }
        


        static const char Name[] = "Lingo::Dictionary";

        void Dictionary:: operator()(const String &name, Pattern * const p)
        {
            assert(0!=code);
            code->motifs.pushTail(p);
            try
            {
                if( code->query(name) ) throw Specific::Exception(Name,"multiple '%s'", name.c_str());
                code->labels << name;
            }
            catch(...)
            {
                delete code->motifs.popTail();
                throw;
            }
        }

        void Dictionary:: operator()(const char * const name, Pattern * const p)
        {
            const String _(name);
            (*this)(_,p);
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
