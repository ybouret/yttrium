
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/company.hpp"
 
namespace Yttrium
{
    namespace Chemical
    {

        

        //______________________________________________________________________
        //
        //
        //
        //! Actors = Proxy to const Company
        //
        //
        //______________________________________________________________________
        class Actors : public Proxy<const Company>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors() noexcept; //!< setup
            virtual ~Actors() noexcept; //!< cleanup
            Actors(const Actors &);     //!< duplicate
            Y_OSTREAM_PROTO(Actors);    //!< display name

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & operator()(const Species &);                 //!< register with coeff=1
            const Actor & operator()(const unsigned, const Species &); //!< register


        private:
            Y_DISABLE_ASSIGN(Actors);
            virtual ConstInterface & surrogate() const noexcept;
            friend class Components;
            Company company;
        };


    }

}

#endif
