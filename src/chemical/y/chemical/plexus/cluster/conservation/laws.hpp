
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/plexus/cluster/conservation/law.hpp"
#include "y/chemical/assembly.hpp"
#include "y/container/matrix.hpp"
#include "y/stream/xmlog.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            typedef Small::BareLightList<const Law> LList;
            typedef LList::NodeType                 LNode;

            //__________________________________________________________________
            //
            //
            //
            //! List of Laws
            //
            //
            //__________________________________________________________________
            class Laws : public Law::List, public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________


                //______________________________________________________________
                //
                //
                //! independant group of dependents laws
                //
                //______________________________________________________________
                class Group : public Object, public LList
                {
                public:
                    typedef CxxListOf<Group> List;

                    explicit Group(const Law &first);
                    virtual ~Group() noexcept;

                    Group *next; //!< for list
                    Group *prev; //!< for list

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Group);
                };

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
               
                //! setup
                explicit Laws(const SList            &sp,
                              const Matrix<unsigned> &Qm);

                //! cleanup
                virtual ~Laws() noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Group::List groups;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
            };
        }

    }

}



#endif

