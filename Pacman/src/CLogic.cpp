#include "CLogic.h"
int CLogic::score = 0;

int CLogic::eatenBonuses = 0;

int CLogic::ghostMultiplier = 1;

EDirection CLogic::pacDir = EDirection::LEFT;

std::vector< std::shared_ptr<CGhost> > CLogic::ghosts;

void CLogic::updateScore(int points)
{

CLogic::score += points;

}

void CLogic::ghostPoints(int points)
{

    updateScore( points * ghostMultiplier );

    ghostMultiplier++;

}

void CLogic::updateEatenBonuses()
{

   eatenBonuses++;

}

EDirection CLogic::getPacDir()
{

    return pacDir;

}

CCoords CLogic::getBlinkyPos()
{

    return ghosts[0]->getPos();

}

