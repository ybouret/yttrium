
//! \file

#ifndef Y_Chemical_Controller_Included
#define Y_Chemical_Controller_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/plexus/limits.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Landscape
        {
        public:
            Landscape(const BBank &bbank,
                      const SBank &sbank) noexcept :
            capping(bbank,sbank),
            missing(bbank,sbank)
            {
            }
            
            Landscape(const Landscape &other) : capping(other.capping), missing(other.missing)
            {
            }

            ~Landscape() noexcept {}

            friend std::ostream & operator<<(std::ostream &os, const Landscape &self)
            {
                os << "capping=" << self.capping << " / missing=" << self.missing;
                return os;
            }

            void reset() noexcept { capping.reset(); missing.reset(); }
            
            void shape(const Components &components,
                       const XReadable  &C,
                       const Level       level)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const xreal_t zero = 0;
                reset();

                //--------------------------------------------------------------
                //
                // scan reactants
                //
                //--------------------------------------------------------------
                for(const Actor *a=components.reac.head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    const xreal_t  nu = a->xnu;
                    const xreal_t  cc = C[sp.indx[level]];
                    if(cc>=zero)
                    {
                        capping.reac(sp,cc/nu);
                    }
                    else
                    {
                        missing.reac(sp,(-cc)/nu);
                    }
                }

                //--------------------------------------------------------------
                //
                // scan products
                //
                //--------------------------------------------------------------
                for(const Actor *a=components.prod.head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    const xreal_t  nu = a->xnu;
                    const xreal_t  cc = C[sp.indx[level]];
                    if(cc>=zero)
                    {
                        capping.prod(sp,cc/nu);
                    }
                    else
                    {
                        missing.prod(sp,(-cc)/nu);
                    }
                }
            }


            Limits capping;
            Limits missing;

        private:
            Y_DISABLE_ASSIGN(Landscape);
        };

        //______________________________________________________________________
        //
        //
        //
        //! equilibrium stripped from its unbounded components
        //
        //
        //______________________________________________________________________
        class Controller : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            typedef Components::ConstIterator ConstIterator; //!< alias

            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from equilibrium
            explicit Controller(const Equilibrium &eq,
                                const AddressBook &conserved);
            
            //! cleanup
            virtual ~Controller() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
           
            //! true if analogous components
            bool isEquivalentTo(const Controller &) const noexcept;
            


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Equilibrium &primary;    //!< persistent primary equilibrium
            const Components   components; //!< conserved components
            Controller        *next;       //!< for list
            Controller        *prev;       //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Controller);
        };

        typedef CxxListOf<Controller> Controllers; //!< list of controllers

    }

}

#endif

