
#include "y/mpi++/api.hpp"
#include "y/system/exception.hpp"
#include <cstdarg>
#include <cstdio>

namespace Yttrium
{

    static const char fn[] = "MPI::print";


    static inline void OutputStringTo(FILE *fp, const String &s)
    {
        fwrite(s.ro_addr(), 1, s.measure(), fp);
    }



    void MPI:: print(FILE *fp, const char *fmt, ...)
    {
        if(primary && 0==fp)
        {
            throw MPI::Exception(MPI_ERR_BAD_FILE,"NULL File to print to");
        }
        //______________________________________________________________________
        //
        //
        // make string
        //
        //______________________________________________________________________
        assert(fmt!=NULL);
        int res = 0;
        {
            va_list ap;
            va_start(ap,fmt);
            res = Formatted::ComputeSize(fmt,&ap);
            va_end(ap);
            if(res<0) throw Libc::Exception(errno,"%s(...)",fn);
        }

        if(res<=0)
        {
            if(primary)
            {
                // do nothing
            }
            else
            {
                const String mine;
                SendOne<String>::With(*this,mine,0,Tag);
            }
        }
        else
        {
            String  mine(res,AsCapacity,true); assert( mine.size() == size_t(res) );
            va_list ap;
            va_start(ap,fmt);
            const int chk = Formatted::WriteString(mine,fmt,&ap);
            va_end(ap);
            if(res!=chk) throw  Specific::Exception(fn,"lengths mismatch!");
            if(primary)
            {
                OutputStringTo(fp,mine);
                for(size_t r=1;r<size;++r)
                {
                    const String temp = RecvOne<String>::With(*this,r,Tag);
                    OutputStringTo(fp,temp);
                }
                fflush(fp);
            }
            else
            {
                SendOne<String>::With(*this,mine,0,Tag);
            }
        }


    }
}

