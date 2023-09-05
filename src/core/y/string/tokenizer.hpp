

//! \file

#ifndef Y_String_Tokenizer_Included
#define Y_String_Tokenizer_Included 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    namespace Core {
        

        //______________________________________________________________________
        //
        //
        //
        //! Tokenizer
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Tokenizer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Tokenizer(const T *, const size_t); //!< setup
            explicit Tokenizer(const T *);               //!< setup
            explicit Tokenizer(const String<T> &);       //!< setup
            virtual ~Tokenizer() noexcept;               //!< cleanup

            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________
            const T *    token() const noexcept; //!< current token location
            size_t       units() const noexcept; //!< current token size
            
            bool         next(const T * const sep, const size_t num) noexcept; //!< find next
            bool         next(const T *)                             noexcept; //!< find next
            bool         next(const T)                               noexcept; //!< find next
            bool         next(const String<T> &)                     noexcept; //!< find next

            //! split src and append tokens to seq
            template <typename SEQUENCE> static inline
            void AppendTo(SEQUENCE        &seq,
                          const String<T> &src,
                          const T * const  sep,
                          const size_t     num,
                          const int        imax=-1)
            {
                Tokenizer _(src);
                int       i=0;
                while( _.next(sep,num) ) {
                    ++i;
                    if(imax>=0 && i>imax) return;
                    const String<T> s(_.token(),_.units());
                    seq << s;
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tokenizer);
            class Code;
            Code *code;
        };
    }

    typedef Core::Tokenizer<char> Tokenizer; //!< alias


}

#endif

