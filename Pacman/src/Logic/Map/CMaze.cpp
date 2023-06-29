#include "CMaze.h"
#include "../../CLogic.h"

bool CMaze::pacmanDied = false; ///< pacman's death indicator.

bool CMaze::frightened = false; ///< FRIGHTENED game mode trigger.

CCoords CMaze::gatesPos = CCoords(14 , 12); ///< gate's position in the maze.

std::shared_ptr<CTile> CMaze::maze[28][31]; ///< maze array of tiles.

/*!
        Getter for tunnel position in the maze.
        \return tunnel position.
     */
CCoords CMaze::getGatesPos()
{

    return gatesPos;

}
/*!
        Method uses to add points to the game score.
        \param[in] nPoints amount of points to add.
     */
void CMaze::addPoints( int nPoints )
{
    CLogic::updateScore(nPoints);
}
/*!
        Method uses to add score points for eaten ghost to the game score.
        \param [in] nPoints amount of points.
     */
void CMaze::addGhostPoints( int nPoints )
{
    CLogic::ghostPoints(nPoints);
}
/*!
        Method uses to add eaten bonuses to the game.
     */
void CMaze::addEatenBonus()
{
   CLogic::updateEatenBonuses();
}
/*!
        Method uses to activate FRIGHTENED trigger.
        \param[in] mode depends on mode activate trigger.
     */
void CMaze::changeGhostState(const EGhostmode &mode)
{

frightened = ( mode == EGhostmode::FRIGHTENED ) ;


}

