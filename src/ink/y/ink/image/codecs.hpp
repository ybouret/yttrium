
//! \file

#ifndef Y_Ink_Image_Codecs_Included
#define Y_Ink_Image_Codecs_Included 1

#include "y/ink/image/format.hpp"

namespace Yttrium
{

    namespace Color { class Ramp; }

    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Database of Formats
        //
        //
        //______________________________________________________________________
        class Codecs : public Codec, public Singleton<Codecs>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                 //!< "Ink::Codecs"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 20; //!< life time
            class Code;                                                              // forward declaration

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            void operator()(Format * const);            //!< record a newly created format
            bool has(const String &fid) const noexcept; //!< check if format is loaded
            bool has(const char * const fid)     const; //!< check if format is loaded

            //! check if FORMAT::CallSign is in use
            template <typename FORMAT> inline
            bool uses() const { return has( FORMAT::CallSign ); }

            //! safely use FORMAT
            template <typename FORMAT> inline
            void use() { if(!uses<FORMAT>()) (*this)( new FORMAT() ); }

            Codecs &        std(); //!< load std formats
            static Codecs & Std(); //!< instance with std formats

            //! append used formats
            void fetch(Sequence<Format::Handle> &formats) const;

            //! render a Ramp
            void renderRamp(const String &fileName,const Color::Ramp &ramp,const unit_t W, const unit_t H) const;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            //! save image according to format+options
            virtual void save(const Image  &image,
                              const String &fileName,
                              const Options *options) const;

            //! load image according to format+options
            virtual Image load(const String  &fileName,
                               const Options *options) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Codecs);
            friend class Singleton<Codecs>;
            explicit Codecs();
            virtual ~Codecs() noexcept;
        };

    }

}

#endif
