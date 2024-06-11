//! \file

#ifndef Y_Net_API_Included
#define Y_Net_API_Included 1

#include "y/string.hpp"
#include "y/singleton.hpp"


namespace Yttrium
{

	class Network : public Singleton<Network>
	{
	public:
		static const char * const      CallSign; //!< "Network"
		static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 9; //!< Longevity

	private:
		Y_DISABLE_COPY_AND_ASSIGN(Network);
		explicit Network();
		virtual ~Network() noexcept;
		friend class Singleton<Network>;
	};



}

#endif
