#include "../Map/CMaze.h"
#include "CBonus.h"

void CBonus::doAction()
{

    CMaze::addPoints( points );

    CMaze::addEatenBonus();

}