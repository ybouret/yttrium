
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

            typedef Small::BareLightList<const Law> LList; //!< alias
            typedef LList::NodeType                 LNode; //!< alias
            typedef Small::CoopLightList<const Law> LRepo; //!< alias
            typedef LRepo::ProxyType                LBank; //!< alias
            
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
                    typedef CxxListOf<Group> List; //!< alias

                    explicit Group(const Law &first); //!< setup with first law
                    virtual ~Group() noexcept;        //!< cleanup

                    bool accept(const Law   &) const noexcept; //!< has common species with law
                    bool accept(const Group &) const noexcept; //!< has common species with group
                    void compile();                            //!< gather and make AuxLevel

                    const SList   crew; //!< shared species in group
                    Group *       next; //!< for list
                    Group *       prev; //!< for list

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
                const Group::List groups;          //!< independant groups of dependant laws
                const size_t      maxGroupSize;    //!< max(groups.size)

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                static void Fusion(Group::List &) noexcept;
            };
        }

    }

}



#endif

