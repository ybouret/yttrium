#include "y/config/starting.h"

const char Y_Unknown[] = "unknown";
const char Y_Success[] = "success";
const char Y_Failure[] = "failure";
const char Y_Warning[] = "warning";
const char Y_Message[] = "message";

int Y_Die(const char *msg)
{
    (void)msg;
    return 1;
}

int Y_Good(const void *addr, const size_t size)
{
    if(NULL!=addr)
    {
        return 1;
    }
    else
    {
        assert(NULL==addr);
        return size<=0 ? 1 : 0;
    }

}
