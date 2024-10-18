//! \file

#ifndef Y_Lingo_Module_Included
#define Y_Lingo_Module_Included 1

#include "y/lingo/token.hpp"
#include "y/stream/input.hpp"
#include "y/ptr/arc.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //! shared input
        //
        //______________________________________________________________________
        typedef ArcPtr<InputStream> Input;

        //______________________________________________________________________
        //
        //
        //
        //! convert IO::Char into Jive::Char from context and input
        //
        //
        //______________________________________________________________________
        class Module : public Quantized, public Context
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
            Lingo::Char *get();

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
                const Context ctx(dataName,AsCaption);
                return OpenData_(ctx,addr,size);
            }

            //! open user's buffer
            template <typename DATANAME> static inline
            Module * OpenData(DATANAME &dataName, const Memory::ReadOnlyBuffer &buff)
            {
                const Context ctx(dataName,AsCaption);
                return OpenData_(ctx,buff.ro_addr(),buff.measure());
            }

            //! open text
            template <typename DATANAME> static inline
            Module *OpenData(DATANAME &dataName, const char *text)
            {
                const Context ctx(dataName,AsCaption);
                return OpenData_(ctx,text);
            }

            //! open line
            template <typename DATANAME> static inline
            Module *OpenData(DATANAME &dataName, const IO::Chars &line)
            {
                const Context ctx(dataName,AsCaption);
                return OpenData_(ctx,line);
            }

            //! open a file
            template <typename FILENAME> static inline
            Module * Open(FILENAME &fileName) { return OpenFile(fileName); }


            //! open data
            template <typename DATANAME, typename DATA> static inline
            Module * Open(DATANAME &dataName, DATA &data) { return OpenData(dataName,data); }



        private:
            Input input;
            Y_DISABLE_COPY_AND_ASSIGN(Module);
            static Module * OpenData_(const Context &, const void *addr, const size_t size);
            static Module * OpenData_(const Context &, const char *text);
            static Module * OpenData_(const Context &, const IO::Chars &line);

        };

    }
}

#endif

