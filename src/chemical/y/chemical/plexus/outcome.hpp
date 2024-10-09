//! \file

#ifndef Y_Chemical_Plexus_Outcome_Included
#define Y_Chemical_Plexus_Outcome_Included 1



namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! Solver process outcome
        //
        //______________________________________________________________________
        enum Outcome
        {
            Jammed, //!< no active equilibrium
            Solved, //!< exact zero objectice function
            Better //!< improved objective function
            //Locked  //!< spurious objectice function
        };

    }

}

#endif

