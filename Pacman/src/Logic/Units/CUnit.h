#ifndef C_UNIT
#define C_UNIT

#include "../Map/CMaze.h"
/*!
	\brief Unit class.

	Parent class of all units in the game.
*/
class CUnit
{

public:

    /*!
        Constructor for units.
        \param[in] nPos unit's initial and current position.
     */
    CUnit( const CCoords & nPos  )
    {

        initPos = pos = nPos;


    };

     CUnit() = default;

    ~CUnit() = default;

/*!
       Check if unit can step on tile.
        \param[in] dir direction to step.
      \return true if can ,  false if can't.
     */
   virtual bool canStep( const EDirection & dir ) const
    {
        switch( dir )
        {
            case EDirection::LEFT:
                 if( pos.xPos - 1  < 0 ) return false;
                 return  CMaze::getTile( CCoords(pos.xPos - 1 , pos.yPos) ).canStep();

            case EDirection::RIGHT :
                if( pos.xPos + 1  >= 28 ) return false;
                return CMaze::getTile( CCoords(pos.xPos + 1 , pos.yPos)).canStep();

            case EDirection::DOWN :
                if( pos.yPos + 1  > 30 ) return false;
                return CMaze::getTile( CCoords( pos.xPos  , pos.yPos + 1) ).canStep();

            case EDirection::UP :
                if( pos.yPos - 1  < 0 ) return false;
                return CMaze::getTile( CCoords(pos.xPos  , pos.yPos - 1) ).canStep();

            case EDirection::IDLE :
                return false;

        }


        return false;

    }

    /*!
        Uses to move unit.
        Check if unit can step on tile ,
        and if can moves on it.
        \param[in] dir direction to move.
     */
    virtual void move( const EDirection &  dir )
    {
        if ( canStep( dir ) )
        {

            tick+=speed; // When tick reaches 100 unit ends move.

            switch( dir )
            {
                case EDirection::LEFT:
                    if(tick >= 100 )
                    {
                        pos.xPos--;
                        tick = 0;
                    }
                    break;

                case EDirection::RIGHT :
                    if(tick >= 100 )
                    {
                        pos.xPos++;
                        tick = 0;
                    }
                    break;

                case EDirection::DOWN :
                    if(tick >= 100 )
                    {
                        pos.yPos++;
                        tick = 0;
                    }
                    break;

                case EDirection::UP :
                    if(tick >= 100 )
                    {
                        pos.yPos--;
                        tick = 0;
                    }
                    break;

                default:

                    break;

            }


        }

    }

    /*!
       Set unit's position.
      \param[in] nPos new position.
     */
    void setPos( const CCoords & nPos )
    {
        pos = nPos;

    }
/*!
        Getter for unit's position.
       \return current postion.
     */
    CCoords getPos() const
    {

        return pos;

    }

    /*!
       Check if unit is dead.
      \return true if dead, false if alive.
     */
    bool isDead()
    {

        return !alive;

    }

    /*!
         Revive unit.
         Change it's position to initial
         and change alive flag to true.

     */
    virtual void revive()
    {

        pos = initPos;

        alive = true;



    }

    /*!
        Kill unit ( change alive flag to false)

     */
   virtual void die()
    {

        alive = false;

    }
    /*!
        Getter for unit's initial position.
       \return initial postion.
     */
    CCoords getInitPos() const
    {

        return initPos;

    }
/*!
        Getter for unit's speed.
       \return current speed.
     */
    float getSpeed()const
    {

        return speed;

    }

    /*!
        Set unit's speed.
        \param [in] nSpeed new speed.
     */
    void setSpeed( float nSpeed )
    {

        speed = nSpeed;

    }
/*!
        Reset unit's tick.

   */
    void resetTick()
    {

       tick = 0;

    }


protected:

    CCoords pos; ///< Unit's current position

    CCoords initPos; ///< Unit's initial position

    bool alive = true; ///< Unit's alive flag

    float speed = 0; ///< Unit's speed

    float tick = 0; ///< Unit's move tick
};

#endif