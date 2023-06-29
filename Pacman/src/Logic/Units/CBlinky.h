#ifndef C_BLINKY
#define C_BLINKY

#include "CGhost.h"

/*!
	\brief Red ghost's class.

	Inherited from CGhost class.
*/
class CBlinky : public CGhost {
public:

    /*!
       Constructor for Blinky.
       \param[in] nPos ghost's initial and current position.
       \param[in] nMode ghost's current mode.
       \param[in] dir ghost's current and initial position.
       \param[in] bonuses ghost's bonuses to release.
       Uses CGhost's constructor.
       Set start counter to true.
    */
    CBlinky(const CCoords &nPos, const EGhostmode &nMode, const EDirection &dir, unsigned int bonuses) :
            CGhost(nPos, nMode, dir, bonuses) {

        curDir = nextDir = EDirection::LEFT;

        startCounter = true;

    };

    ~CBlinky() = default;

/*!
        Overrided chase method from CGhost class.
        Blinky has simple chase method , uses defaultChase()
        method from CGhost class.
        \param[in] coords target coordinates.
     */
    void chase(const CCoords &coords) override {

        defaultChase(coords);

    };

    /*!
        Overrided scatter method from CGhost class.
        Blinky moves to the top-right maze corner.
        Uses defaultChase()
        method from CGhost class.
     */
    void scatter() override {

        defaultChase(CCoords(28, -1));

    };

/*!
        Overrided method from CGhost class.
        Casts CBlinky's pointer to base class's pointer( CGhost ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CGhost> clone() const override {

        return std::make_shared<CBlinky>(*this);

    }

};

#endif