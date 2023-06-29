#ifndef E_GHOST_MODE
#define E_GHOST_MODE
///Possible ghost modes
enum class EGhostmode
{
    FRIGHTENED, ///< Ghost mode , when ghosts are scared and run away from pacman
    SCATTER, ///< Ghost mode , when ghosts scatter ( move to their maze corner )
    CHASE, ///< Ghost mode , when ghosts chases pacman
    IDLE, ///< Ghost mode , when ghosts don't move
    RELEASE ///< Ghost mode , when ghosts go out from ghost's box (in center of maze)
};

#endif