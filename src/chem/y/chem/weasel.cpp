
#include "y/chem/weasel.hpp"
#include "y/chem/lang/parser.hpp"
#include "y/chem/lang/linker.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Weasel:: CallSign = "Weasel";
        



        class Weasel:: Code : public Parser
        {
        public:
            inline explicit Code() :
            parse(),
            link()
            {}

            inline virtual ~Code() noexcept 
            {}
            
            inline void populate(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
            {
                // first pass
                {
                    AutoPtr<XNode> tree = parse(m); assert(tree.isValid());
                    link(*tree,lib,eqs);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            Parser parse;
            Linker link;
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
            std::cerr << "sizeof(Code)   = " << sizeof(Code) << std::endl;
            std::cerr << "sizeof(Code__) = " << sizeof(Code__) << std::endl;
            try
            {
                Code_ = new ( Y_STATIC_ZARR(Code__) ) Code();
            }
            catch(...)
            {
                Code_ = 0;
                throw;
            }
        }

        void Weasel:: operator()(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
        {
            assert(0!=Code_);
            Code_->populate(m,lib,eqs);
        }

    
    }

}
