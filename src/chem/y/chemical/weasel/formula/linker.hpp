

//! \file

#ifndef Y_Chemical_Formula_Linker_Included
#define Y_Chemical_Formula_Linker_Included 1

#include "y/chemical/formula.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/syntax/translator.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        class Formula::Linker : public Lingo::Syntax::Translator
        {
        public:
            enum Charge
            {
                PositiveCharge,
                NegativeCharge
            };

            explicit Linker(const Weasel::Parser &);
            virtual ~Linker() noexcept;
            
            void     clear()  noexcept;

            Vector<String,MemoryModel>   elements;
            Vector<unsigned,MemoryModel> integers;
            Vector<Charge,MemoryModel>   zcharges;
            int                          z;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
            virtual void init();
            virtual void quit();

            void onELEMENT(const Lingo::Lexeme &);
            void onINTEGER(const Lingo::Lexeme &);
            void onPLUS(const Lingo::Lexeme &);
            void onMINUS(const Lingo::Lexeme &);

            void onFORMULA(const size_t);
            void onSTOCHIO(const size_t);
            void onZ(const size_t);
            
        };
    }

}

#endif

