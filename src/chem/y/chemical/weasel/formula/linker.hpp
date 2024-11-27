

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

        //______________________________________________________________________
        //
        //
        //
        //! convert FORMULA node into string+charge
        //
        //
        //______________________________________________________________________
        class Formula::Linker : public Lingo::Syntax::Translator
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            //! formal charge
            enum Charge
            {
                PositiveCharge, //!< '+'
                NegativeCharge  //!< '-'
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Linker(const Weasel::Parser &); //!< setup using parser
            virtual ~Linker() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void     clear()  noexcept; //!< clear all resources

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Vector<String,MemoryModel>   elements; //!< stack of elements
            Vector<unsigned,MemoryModel> integers; //!< stack of integers
            Vector<Charge,MemoryModel>   zcharges; //!< stack of charges
            int                          z;        //!< computed charge


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

