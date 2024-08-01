
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of Actors
        //
        //
        //______________________________________________________________________
        class Actors : public Entity, public Proxy<const Actor::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors();           //!< setup with top-level index
            virtual ~Actors() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Actors);     //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! record all species
            void record(AddressBook &) const;

            //! algebraic charge
            int charge() const noexcept;

            //! add next actor
            void operator()(const unsigned nu, const Species &sp);

            //! check species
            bool hired(const Species &sp) const noexcept;

            //! check same species, even with different coefficient
            bool akin(const Actors &team) const noexcept;

            //! compute mass action
            void massAction(XMul &xmul, const XReadable &C, const Level L) const;

            //! compute mass action of Cinp+nu*xi
            void massAction(XMul            &xmul,
                            const XReadable &Cinp,
                            const Level      Linp,
                            const xreal_t    xi) const;

            //! derivative of mass action
            void drvsMassAction(XWritable       & drvs,
                                const Level       Lout,
                                const xreal_t     xfac,
                                XMul            & xmul,
                                const XReadable & Cinp,
                                const Level       Linp) const;

            //! derivative of mass action at Cinp+nu*xi
            void drvsMassAction(XWritable       & drvs,
                                XWritable       & Cout,
                                const Level       Lout,
                                const xreal_t     xfac,
                                XMul            & xmul,
                                const XReadable & Cinp,
                                const Level       Linp,
                                const xreal_t     xi) const;


            //! min of maxExtents
            xreal_t maxExtent(const XReadable &C, const Level level) const;

            //! move safely each actor
            void moveSafe(XWritable &C, const Level L, const xreal_t xi) const;

            //! Cout = (Cinp + xi * nu) >= 0
            void addSafe(XWritable       &Cout,
                         const Level      Lout,
                         const XReadable &Cinp,
                         const Level     &Linp,
                         const xreal_t    xi) const;



            //! check that each concentration is positive
            bool accounted(const XReadable &C0, const Level level) const;
           
            //! check that at least one concentration is missing
            bool deficient(const XReadable &C0, const Level level) const;

            //! display compact array
            template <typename ARRAY> inline
            std::ostream & displayCompact(std::ostream &os, ARRAY &arr, const Level level) const
            {
                os << '{';
                const Actor *a=actors.head;
                if(a)
                {
                    Show(os,arr,*a,level);
                    for(a=a->next;a;a=a->next)
                    {
                        Show(os << ';',arr,*a,level);
                    }
                }
                os << '}';
                return os;
            }
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            const unsigned sumNu; //!< sum(nu)
            const real_t   scale; //!< 1.0/sumNu
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            Actor::List actors;
            virtual ConstInterface & surrogate() const noexcept;
            void growName(const String &);
            template <typename ARRAY> static inline
            void Show(std::ostream &os, const ARRAY &arr, const Actor &a, const Level level)
            {
                os << '[' << a.sp.name << ']' << '=' << real_t(arr[a.sp.indx[level]]);
            }
        };

    }

}

#endif

