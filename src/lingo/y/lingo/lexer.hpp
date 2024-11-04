
//! \file

#ifndef Y_Lingo_Lexer_Included
#define Y_Lingo_Lexer_Included 1

#include "y/lingo/lexical/add-on.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! main scanner with sub-scanners to produce lexemes
        //
        //
        //______________________________________________________________________
        class Lexer : public Dictionary, public Lexical::Analyzer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Lexical::Analyzer                   Analyzer; //!< alias
            typedef Lexical::AddOn                      AddOn;    //!< alias
            typedef SuffixSet<String,Analyzer::Pointer> Database; //!< alias
            typedef Small::SoloLightList<Analyzer>      History;  //!< alias
            typedef SuffixSet<String,AddOn::Handle>     AddOns;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with name
            template <typename CAPTION> inline
            Lexer(const CAPTION &lxid) :
            Dictionary(),
            Analyzer(lxid,*this,*this),
            analyzer(this),
            lexemes(),
            history(),
            analyzers(),
            addOns()
            {
                initialize();
            }

            //! cleanup
            virtual ~Lexer() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            //! declare a newly created analyzer
            template <typename ANALYZER>
            ANALYZER & decl(ANALYZER * const newAnalyzer) {
                assert(0!=newAnalyzer);
                const Analyzer::Pointer _( newAnalyzer );
                mustInsert(_);
                return *newAnalyzer;
            }


            //! declare an AddOn = analyzer + code
            template <typename ADD_ON>
            ADD_ON & plug(const String &uuid)
            {
                ADD_ON * const addOn = new ADD_ON(*this,uuid);
                mustRecord(addOn);
                return *addOn;
            }

            //! alias
            template <typename ADD_ON>
            ADD_ON & plug(const char * const uuid)
            {
                const String _(uuid);
                return plug<ADD_ON>(_);
            }

            //! declare 1-arg AddOn
            template <typename ADD_ON, typename ARG1>
            ADD_ON & plug(const String &uuid, const ARG1 &arg1)
            {
                ADD_ON * const addOn = new ADD_ON(*this,uuid,arg1);
                mustRecord(addOn);
                return *addOn;
            }

            //! alias
            template <typename ADD_ON, typename ARG1>
            ADD_ON & plug(const char * const uuid, const ARG1 &arg1)
            {
                const String _(uuid);
                return plug<ADD_ON>(_,arg1);
            }

            



            //! release lexemes, free history, scanner to this
            void restart() noexcept;

            //! get next lexeme
            Lexeme * get(Source &source);


            //! manually change analyzer by its name
            void performCall(const Caption &);

            //! manually back from current analyzer
            void performBack();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            Analyzer *analyzer;  //!< active analyzer
            Lexemes   lexemes;   //!< cache of scanned lexemes
            History   history;   //!< for scanner call/back
            Database  analyzers; //!< existing analyzers
            AddOns    addOns;    //!< existing addons

            void     initialize();                          //!< record this into scanner
            void     mustInsert(const Analyzer::Pointer &); //!< must insert new analyzer
            void     mustRecord(AddOn * const);
        };

    }

}

#endif

