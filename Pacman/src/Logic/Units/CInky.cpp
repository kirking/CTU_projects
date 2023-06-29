#include "CPinky.h"
#include "../../CLogic.h"

CCoords CInky::pacmanChasePos(const CCoords &pacmanPos)
{

    CCoords blinkyPos = CLogic::getBlinkyPos();

    int pacDir = (int) CLogic::getPacDir();

    if( pacDir == 2 )
    {
        //Condition , when pacman moves up
        return  CCoords( 2 * blinkyPos.xPos - pacmanPos.xPos - pacDir  , 2 * blinkyPos.yPos - pacmanPos.yPos - pacDir );

    }

    else if( pacDir == -2  )
    {

        return  CCoords( 2 * blinkyPos.xPos - pacmanPos.xPos , 2 * blinkyPos.yPos - pacmanPos.yPos - pacDir );

    }

    else
    {

      return ( CCoords( 2 * blinkyPos.xPos - pacmanPos.xPos + pacDir * 2 , 2 * blinkyPos.yPos - pacmanPos.yPos  ));

    }




}