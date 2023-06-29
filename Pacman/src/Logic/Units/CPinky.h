#ifndef C_PINKY
#define C_PINKY

#include "CGhost.h"
/*!
	\brief Pink ghost's class.

	Inherited from CGhost class.
*/
class CPinky : public CGhost
{
public:

    /*!
       Constructor for Pinky.
       \param[in] nPos ghost's initial and current position.
       \param[in] nMode ghost's current mode.
       \param[in] dir ghost's current and initial position.
       \param[in] bonuses ghost's bonuses to release.
       Uses CGhost's constructor.
       Set start counter to true.
    */
    CPinky( const CCoords & nPos ,  const EGhostmode & nMode , const EDirection & dir, unsigned int bonuses )
    :CGhost( nPos ,  nMode , dir , bonuses )
    {

        startCounter = true;

    }

    ~CPinky() = default;

/*!
        Overrided chase method from CGhost class.
        Pinky uses method pacmanChasePos() to find
        target coordinates , also uses defaultChase()
        method from CGhost class.
        Pinky chases pacman's future position in
        4 tiles ahead.
        \param[in] pacmanPos pacman's coordinates.
     */
    void chase( const CCoords & pacmanPos ) override
    {
        defaultChase( pacmanChasePos(pacmanPos) );
    }
/*!
        Overrided scatter method from CGhost class.
        Pinky moves to the top-left maze corner.
        Uses defaultChase()
        method from CGhost class.
     */
    void scatter() override
    {
        defaultChase( CCoords( -1 , -1 ));
    }
/*!
        Uses to find pacman's forward position in 4 tiles ahead ,
        depends on pacman's direction.
        If pacman moves to the top , target position
        will be 4 tiles up and 4 tiles left.
        \param[in] pacmanPos pacman's current position.
        \return pacman's forward position in 4 tiles ahead.
     */
    CCoords pacmanChasePos(const CCoords & pacmanPos );

/*!
        Overrided method from CGhost class.
        Casts CPinky's pointer to base class's pointer( CGhost ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CGhost> clone() const override
    {
        return std::make_shared<CPinky>(*this);
    }

};

#endif