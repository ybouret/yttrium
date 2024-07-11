//! \file

#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Fragment of lists with database
        //
        //
        //______________________________________________________________________
        template <typename LIST>
        class Fragment : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fragment() : list(), book() {} //!< setup
            virtual ~Fragment() noexcept {}         //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert new compatible object
            template <typename T> inline
            Fragment & operator<<(const T &obj) {
                try {
                    Coerce(book) += obj;
                    Coerce(list) << obj;
                } catch(...) {
                    (void) Coerce(book).remove_( &obj );
                    throw;
                }
                return *this;
            }

            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const LIST        list; //!< content as list
            const AddressBook book; //!< content as book

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fragment);
        };

    }

}

#endif

