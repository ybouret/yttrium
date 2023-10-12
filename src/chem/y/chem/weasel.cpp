
#include "y/chem/weasel.hpp"
#include "y/chem/lang/parser.hpp"
#include "y/chem/lang/linker.hpp"
#include "y/system/exception.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/container/algo/crop.hpp"
#include "y/jive/pattern/matcher.hpp"
#include "y/ordered/vector.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Weasel:: CallSign = "Weasel";

        static const char *BuiltInEqs[] =
        {
#include "lang/db.hxx"
        };


        static const unsigned BuiltInNum = sizeof(BuiltInEqs)/sizeof(BuiltInEqs[0]);


        class Weasel:: Code : public Parser
        {
        public:
            typedef Small::BareHeavyList<const String> NameList;
            typedef NameList::NodeType                 NameNode;
            typedef OrderedVector<unsigned>            ISet;

            inline explicit Code() :
            names(),
            parse(),
            xlink()
            {
                buildNames();
            }

            inline virtual ~Code() noexcept 
            {}

            inline void populate_(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
            {
                AutoPtr<XNode> tree = parse(m); assert(tree.isValid());
                xlink(*tree,lib,eqs);
            }

            inline void populate(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
            {
                // first pass
                populate_(m,lib,eqs);

                // second pass is remaining KXP
                OrderedVector<unsigned> iset;
                for(const Linker::StringNode *sn=xlink.RXP.head;sn;sn=sn->next)
                {
                    const String &rxp = **sn;
                    std::cerr << "-- using '" << rxp << "'" << std::endl;
                    Jive::Matcher match( rxp );
                    size_t        count = 0;
                    unsigned      i     = 0;
                    for(const NameNode *who=names.head;who;who=who->next,++i)
                    {
                        const String &eid = **who;
                        if( match.somehow(eid,eid) )
                        {
                            std::cerr << "matching <" << eid << ">" << std::endl;
                            ++count;
                            iset |= i;
                        }
                    }
                    if(count<=0) throw Specific::Exception(Weasel::CallSign,"no built-in equilibrium matching '%s'", rxp.c_str());
                }
                xlink.RXP.free();

                for(ISet::ConstIterator it=iset.begin();it!=iset.end();++it)
                {
                    const unsigned  i   = *it;
                    const char     *txt = BuiltInEqs[i];
                    const String   &eid  = **names.fetch(i+1);
                    std::cerr << " adding '" << txt << "'" << " / " << eid << std::endl;
                    populate_( Jive::Module::OpenData(eid,txt), lib, eqs);
                }

                eqs.updateMaxSizes();
            }

            const NameList names;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            Parser parse;
            Linker xlink;

            static inline bool IsBlank(const int ch) noexcept
            {
                return ' ' == ch || '\t' == ch;
            }

            inline void buildNames()
            {
                static const char fn[] = " built-in equilbrium ";

                for(unsigned i=0;i<BuiltInNum;++i)
                {
                    const char * const ini = BuiltInEqs[i];
                    const char * const sep = strchr(ini,':');
                    if(!sep) 
                        throw Specific::Exception(Weasel::CallSign,"corrupted%s#%u",fn,i);

                    if( 0 != strchr(ini,'@') )
                        throw Specific::Exception(Weasel::CallSign,"invalid%s#%u",fn,i);


                    String s(ini,sep-ini);
                    Algo::Crop(s,IsBlank);
                    
                    if(s.size()<=0)
                        throw Specific::Exception(Weasel::CallSign,"empty name for%s#%u",fn,i);

                    Coerce(names) << s;
                }
            }
        };

        static void *        Code__[ Y_WORDS_FOR(Weasel::Code) ];
        static Weasel::Code *Code_ = 0;

        Weasel:: ~Weasel() noexcept
        {
            assert(0!=Code_);
            Memory::OutOfReach::Naught(Code_);
        }

        Weasel:: Weasel()
        {
            try
            {
                Code_ = new ( Y_STATIC_ZARR(Code__) ) Code();
            }
            catch(...)
            {
                Code_ = 0;
                throw;
            }
            for(unsigned i=0;i<BuiltInNum;++i)
            {
                std::cerr << "-- '" << BuiltInEqs[i] << "'" << std::endl;
            }

        }

        void Weasel:: operator()(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
        {
            assert(0!=Code_);
            Code_->populate(m,lib,eqs);
        }

    
    }

}
