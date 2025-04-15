

//! \file


#ifndef Y_Chemical_DesignCompiler_Included
#define Y_Chemical_DesignCompiler_Included 1

#include "y/chemical/plexus/initial/design.hpp"

//#include "y/lingo/syntax/translator.hpp"
//#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {


            class Compiler
            {
            public:
                explicit Compiler();
                virtual ~Compiler() noexcept;

                Design * operator()(const XNode   &root,
                                    const Library &lib);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compiler);
                void onInstr(const String &instr, Design &design, Library &eq);
            };

        }

    }

}

#endif
