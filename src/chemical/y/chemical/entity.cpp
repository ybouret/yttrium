
#include "y/chemical/entity.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        const String & Entity:: key() const noexcept
        {
            return name;
        }

        
        Entity:: ~Entity() noexcept
        {
        }
        
        String Entity:: fileName() const
        {
            String ans;
            for(size_t i=1;i<=name.size();++i)
            {
                char c = name[i];
                if(isalnum(c))
                {
                    ans +=c;
                    continue;
                }

               
                switch(c)
                {
                    case '+':
                    case '-':
                        break;

                    default:
                        c = '_';
                }

                ans += c;
            }
            return ans;
        }


    }

}



