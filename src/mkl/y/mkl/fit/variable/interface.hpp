
//! \file

#ifndef Y_Fit_Variable_Interface_Included
#define Y_Fit_Variable_Interface_Included 1

#include "y/string.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/hash/set.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            //__________________________________________________________________
            //
            //
            //
            //! a Variable is a name with and index
            //
            //
            //__________________________________________________________________
            class Variable : public String
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef ArkPtr<String,Variable> Handle;   //!< alias
                typedef HashSet<String,Handle>  DB;       //!< alias
                typedef DB::ConstIterator       Iterator; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                explicit Variable(const String &); //!< setup
                explicit Variable(const char   *); //!< setup
                explicit Variable(const char    ); //!< setup
            public:
                virtual ~Variable() noexcept;      //!< cleanup
                Y_OSTREAM_PROTO(Variable);         //!< display with dsp
                
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const String & key() const noexcept;      //!< for set
                virtual size_t idx() const noexcept = 0;  //!< for access

            private:
                virtual void   dsp(std::ostream &) const = 0; //!< to dispplay
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
            };

        }

    }

}


#endif

