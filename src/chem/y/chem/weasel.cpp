
#include "y/chem/weasel.hpp"
#include "y/chem/lang/parser.hpp"
#include "y/chem/lang/linker.hpp"
#include "y/system/exception.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include <cstring>
#include "y/container/algo/crop.hpp"

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
            inline explicit Code() :
            names(),
            parse(),
            xlink()
            {
                buildNames();
            }

            inline virtual ~Code() noexcept 
            {}
            
            inline void populate(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
            {
                // first pass
                {
                    AutoPtr<XNode> tree = parse(m); assert(tree.isValid());
                    xlink(*tree,lib,eqs);
                }
            }

            const Small::BareHeavyList<const String> names;

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
                for(unsigned i=0;i<BuiltInNum;++i)
                {
                    const char * const ini = BuiltInEqs[i];
                    const char * const sep = strchr(ini,':');
                    if(!sep) 
                        throw Specific::Exception(Weasel::CallSign,"corrupted built-in equilibrium #%u",i);

                    String s(ini,sep-ini);
                    Algo::Crop(s,IsBlank);
                    
                    if(s.size()<=0)
                        throw Specific::Exception(Weasel::CallSign,"empty name for equilibrium #%u",i);

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
