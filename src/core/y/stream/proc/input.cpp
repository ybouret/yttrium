

#include "y/stream/proc/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    ProcInput:: ~ProcInput() noexcept
    {
    }


    const char * const ProcInput:: How      = "r";
    const char * const ProcInput:: CallSign = "ProcInput";

    ProcInput:: ProcInput(const String &cmd) :
    ProcStream(cmd.c_str(), How)
    {
    }


    ProcInput:: ProcInput(const char * const cmd) :
    ProcStream(cmd,How)
    {
    }

    const char * ProcInput:: callSign() const noexcept { return CallSign; }

    static inline bool forceQuery(void * const handle,
                                  char &       data)
    {
        FILE *fp = (FILE *)handle;
        if(feof(fp)) return false;
        const size_t nr = fread(&data, 1, 1, fp);
        if(nr<=0) 
        {
            if(feof(fp)) return false;
            throw Specific::Exception(ProcInput::CallSign, "fread failure");
        }
        return true;
    }

    bool ProcInput:: query(char &data)
    {
        assert(0!=handle);

        if(buffer.size>0)
        {
            data = buffer.pullHead();
            return true;
        }
        else
        {
            return forceQuery(handle,data);
        }

    }

    void ProcInput:: store(const char C)
    {
        buffer >> C;
    }

    bool ProcInput:: ready()
    {
        if(buffer.size>0)
        {
            return true;
        }
        else
        {
            char data = 0;
            if(forceQuery(handle,data))
            {
                store(data);
                return true;
            }
            else
            {
                return false;
            }
        }
    }

}
