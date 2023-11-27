#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/wire.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/concurrent/condition.hpp"
#include "y/type/temporary.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! a Player is a Thread with a Context
        //
        //
        //______________________________________________________________________
        class Player : public ThreadContext, public Wire
        {
        public:

            //! setup and start thread, up to fist sync
            inline explicit Player(const size_t  sz,
                                   const size_t  rk,
                                   Lockable     &mx,
                                   Crew::Code   &code) :
            ThreadContext(sz,rk,mx), Wire(Launch,code,*this)
            {

            }

            //! cleanup: join thread
            inline virtual ~Player() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Player);

            //! entry point to run code.call(player) in this thread
            static void Launch(Crew::Code &code, const Player &player) noexcept;

        };

        //______________________________________________________________________
        //
        //! alias
        //______________________________________________________________________
        typedef Memory::Wad<Player,Memory::Dyadic> Team;

        //______________________________________________________________________
        //
        //
        //
        //! a Crew is a Team of Player with concurrent primitives
        //
        //
        //______________________________________________________________________
        class Crew :: Code : public Object, public Team
        {
        public:
            typedef const Temporary<Kernel *> SetKernel;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup and synchronize threads
            explicit Code(const Topology &topology) :
            Object(),
            Team(topology.size),
            kRun(0),
            sync(),
            size(0),
            team( lead() ),
            item( team-1 ),
            done(0),
            waitCV(),
            doneCV()
            {
                const size_t goal = topology.size;
                assert(capacity>=goal);
                Y_THREAD_MSG("[Crew] -------- topology     =" << topology << " #" << goal);

                try
                {
                    const Topology::NodeType *node = topology.head;
                    while(size<goal)
                    {
                        assert(0!=node);

                        //------------------------------------------------------
                        // start building player
                        //------------------------------------------------------
                        Player *player = new ( &team[size] ) Player(goal,size,sync,*this);

                        //------------------------------------------------------
                        // wait for synchronization
                        //------------------------------------------------------
                        {
                            Y_LOCK(sync);
                            if(done<=size) doneCV.wait(sync);
                        }

                        //------------------------------------------------------
                        // update status
                        //------------------------------------------------------
                        ++Coerce(size);
                        player->assign( **node );
                        node = node->next;
                    }

                    Y_THREAD_MSG("[Crew] -------- synchronized #" << size);
                    done = 0;
                }
                catch(...)
                {
                    quit();
                    throw;
                }
            }

            virtual ~Code() noexcept { quit(); }

            //__________________________________________________________________
            //
            //
            //! perform kernel in parallel
            //
            //__________________________________________________________________
            inline void cycle(Kernel &kernel) noexcept
            {
                assert(0==done);
                assert(0==kRun);

                SetKernel k(kRun, &kernel);                        // setting the kernel to run
                waitCV.broadcast();                                // wake up everyone
                { Y_LOCK(sync); if(done<size) doneCV.wait(sync); } // synchronize
                assert(size==done); done = 0;                      // done
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Kernel              *kRun;
            Mutex                sync;
            const size_t         size;
            Player * const       team;
            const Player * const item;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            friend class Player;

            size_t         done;
            Condition      waitCV;
            Condition      doneCV;

            inline void quit() noexcept
            {
                // assuming everyone is waiting
                Y_THREAD_MSG("[Crew] -------- quit #" << size);
                waitCV.broadcast();
                while(size>0)
                    Memory::OutOfReach::Naught( &team[--Coerce(size)] );
                Y_THREAD_MSG("[Crew] -------- done");
            }

            //! parallel code assigned to a given player
            inline void call(const Player &player) noexcept
            {
                const char * const id = player.name;
                //--------------------------------------------------------------
                // LOCK mutex
                //--------------------------------------------------------------
                sync.lock();
                Y_THREAD_MSG("[Crew@" << id << "] startup");

                //--------------------------------------------------------------
                // update #done and signal Crew this player is OK
                //--------------------------------------------------------------
                ++done;
                if(done>size) doneCV.signal();

                //--------------------------------------------------------------
                // wait on a LOCKED mutex
                //--------------------------------------------------------------
            CYCLE:
                waitCV.wait(sync);
                //Y_THREAD_MSG("[Threads] " << id << " woke up");

                if(0==kRun)
                {
                    //----------------------------------------------------------
                    Y_THREAD_MSG("[Crew@" << id << "] returning...");
                    //----------------------------------------------------------
                    sync.unlock();
                    return;
                }
                else
                {
                    //----------------------------------------------------------
                    // running unlocked
                    Y_THREAD_MSG("[Crew@" << id << "] running...");
                    //----------------------------------------------------------
                    sync.unlock();
                    try        { (*kRun)(player); }
                    catch(...) { }
                    sync.lock();
                    if(++done>=size) doneCV.signal();
                    goto CYCLE;
                }
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
            assert(0!=code);
            assert(indx>=1);
            assert(indx<=size());
            assert(indx==code->item[indx].indx);
            return code->item[indx];
        }

        void Crew:: run(Kernel &kernel) noexcept
        {
            assert(0!=code);
            code->cycle(kernel);
        }

    }

}

