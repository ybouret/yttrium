//! \file

#ifndef Y_MKL_Data_Stream_Included
#define Y_MKL_Data_Stream_Included 1

#include "y/sequence/interface.hpp"
#include "y/stream/input.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //______________________________________________________________________
        //
        //
        //
        //! helper to load from text stream
        //
        //
        //______________________________________________________________________
        struct DataStream
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef void (*ATOF)(void *, const String &, const char   *); //!< alias

            //! conversion
            template <typename T>
            static void ATOF_(void *, const String &, const char *);

            //__________________________________________________________________
            //
            //
            //! named column
            //
            //__________________________________________________________________
            class Column : public Object, public Counted
            {
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
            protected:
                //! setup with name and index
                explicit Column(const String &, const size_t);
            public:
                //! cleanup
                virtual ~Column() noexcept;

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________
                const String & key()  const noexcept;     //!< for associative set
                void           append(const String &msg); //!< proc(mine,msg,name())

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const String   name; //!< uuid
                const size_t   indx; //!< column index
                void  *  const mine; //!< sequence address
                ATOF     const proc; //!< data to sequence

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Column);
            };


            //__________________________________________________________________
            //
            //
            //! wrapping a sequence within a column
            //
            //__________________________________________________________________
            template <typename T>
            class ColumnOf : public Column
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup persistent sequence
                inline explicit ColumnOf(const String &uid,
                                         const size_t  num,
                                         Sequence<T>  &seq) :
                Column(uid,num), target(seq)
                {
                    Coerce(mine) = &target;
                    Coerce(proc) = ATOF_<T>;
                }

                //! cleanup
                inline virtual ~ColumnOf() noexcept {}

                //______________________________________________________________
                //
                // member
                //______________________________________________________________
                Sequence<T> & target; //!< persistent target

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ColumnOf);
            };


            //__________________________________________________________________
            //
            //
            //! Database of columns
            //
            //__________________________________________________________________
            class Columns
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Columns();          //!< setup
                virtual ~Columns() noexcept; //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                size_t size() const throw(); //!< registered columns
                void   read(Column *);       //!< insert a newly created colum

                //! create and insert a column
                template <typename T> inline
                void read(const String &uid,
                          const size_t  num,
                          Sequence<T>  &seq)
                {
                    read( new ColumnOf<T>(uid,num,seq) );
                }

                //! create and insert a column
                template <typename T> inline
                void read(const char * uid,
                          const size_t num,
                          Sequence<T> &seq)
                {
                    const String _(uid);
                    read(_,num,seq);
                }


                //! append input content to registered columns
                void load(InputStream &input,
                          const size_t skip=0,
                          const size_t nmax=0);



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Columns);
                class Code;
                Code *code;
            };
        };
    }
    
}

#endif

