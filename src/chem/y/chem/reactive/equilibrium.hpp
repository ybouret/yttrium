
//! \file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chem/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! an equilibrium is a named set of components
        //
        //
        //______________________________________________________________________
        class Equilibrium : public Entity, public Components
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<String,Equilibrium> Pointer;  //!< alias
            typedef SuffixSet<String,Pointer>  DataBase; //!< alias
            static const char * const          SchemeId; //!< default GraphViz scheme

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Equilibrium() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Equilibrium);    //!< "<name>"
     
        protected:
            //! setup with identifier
            template <typename ID> inline
            explicit Equilibrium(const ID &id) : Entity(id), Components() {}

        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            xreal K(double t);                      //!< check the return value of internal getK(t)
            void  viz(OutputStream &)        const; //!< name + links
            void  graphViz(OutputStream &fp) const; //!< all species + this


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xreal getK(double) = 0;
            void    vizColor(OutputStream &fp) const;
            void    vizArrow(OutputStream &fp, const Vizible *v, const bool to, const unsigned nu) const;
        };

        //______________________________________________________________________
        //
        //
        // List of equilibria
        //
        //______________________________________________________________________
        typedef Small::BareLightList<const Equilibrium> EqRepo; //!< alias
        typedef EqRepo::NodeType                        EqNode; //!< alias



        //______________________________________________________________________
        //
        //
        //
        //! equilibrium with const constant
        //
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:

            //! setup
            template <typename ID> inline
            explicit ConstEquilibrium(const ID &id, const xreal k) :
            Equilibrium(id), K_(k)
            {
            }

            //! cleanup
            virtual ~ConstEquilibrium() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const xreal K_;
            virtual xreal getK(double);
        };
    }

}

#endif
