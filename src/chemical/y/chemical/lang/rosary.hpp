
//! \file

#ifndef Y_Chemical_Rosary_Included
#define Y_Chemical_Rosary_Included 1

#include "y/chemical/species/library.hpp"
#include "y/singleton.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Rapid Obnoxious Species And Reactions Yoga
        //
        //
        //______________________________________________________________________
        class Rosary : public Singleton<Rosary>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            static const char * const      CallSign;       //!< ChemicalRosary
            static const AtExit::Longevity LifeTime = 0;   //!< LifeTime
            class Compiler;                                // internal compiler

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! parse module, populate library and eqs
            void operator()(Jive::Module *m,
                            Library      &lib);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rosary);
            friend class Singleton<Rosary>;

            explicit Rosary();
            virtual ~Rosary() noexcept;
        };


    }

}

#endif
