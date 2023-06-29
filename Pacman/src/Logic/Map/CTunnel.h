#ifndef C_TUNNEL
#define C_TUNNEL

#include "CTile.h"
#include "../../EDirection.h"
/*!
    \brief Tunnel class.

	 Represents tunnels in the maze.
     */
class CTunnel : public CTile
{

public:

    /*!
        Constructor for tunnel, set sym to 'T'.
     */
    CTunnel()
    {

        sym = 'T';

    };

    /*!
        Constructor for tunnel.
        \param[in] thisTun this tunnel coordinates.
        \param[in] nDir direction of entrance
        \param[in] secTun second tunnel coordinates.
     */
    CTunnel( const CCoords & thisTun ,  const EDirection & nDir, const CCoords & secTun = CCoords(0 , 0)  )
    {

        pos = thisTun;

        secPos = secTun;

        dir = nDir;

        sym = 'T';

    }

    ~CTunnel() = default;

    /*!
        Overrided method from CTile class.
        Casts CTunnel's pointer to base class's pointer( CTile ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CTile> clone() const override
    {

        return std::make_shared<CTunnel>(*this);

    }

    /*!
           Overrided method .
           \return true , cause units can pass trough the tunnel.

        */
    bool canStep() const override
    {

        return true;

    }

    /*!
           Getter for tunnel's position.
           \return tunnel's position.


        */
    CCoords getPos()
    {

        return pos;

    }

    /*!
          Getter for second tunnel's position.
          \return second tunnel's position.


       */
    CCoords getSecPos()
    {

        return secPos;

    }

    /*!
          Getter for tunnel's entrance direction.
          \return tunnel's entrance direction.


       */
    EDirection getDir() const
    {

        return dir;

    }

private:

    CCoords pos,    /// < tunnel's entrance position
            secPos; /// < tunnel's exit position

    EDirection dir ; /// < tunnel's entrance direction.



};

#endif