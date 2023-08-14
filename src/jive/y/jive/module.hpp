//! \file

#ifndef Y_Jive_Module_Included
#define Y_Jive_Module_Included 1

#include "y/jive/token.hpp"
#include "y/stream/input.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        typedef ArcPtr<InputStream> Input; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! convert IO::Char into Jive::Char
        //
        //
        //______________________________________________________________________
        class Module : public Object, public Context
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit Module(const Context &ctx,
                            const Input   &inp) noexcept;
            
            //! cleanup
            virtual ~Module() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get new char with automatic newChar()
            Jive::Char *get();

            //__________________________________________________________________
            //
            //
            // helpers to create Modules
            //
            //__________________________________________________________________
            static Module * OpenStdIn();                       //!< read from StdIn
            static Module * OpenFile(const char   *fileName);  //!< open file
            static Module * OpenFile(const String &fileName);  //!< open file

            //! open user's data
            template <typename DATANAME>
            static Module * OpenData(DATANAME &dataName, const void *addr, const size_t size)
            {
                const Context ctx = new String(dataName);
                return OpenData_(ctx,addr,size);
            }

            //! open user's buffer
            template <typename DATANAME>
            static Module * OpenData(DATANAME &dataName, const Memory::ReadOnlyBuffer &buff)
            {
                const Context ctx = new String(dataName);
                return OpenData_(ctx,buff.ro_addr(),buff.measure());
            }

        private:
            Input input;
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            static Module * OpenData_(const Context &, const void *addr, const size_t size);
            
        };

    }
}

#endif

