

#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Fragment : public Proxy<const Assembly>
        {
        public:
            explicit Fragment() noexcept;
            virtual ~Fragment() noexcept;

            void enroll(const Components &);

            //! print name:reac<=>prod:
            std::ostream & print(std::ostream &os, const Components &) const;

            virtual void updateFragment() noexcept = 0;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fragment);
            virtual ConstInterface & surrogate() const noexcept;
            Assembly self; //!< for names
            Assembly reac; //!< for actors
            Assembly prod; //!< for actors
        };

    }

}

#endif

