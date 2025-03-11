
//! \file


#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1


#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! database of components
        //
        //
        //______________________________________________________________________
        typedef SuffixSet<const String,const Component> ComponentsDB;

        //______________________________________________________________________
        //
        //
        //
        //! Latchable Database of components
        //
        //
        //______________________________________________________________________
        class ComponentsType : public ComponentsDB, public Latchable
        {
        public:
            explicit ComponentsType();           //!< setup
            virtual ~ComponentsType() noexcept;  //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ComponentsType);
        };


        //______________________________________________________________________
        //
        //
        //
        //! named components
        //
        //
        //______________________________________________________________________
        class Components : public   Entity, public   Proxy<const ComponentsType>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const Symbol;          //!< "<=>";
            static const char         Separator = ':'; //!< for parser

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Components(const String * const); //!< setup
            virtual ~Components()            noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Components);               //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void operator()(const Role, const unsigned, const Species &); //!< append
            void operator()(const Role, const  Species &);                //!< (*this)(role,1,species);
            void latch() noexcept;                                        //!< latch all

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            const Actors reac; //!< current reactants
            const Actors prod; //!< current products

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            Y_PROXY_DECL();
            ComponentsType db;
        };

    }

}

#endif

