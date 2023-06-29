#ifndef C_CLYDE
#define C_CLYDE

#include "CGhost.h"
/*!
	\brief Orange ghost's class.

	Inherited from CGhost class.
*/
class CClyde : public CGhost {
public:

    /*!
       Constructor for Clyde.
       \param[in] nPos ghost's initial and current position.
       \param[in] nMode ghost's current mode.
       \param[in] dir ghost's current and initial position.
       \param[in] bonuses ghost's bonuses to release.
       Uses CGhost's constructor.
    */
    CClyde(const CCoords &nPos, const EGhostmode &nMode, const EDirection &dir, unsigned int bonuses)
            : CGhost(nPos, nMode, dir, bonuses) {};

    ~CClyde() = default;

/*!
        Overrided chase method from CGhost class.
        Clyde's chase behavior depends on distance
        between Clyde and pacman.
        If distance more then 8 , Clyde uses
        standart chase strategy ( as Blinky ) ,
        else Clyde moves to bottom-left maze corner.
        \param[in] pacmanPos pacman's coordinates.
     */
    void chase(const CCoords &pacmanPos) override {

        //Compute and check distance
        if (sqrt(pow((pacmanPos.xPos - pos.xPos), 2) +
                 pow((pacmanPos.yPos - pos.yPos), 2)) < 8)

            defaultChase(CCoords(-1, 31));

        else defaultChase(pacmanPos);

    };
/*!
        Overrided scatter method from CGhost class.
        Clyde moves to the bottom-left maze corner.
        Uses defaultChase()
        method from CGhost class.
     */
    void scatter() override {

        defaultChase(CCoords(-1, 31));

    }

/*!
        Overrided method from CGhost class.
        Casts CClyde's pointer to base class's pointer( CGhost ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CGhost> clone() const override {

        return std::make_shared<CClyde>(*this);

    }

};

#endif