#ifndef C_INKY
#define C_INKY

#include "CGhost.h"
/*!
	\brief Blue ghost's class.

	Inherited from CGhost class.
*/
class CInky : public CGhost
{
public:

    /*!
       Constructor for Inky.
       \param[in] nPos ghost's initial and current position.
       \param[in] nMode ghost's current mode.
       \param[in] dir ghost's current and initial position.
       \param[in] bonuses ghost's bonuses to release.
       Uses CGhost's constructor.
    */
    CInky( const CCoords & nPos ,  const EGhostmode & nMode , const EDirection & dir , unsigned int bonuses)
    :CGhost( nPos ,  nMode , dir , bonuses){};

    ~CInky() = default;


/*!
        Overrided chase method from CGhost class.
        Inky uses method pacmanChasePos() to find
        target coordinates , also uses defaultChase()
        method from CGhost class.
        Inky chases position , which resulted from
        doubling vector between Blinky's position
        and Blinky's chase position.
        \param[in] pacmanPos pacman's coordinates.
     */
    void chase( const CCoords & pacmanPos ) override
    {

     defaultChase(pacmanChasePos(pacmanPos));

    }
/*!
        Overrided scatter method from CGhost class.
        Inky moves to the bottom-right maze corner.
        Uses defaultChase()
        method from CGhost class.
     */
    void scatter() override
    {

        defaultChase( CCoords( 28, 31 ));

    }



/*!
        Uses to find target position , which resulted from
        doubling vector between Blinky's position
        and Blinky's chase position.
        If pacman moves to the top , Blinky's chase position
        will be 2 tiles up and 2 tiles left.
        \param[in] pacmanPos pacman's current position.
        \return target position.
     */
    CCoords pacmanChasePos(const CCoords &pacmanPos);

/*!
        Overrided method from CGhost class.
        Casts CInky's pointer to base class's pointer( CGhost ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CGhost> clone() const override
    {

        return std::make_shared<CInky>(*this);

    }

};

#endif