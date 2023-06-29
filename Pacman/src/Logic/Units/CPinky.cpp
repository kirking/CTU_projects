#include "CPinky.h"
#include "../../CLogic.h"
CCoords CPinky::pacmanChasePos(const CCoords &pacmanPos)
{

    int pacDir = (int) CLogic::getPacDir();
    //Condition when pacman moves up.
    if( pacDir == 2 ) return  CCoords(pacmanPos.xPos - pacDir * 2 , pacmanPos.yPos - pacDir * 2 );

    else if( pacDir == -2  ) return  CCoords(pacmanPos.xPos , pacmanPos.yPos - pacDir * 2 );

    else return ( CCoords(pacmanPos.xPos + pacDir * 4, pacmanPos.yPos  ));

}

