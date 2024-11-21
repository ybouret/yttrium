//! \file

#ifndef Y_Lingo_Pattern_Matching_Included
#define Y_Lingo_Pattern_Matching_Included 1


#include "y/lingo/pattern/regexp.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Matching pattern helper
        //
        //
        //______________________________________________________________________
        class Matching : public Token
        {
        public:
            enum Method
            {
                Exactly,
                Somehow
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from expression
            template <typename EXPR>
            inline Matching(const EXPR &             expr,
                            const Dictionary * const dict = 0) :
            Token(), motif( RegExp(expr,dict) )
            {
            }

            Matching(const Matching &);   //!< cloned copy
            virtual ~Matching() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check exact match
            template <typename NAME, typename DATA> inline
            bool exactly(const NAME &name, const DATA &data)
            {
                return exactly_( Module::OpenData(name,data) );
            }

            
            //! check partial match
            template <typename NAME, typename DATA> inline
            bool somehow(const NAME &name, const DATA &data)
            {
                return somehow_( Module::OpenData(name,data) );
            }

            template <typename NAME, typename DATA> inline
            bool operator()(const Method how, const NAME &name, const DATA &data)
            {
                switch( how )
                {
                    case Exactly: return exactly(name,data);
                    case Somehow: return somehow(name,data);
                }
                // NEVER get here
                return false;
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const AutoPtr<const Pattern> motif; //!< compiled pattern

        private:
            Y_DISABLE_ASSIGN(Matching);
            bool exactly_(Module * const);
            bool somehow_(Module * const);
        };

    }
}

#endif

