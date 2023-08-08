//! \file

#ifndef Y_Jive_Module_Included
#define Y_Jive_Module_Included 1

#include "y/jive/token.hpp"
#include "y/io/stream/input.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        typedef ArcPtr<InputStream> Input;

        //! convert IO::Char into Jive::Char
        class Module : public Object, public Context
        {
        public:
            explicit Module(const Context &ctx,
                            const Input   &inp) noexcept;
            virtual ~Module() noexcept;

            //! get new char with automatic newChar()
            Jive::Char *get();

            static Module * OpenStdIn();
            static Module * OpenFile(const char   *fileName);
            static Module * OpenFile(const String &fileName);

        private:
            Input input;
            Y_DISABLE_COPY_AND_ASSIGN(Module);
        };

    }
}

#endif

