
#include "y/mkl/io/data-stream.hpp"
#include "y/type/nullify.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/ptr/ark.hpp"
#include "y/system/exception.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/text/ascii/convert.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace
        {
            template <typename T> static inline
            void Grow(void *addr, const String &msg, const char *ctx)
            {
                assert(0!=addr);
                Sequence<T> &seq = *static_cast< Sequence<T> * >(addr);
                seq.pushTail( ASCII::Convert::ToReal<T>(msg,ctx) );
            }
        }

        template <>
        void DataStream::ATOF_<float>(void *addr, const String &msg, const char *ctx)
        {
            Grow<float>(addr,msg,ctx);
        }

        template <>
        void DataStream::ATOF_<double>(void *addr, const String &msg, const char *ctx)
        {
            Grow<double>(addr,msg,ctx);
        }


        template <>
        void DataStream::ATOF_<long double>(void *addr, const String &msg, const char *ctx)
        {
            Grow<long double>(addr,msg,ctx);
        }


        DataStream:: Column:: Column(const String &uid,
                                     const size_t num) :
        Object(), Counted(),
        name(uid),
        indx(num),
        mine(0),
        proc(0)
        {
        }

        DataStream:: Column:: ~Column() noexcept { }

        const String & DataStream:: Column:: key() const noexcept
        {
            return name;
        }

        void  DataStream:: Column:: append(const String &msg)
        {
            assert(0!=mine);
            assert(0!=proc);
            proc(mine,msg,name.c_str());
        }

    }

    namespace MKL
    {


        class DataStream:: Columns:: Code : public Object
        {
        public:
            typedef ArkPtr<String,Column>    ColPtr;
            typedef SuffixSet<String,ColPtr> DataBase;

            // initialize
            inline explicit Code() : Object(), cdb() {}

            // cleanup
            inline virtual ~Code() noexcept {}

            // insert a new column
            inline void read(Column *col)
            {
                static const char fn[] = "DataStream::Columns";
                assert(0!=col);
                const ColPtr ptr(col);
                if(col->indx<=0)
                    throw Specific::Exception(fn,"column '%s' has zero index!", col->name.c_str() );

                for(DataBase::Iterator it=cdb.begin(); it != cdb.end(); ++it)
                {
                    const ColPtr &rhs = *it;
                    assert(0!=rhs->mine);
                    if(rhs->mine==ptr->mine)
                        throw Specific::Exception(fn,"same sequence in '%s' and '%s'", ptr->name.c_str(), rhs->name.c_str());
                }
                
                if(!cdb.insert(ptr))
                    throw Specific::Exception(fn,"multiple column '%s'", ptr->name.c_str());
            }


            void load(InputStream &input,
                      const size_t skip,
                      const size_t nmax)
            {
                static const char fn[] = "DataStream::Load";

                for(size_t i=1;i<=skip;++i)
                {
                    IO::Chars line;
                    if(!input.gets(line))
                        throw Specific::Exception(fn,"no line to skip %u/%u", unsigned(i), unsigned(skip));
                }

                size_t count = 0;
                {
                    Vector<String>           words;
                    String                   line;
                    const size_t             ncol = cdb.size();
                    const DataBase::Iterator init = cdb.begin();

                    while(input.gets(line))
                    {
                        if( line.size() <= 0 ) continue;
                        if( '#' == line[1] )   continue;

                        words.free();
                        Tokenizer::AppendTo(words,line, " \t", 2);
                        std::cerr << "words: " << words << std::endl;

                        const size_t       nw = words.size();
                        DataBase::Iterator it = init;
                        for(size_t i=ncol;i>0;--i,++it)
                        {
                            Column      &col = **it;
                            const size_t idx = col.indx;
                            assert(0!=col.mine);
                            assert(0!=col.proc);

                            if(idx<=0||idx>nw)
                                throw Specific::Exception(fn,"'%s' @%u not in #words=%u", col.name.c_str(), unsigned(idx), unsigned(nw) );
                            col.append(words[idx]);
                        }

                        ++count;
                        if(nmax>0 && count>=nmax) break;
                    }
                }

            }

            // members
            DataBase cdb;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        DataStream:: Columns:: Columns() : code( new Code() )
        {
        }

        size_t DataStream:: Columns:: size() const noexcept
        {
            assert(0!=code);
            return code->cdb.size();
        }


        DataStream:: Columns:: ~Columns() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        void   DataStream:: Columns:: read(Column *col)
        {
            assert( 0 != col);
            code->read(col);
        }

        void DataStream:: Columns:: load(InputStream &input,
                                         const size_t skip,
                                         const size_t nmax)
        {
            assert(code!=0);
            code->load(input,skip,nmax);
        }



    }

}
