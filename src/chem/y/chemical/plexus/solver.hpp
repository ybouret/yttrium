
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/reactive/equilibrium/aftermath.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Engine : public ProBank, public ProList, public Assembly
        {
        public:
            explicit Engine() : ProBank(), ProList( static_cast<ProBank &>(*this) ), Assembly() {}
            virtual ~Engine() noexcept {}

            void update() noexcept
            {
                forget();
                for(const ProNode *pn=head;pn;pn=pn->next)
                    enroll(**pn);
            }

            void  reset() noexcept
            {
                forget();
                free();
            }

            void  show(XMLog &xml) const
            {
                Y_XML_SECTION_OPT(xml, "Chemical::Prospects", "count='" << size << "'");
                for(const ProNode *pn=head;pn;pn=pn->next)
                {
                    const Prospect &pro = **pn;
                    if(xml.verbose) {
                        const char * const info = Components::SituationText(pro.out.st);
                        print(xml() << info << " @",pro,Justify::Left)
                        << " : |xi| = | " << std::setw(15) << real_t(pro.xi) << " |"
                        << " ff = " << std::setw(15) << real_t(pro.ff)
                        << std::endl;
                    }
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Mix solver
        //
        //
        //______________________________________________________________________
        class Solver : public Aftermath, public Proxy<const Engine>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Solver"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Solver(const Mix &); //!< setup from persistent mix
            virtual ~Solver() noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void run(XMLog &xml, XWritable &C, const Level L, const XReadable &K);
            void show(XMLog &xml) const;


            xReal objectiveFunction(const XReadable &C, const Level L);
            xReal objectiveFunction(const xReal u);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            virtual ConstInterface & surrogate() const noexcept;
            
            const Mix   & mix;          //!< persistent mix
            Engine        my;
            XMatrix       Csolve;       //!< all possible solving C
            XMatrix       deltaC;       //!< all possible delta   C
            XArray        Cini;         //!< starting point
            XArray        Cend;         //!< final point
            XArray        Ctmp;         //!< updated by objectiveFunction(u)


        };
    }

}

#endif

