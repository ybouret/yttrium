//! \file

#ifndef Y_Net_API_Included
#define Y_Net_API_Included 1

#include "y/string.hpp"
#include "y/singleton.hpp"
#include "y/system/duration.hpp"
#include "y/system/exception.hpp"

#if defined(Y_WIN)
#include <ws2tcpip.h>
#endif

#if defined(Y_BSD)
#include <sys/select.h>
#endif

namespace Yttrium
{

    namespace Network
    {

#if defined(Y_BSD)
        typedef Libc::Exception  Exception; //!< alias
#endif

#if defined(Y_WIN)
        typedef Win32::Exception Exception; //!< alias
#endif

        class API : public Singleton<API>
        {
        public:
            //______________________________________________________________________
            //
            //
            // Definitions
            //
            //______________________________________________________________________
            static const char * const      CallSign;                                //!< "Network"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 9; //!< Longevity

            //______________________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________________
            bool isError(const int returnValue) const noexcept; //!< returnValue to error
            int  lastError()                          noexcept; //!< errno, WSAGetLastError()...

        private:
            Y_DISABLE_COPY_AND_ASSIGN(API);
            friend class Singleton<API>;
            explicit API();
            virtual ~API() noexcept;
            
        };

    }

#if 0
    //__________________________________________________________________________
    //
    //
    //
    //! Network initialization and API
    //
    //
    //__________________________________________________________________________
	class Network : public Singleton<Network>
	{
    public:

        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
		static const char * const      CallSign;                                //!< "Network"
		static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 9; //!< Longevity
#if defined(Y_BSD)
        typedef Libc::Exception Exception; //!< alias
#endif

#if defined(Y_WIN)
        typedef Win32::Exception Exception; //!< alias
#endif

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        static bool IsError(const int returnValue) noexcept; //!< returnValue to error
        static int  LastError()                    noexcept; //!< errno, WSAGetLastError()...

        //! wrapper around the ::select function
        size_t select(int         nfds,
                      fd_set *   readfds,
                      fd_set *   writefds,
                      fd_set *   errorfds,
                      Duration      &duration);

        void   sleepFor(double ns); //!< broken...

	private:
		Y_DISABLE_COPY_AND_ASSIGN(Network);
		explicit Network();
		virtual ~Network() noexcept;
		friend class Singleton<Network>;
	};
#endif
    


}

#endif
