
#ifndef Y_Net_Socket_Protocol_Family_Included
#define Y_Net_Socket_Protocol_Family_Included 1


#include "y/net/type/parameter.hpp"


namespace Yttrium
{
    namespace Network
    {

        typedef Parameter<int> ProtocolFamilyClass;

        class ProtocolFamily : public ProtocolFamilyClass
        {
        public:
            typedef SuffixSet<String,const ProtocolFamily> Set;

            template <typename NAME> inline
            explicit ProtocolFamily(const int protocolFamily, const NAME &uuid) :
            ProtocolFamilyClass(protocolFamily,uuid)
            {
            }

            explicit ProtocolFamily(const ProtocolFamily &) noexcept;
            virtual ~ProtocolFamily() noexcept;


        private:
            Y_DISABLE_ASSIGN(ProtocolFamily);
        };

    }
}

#endif

