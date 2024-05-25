
#include "y/apk/element/transmogrify.hpp"


namespace Yttrium
{

    namespace APK
    {
#define Y_APK_TRANS(TGT,SRC) (void*) Transmogrify<uint##TGT##_t,uint##SRC##_t>::To

        // TranProc<TARGET,SOURCE>
        void * TransmogrifyProc::Table[4][4] =
        {
            {                 0,  Y_APK_TRANS(8,16),  Y_APK_TRANS(8,32), Y_APK_TRANS(8,64)  },
            { Y_APK_TRANS(16,8),                  0, Y_APK_TRANS(16,32), Y_APK_TRANS(16,64) },
            { Y_APK_TRANS(32,8), Y_APK_TRANS(32,16),                  0, Y_APK_TRANS(32,64) },
            { Y_APK_TRANS(64,8), Y_APK_TRANS(64,16), Y_APK_TRANS(64,32),                  0 }
        };

    }

}
