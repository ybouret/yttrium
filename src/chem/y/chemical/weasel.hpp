
//! \file

#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/library.hpp"
#include "y/lingo/module.hpp"
#include "y/singleton.hpp"
#include "y/lua++/state.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Weasel is Equilibria And Species Elementary Language
        //
        //
        //______________________________________________________________________
        class Weasel : public Singleton<Weasel>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                 //!< "Weasel"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 30; //!< alias
            class Parser;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! parser/linked
            void operator()(Lingo::Module * const);



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lua::VM              luaVM;   //!< shared VM
            const Lingo::Caption caption; //!< shared caption

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Weasel);
            friend class Singleton<Weasel>;
            virtual ~Weasel() noexcept;
            explicit Weasel();
        };
    }

}

#endif
