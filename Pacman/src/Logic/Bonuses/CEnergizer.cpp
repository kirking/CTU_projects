#include "CEnergizer.h"
#include "../Map/CMaze.h"

void CEnergizer::doAction()
{

    CMaze::addPoints( points );

    CMaze::addEatenBonus();

    CMaze::changeGhostState( EGhostmode::FRIGHTENED );

}