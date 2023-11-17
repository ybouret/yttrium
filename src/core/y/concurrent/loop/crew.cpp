#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/wire.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/concurrent/condition.hpp"

namespace Yttrium
{

    namespace Concurrent
    {



        class Player : public ThreadContext, public Wire
        {
        public:
            inline explicit Player(const size_t  sz,
                                   const size_t  rk,
                                   Lockable     &mx,
                                   Crew::Code   &code) :
            ThreadContext(sz,rk,mx),
            Wire(Launch,code,*this)
            {

            }

            inline virtual ~Player() noexcept 
            {

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Player);

            //! code.call(player)
            static void Launch(Crew::Code &code, const Player &player) noexcept;

        };

        typedef Memory::Wad<Player,Memory::Dyadic> Team;

        class Crew :: Code : public Object, public Team
        {
        public:

            explicit Code(const Topology &topology) :
            Object(),
            Team(topology.size),
            access(),
            size(0),
            team( lead() ),
            item( team-1 ),
            done(0),
            waitCV(),
            doneCV()
            {
                const size_t goal = topology.size;
                assert(capacity>=goal);
                Y_THREAD_MSG("[Threads] topology = " << topology << " #" << goal);
                try
                {
                    const Topology::NodeType *node = topology.head;
                    while(size<goal)
                    {
                        assert(0!=node);

                        // start building player
                        Player *player = new ( &team[size] ) Player(goal,size,access,*this);

                        // wait for synchronization
                        {
                            Y_LOCK(access);
                            if(done<=size) doneCV.wait(access);
                        }

                        ++Coerce(size);
                        player->assign( **node );
                        node = node->next;
                    }

                    Y_THREAD_MSG("[Threads] ready #" << size);


                }
                catch(...)
                {
                    quit();
                    throw;
                }
            }

            virtual ~Code() noexcept
            {
                quit();
            }

            Mutex          access;
            const size_t   size;
            Player * const team;
            const Player * const item;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            friend class Player;

            size_t         done;
            Condition      waitCV;
            Condition      doneCV;

            inline void quit() noexcept
            {
                Y_THREAD_MSG("[Threads] quit #" << size);

                waitCV.broadcast();
                while(size>0)
                    Memory::OutOfReach::Naught( &team[--Coerce(size)] );


            }

            //! parallel code
            inline void call(const Player &player) noexcept
            {
                //--------------------------------------------------------------
                // LOCK mutex
                //--------------------------------------------------------------
                access.lock();
                Y_THREAD_MSG("[Threads] startup @" << player.name);

                //--------------------------------------------------------------
                // update #done and signal Crew this player is OK
                //--------------------------------------------------------------
                ++done;
                if(done>size) doneCV.signal();

                //--------------------------------------------------------------
                // wait on a LOCKED mutex
                //--------------------------------------------------------------
                waitCV.wait(access);
                Y_THREAD_MSG("[Threads] woke up @" << player.name);


                access.unlock();
            }

        };

        void Player:: Launch(Crew::Code &code, const Player &player) noexcept
        {
            code.call(player);
        }
    }
}

#include "y/type/nullify.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        Crew:: ~Crew() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }



        Crew:: Crew(const Topology &topology) :
        Loop(),
        code( new Code(topology) )
        {
        }
        

        
        const char * const Crew:: CallSign = "Concurrent::Crew";
        const char *       Crew:: callSign() const noexcept { return CallSign; }
        size_t             Crew:: size()     const noexcept { assert(0!=code); return code->size; }
        Crew::ConstType &  Crew:: operator[](const size_t indx) const noexcept
        {
            assert(indx>=1);
            assert(indx<=size());
            assert(indx==code->item[indx].indx);
            return code->item[indx];
        }


    }

}

