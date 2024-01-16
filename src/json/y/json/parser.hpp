//! \file

#ifndef Y_JSON_Parser_Included
#define Y_JSON_Parser_Included 1

#include "y/singleton.hpp"

namespace Yttrium
{

    namespace JSON
    {
        class Parser : public Singleton<Parser>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 13;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            friend class Singleton<Parser>;
            
            explicit Parser();
            virtual ~Parser() noexcept;
        };
    }


}

#endif

