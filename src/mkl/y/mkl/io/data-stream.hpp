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



        struct DataStream
        {

            class Column : public Object, public Counted
            {
            protected:
                explicit Column(const String &, const size_t);
            public:
                virtual ~Column() noexcept;

                const String & key() const noexcept;

                const String   name;
                const size_t   indx;
                void  *  const mine;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Column);
            };



            template <typename T>
            class ColumnOf : public Column
            {
            public:
                inline explicit ColumnOf(const String &uid,
                                         const size_t  num,
                                         Sequence<T>  &seq) :
                Column(uid,num), target(seq)
                {
                    Coerce(mine) = &target;
                }

                inline virtual ~ColumnOf() noexcept {}

                Sequence<T> & target;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ColumnOf);
            };


            class Columns
            {
            public:
                explicit Columns();
                virtual ~Columns() noexcept;

                size_t size() const throw();
                void   read(Column *);

                template <typename T> inline
                void read(const String &uid,
                          const size_t  num,
                          Sequence<T>  &seq)
                {
                    read( new ColumnOf<T>(uid,num,seq) );
                }

                template <typename T> inline
                void read(const char * uid,
                          const size_t num,
                          Sequence<T> &seq)
                {
                    const String _(uid);
                    read(_,num,seq);
                }

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

