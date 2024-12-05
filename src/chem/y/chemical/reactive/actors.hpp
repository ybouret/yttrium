
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
            explicit Actors(const bool bareName) noexcept; //!< setup
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

            void  activity(XMul &xmul, const XReadable &C, const Level L)                 const; //!< gather product of activitis into mul
            void  activity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const; //!< gather product of shifted activities into mul
            bool  haveZero(const XReadable &C, const Level L)                    const noexcept; //!< check if at least one concentration is zero
            xReal limitingExtent(const XReadable &C, const Level L)              const noexcept; //!< fast >=0 limiting extent among actors
            void  moveSafely(XWritable &C, const Level L, const xReal xi)        const noexcept; //!< move all actors safely

        private:
            Y_DISABLE_ASSIGN(Actors);
            virtual ConstInterface & surrogate() const noexcept;
            friend class Components;
            Company company;
        };


    }

}

#endif
