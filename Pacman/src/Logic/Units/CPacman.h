#ifndef C_PACMAN
#define C_PACMAN

#include "CUnit.h"
/*!
	\brief Pacman's class.

	Inherited from CUnit class.
*/
class CPacman : public CUnit
{

public:

    CPacman():CUnit(){};

    /*!
       Constructor for pacman.
       \param[in] nPos pacman's initial and current position.
       Uses CUnit's constructor.

    */
    CPacman( const CCoords & nPos  ):CUnit( nPos){};

    ~CPacman() = default;

/*!
        Method uses for eating bonuses on the road.
     */
    void eat()
    {
         // Check if tile is road and it has a bonus to eat.
        if( CMaze::getTile( pos ).getSym() != 'T' && dynamic_cast< CRoad & >(CMaze::getTile( pos )).hasBonus() )
        {
            dynamic_cast< CRoad & >( CMaze::getTile( pos ) ).getBonus();
        }

    }
/*!
 *      Overrided method from CUnit class.
        Uses to move Pacman and eat bonuses.
        \param[in] dir direction to move.

     */
   void move(const EDirection &dir) override
   {

       eat();
       if ( canStep( dir ) )
       {

           tick+=speed;

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
                       tick =0;
                   }
                   break;

               case EDirection::DOWN :
                   if(tick >= 100 )
                   {
                       pos.yPos++;
                       tick =0;
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

};

#endif