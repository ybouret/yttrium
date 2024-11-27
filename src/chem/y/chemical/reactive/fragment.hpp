

#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! helper to display components assembly
        //
        //
        //______________________________________________________________________
        class Fragment : public Proxy<const Assembly>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fragment() noexcept; //!< setup
            virtual ~Fragment() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void update() noexcept = 0; //!< update all post-construction

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! update all assemblies
            void enroll(const Components &);

            //! print name:reac<=>prod:
            std::ostream & print(std::ostream &os, const Components &) const;



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

