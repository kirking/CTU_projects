#ifndef C_GHOST
#define C_GHOST

#include <cmath>
#include <ctime>
#include "CUnit.h"
/*!
	\brief Ghost class.

	CBlinky's , CPinky's , CInky's , CClyde's base class. Inherited from CUnit class.
*/
class CGhost : public CUnit
{

public:

    /*!
       Constructor for ghosts.
       \param[in] nPos ghost's initial and current position.
       \param[in] nMode ghost's current mode.
       \param[in] dir ghost's current and initial position.
       \param[in] bonuses ghost's bonuses to release.
    */
    CGhost( const CCoords & nPos , const EGhostmode & nMode , const EDirection & dir , unsigned int bonuses ):

    CUnit( nPos )  , mode(nMode), initDir(dir)
    {

        forwardPos = nPos;

        curDir = initDir;

        bonusesToRelease = bonuses;


    };

    ~CGhost() = default;


/*!
       Change ghost's current mode.
        \param[in] nMode new ghost mode.
        If ghost's current mode equal to new mode , IDLE or RELEASE it won't change.
     */
    void changeMode( const EGhostmode & nMode )
    {

        if (mode != EGhostmode::IDLE && mode != EGhostmode::RELEASE && mode != nMode )
        {
               // After mode's change , ghosts change their direction to opposite
                if(canStep(EDirection(-(int)curDir))) nextDir = EDirection(-(int)curDir);

                if(mode == EGhostmode::FRIGHTENED)
                {

                    forwardPos = pos;

                    moveForwardPos(curDir);


                }

            mode = nMode;


        }

    }
/*!
       Overrided method from CUnit class.
       Difference with CUnit method is control ghost's RELEASE mode
      ( can pass trough opened gates )
        \param[in] dir direction to step.
      \return true if can ,  false if can't.

     */
    bool canStep(const EDirection &dir) const override
    {

            switch( dir )
            {
                case EDirection::LEFT:
                    if( pos.xPos - 1  < 0 ) return false;  //Map excess control
                    else if( CMaze::getTile( CCoords(pos.xPos - 1 , pos.yPos) ).getSym() == 'g'
                             && mode == EGhostmode::RELEASE ) return true;                             //Gate control
                    return  CMaze::getTile( CCoords(pos.xPos - 1 , pos.yPos) ).canStep();

                case EDirection::RIGHT :
                    if( pos.xPos + 1  >= 28 ) return false;
                    else if( CMaze::getTile( CCoords(pos.xPos + 1 , pos.yPos) ).getSym() == 'g'
                             && mode == EGhostmode::RELEASE ) return true;
                    return CMaze::getTile( CCoords(pos.xPos + 1 , pos.yPos)).canStep();

                case EDirection::DOWN :
                    if( pos.yPos + 1  > 30 ) return false;
                    else if( CMaze::getTile( CCoords(pos.xPos , pos.yPos + 1) ).getSym() == 'g'
                             && mode == EGhostmode::RELEASE ) return true;
                    return CMaze::getTile( CCoords( pos.xPos  , pos.yPos + 1) ).canStep();

                case EDirection::UP :
                    if( pos.yPos - 1  < 0 ) return false;
                    else if( CMaze::getTile( CCoords(pos.xPos , pos.yPos - 1) ).getSym() == 'g'
                             && mode == EGhostmode::RELEASE ) return true;
                    return CMaze::getTile( CCoords(pos.xPos  , pos.yPos - 1) ).canStep();

                case EDirection::IDLE :
                    return false;

            }

            return false;

    }
/*!
      Release ghost from box.


     */
    void  goOut()
    {

        mode = EGhostmode::RELEASE;
        //Go to the gates position
        defaultChase(CCoords(CMaze::getGatesPos().xPos -1 , CMaze::getGatesPos().yPos -2));

         //After reach gates , change mode to scatter
        if (pos == CCoords(CCoords(CMaze::getGatesPos().xPos -1 , CMaze::getGatesPos().yPos -1)))
        {

        mode = EGhostmode::SCATTER;

        }

    }

/*!
        Uses for chasing determined tile.
        \param[in] coords coordinates of tile.
        Uses checkDistane method to compute optimal trace.
     */
    void defaultChase(const CCoords & coords )
    {
        // Find optimal direction
        if(tick == 0 )checkDistance( coords );

        if ( canStep( curDir ) )
        {

            tick+=speed;

            switch( curDir )
            {
                case EDirection::LEFT:
                    if(tick >= 100 ) //end move
                    {
                        pos.xPos--;
                        tick = 0;
                        curDir = nextDir;
                    }
                    break;

                case EDirection::RIGHT :
                    if(tick >= 100 )
                    {
                        pos.xPos++;
                        tick = 0;
                        curDir = nextDir;
                    }
                    break;

                case EDirection::DOWN :
                    if(tick >= 100 )
                    {
                        pos.yPos++;
                        tick = 0;
                        curDir = nextDir;
                    }
                    break;

                case EDirection::UP :
                    if(tick >= 100 )
                    {
                        pos.yPos--;
                        tick = 0;
                        curDir = nextDir;
                    }
                    break;

                default:

                    break;

            }


        }

    }

    /*!
        Set pacman position to ghost.
        \param[in] pacPos pacman position.

     */
    void setPacmanPos( const CCoords & pacPos )
    {

        pacmanPos = pacPos;

    }

    /*!
      Overrided method from CUnit class.

      Main difference with parent's method is adding points for death.

     */
    void die() override
    {

        alive = false;

        CMaze::addGhostPoints(200);

    }

    /*!
        Check if pacman ate ghost or ghost killed pacman.

     */
    void action()
    {

        if( pos == pacmanPos )
        {

            if( mode == EGhostmode::FRIGHTENED )
            {

                die();

            }
            else
            {

                CMaze::killPacman();

            }


        }

    }

    /*!
       Uses to coordinate ghost's moves.

       Move depends on current mode.
     */
     void go()
    {

        switch ( mode )
        {

            case EGhostmode:: CHASE :
                chase(pacmanPos);
                break;

            case EGhostmode:: SCATTER :
                scatter();
                break;

            case EGhostmode:: FRIGHTENED:
                frightened();
                break;

            case EGhostmode::IDLE :
                break;

            case EGhostmode ::RELEASE :
                goOut();
                break;


        }

    }

    /*!
        Moves ghost's forward position.
        \param[in] dir direction to move.
     */
    void moveForwardPos( const EDirection & dir )
    {
       /*
        * Ghosts think one tile ahead , so they check all possible
        * directions from next tile.
        * This method move forward position.
        */
        switch( dir )
        {

            case EDirection::LEFT:
                if( forwardPos.xPos != 0 )
                forwardPos.xPos -= 1;
                break;

            case EDirection::RIGHT :
                if( forwardPos.xPos != 27 )
                forwardPos.xPos += 1;
                break;

            case EDirection::DOWN :
                if( forwardPos.yPos != 30 )
                forwardPos.yPos += 1;
                break;

            case EDirection::UP :
                if( forwardPos.yPos != 0 )
                forwardPos.yPos -= 1;
                break;
            default:
                break;

        }

    }

    /*!
        Finds optimal next direction to move.
        \param[in] coords target coordinates.
     */
    void checkDistance( const CCoords & coords )
    {

        double distance = -1;

        moveForwardPos(curDir);

        //Check all possible directions
        for( int i = -2 ; i < 3 ; ++ i )
        {
            moveForwardPos( EDirection(i) );

            // Check if can step , and if current direction isn't opposite.
            if( i != 0 && i != -(int)curDir && (CMaze::getTile(forwardPos).canStep()
            || ( mode == EGhostmode::RELEASE && CMaze::getTile(forwardPos).getSym() == 'g' ) ) )
            {

                if( distance == -1 )
                {
                    //Compute distance
                    distance = sqrt( pow( ( coords.xPos - forwardPos.xPos ) ,2 ) +
                            pow( ( coords.yPos - forwardPos.yPos) , 2));

                    nextDir = (EDirection ) i ; // Set next direction.

                }

                else if( sqrt( pow( ( coords.xPos - forwardPos.xPos ) ,2 ) +
                          pow( ( coords.yPos - forwardPos.yPos) , 2)) < distance )
                {

                    distance = sqrt( pow( ( coords.xPos - forwardPos.xPos ) ,2 ) +
                                    pow( ( coords.yPos - forwardPos.yPos) , 2));

                    nextDir = (EDirection ) i ;

                }

            }

            moveForwardPos( EDirection( -i ) );



        }

    }


/*!
        Overrided method from CUnit class.

        Differences with parent's method are :

        setting current position to initial ,

        setting current direction to initial ,

        setting mode to CHASE ,

        setting forward position to current.

     */
void revive() override
{
    pos = initPos;

    alive = true;

    curDir = initDir;

    mode = EGhostmode::CHASE;

    forwardPos = pos;

    tick = 0;
}


/*!
        Move ghost to another tunnel.
        \param[in] coords second tunnel's coordinates.
     */

void tunnelTp( const CCoords & coords )
{

        pos = coords;

        forwardPos = pos;

        tick = 0;

        nextDir = curDir;

}
/*!
        Getter for current direction.
        \return current direction.
     */
EDirection getCurDir() const
{

        return curDir;

}

/*!
        Getter for current mode.
        \return current mode.
     */
EGhostmode getMode()
{

        return mode;

}

/*!
        Checks if it's enough eaten bonuses to
        release ghost from the box.
        \return true if enough , false if not.
     */
bool timeToRelease() const
{

        return ( bonusesToRelease == releaseCounter)  && startCounter;

}

/*!
        Method uses in inherited classes to
        make possible polymorphic call ,
        also simplify memory management
        ( uses shared_ptr instead of common pointer).
        \return base class's shared_ptr.
     */
    virtual std::shared_ptr<CGhost> clone() const = 0;

/*!
        Uses for chasing pacman , each ghost has own rules fro chasing.
        \param [in] pacmanPos pacman position.
     */
    virtual void chase( const CCoords & pacmanPos ) = 0;

/*!
        Uses for choosing direction if FRIGHTENED mode.
        Pseudo-randomly choose direction.
     */
    virtual void frightened()
    {

     if(tick == 0) {


         EDirection saveDir = curDir;

         curDir = nextDir;

          //Randomly generate direction
             srand(time(nullptr));

             static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

             int tmp = static_cast<int>(rand() * fraction * (2 - (-2) + 1) + (-2));

            // If direction == 0 (IDLE) , regenerate direction.
             while (  tmp == 0 )
             {

                 static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

                 tmp = static_cast<int>(rand() * fraction * (2 - (-2) + 1) + (-2));

             }

             // Check if direction isn't opposite and ghost can step on tile in this direction
             if( canStep(EDirection(tmp)) && tmp != -(int)saveDir && tmp != -(int)curDir  )
             {

                 nextDir = EDirection(tmp);

             }
             else
             {
                 //Search possible direction
                 if(canStep(EDirection(2)) && saveDir != EDirection(-2) && curDir != EDirection(-2) ) nextDir = EDirection (2);

                 else if( canStep(EDirection(-1)) && saveDir != EDirection(1) && curDir != EDirection(1)) nextDir = EDirection (-1);

                 else if( canStep(EDirection(1)) && saveDir != EDirection(-1) && curDir != EDirection(-1)) nextDir = EDirection (1);

                 else  if( canStep(EDirection(-2)) && saveDir != EDirection(2) && curDir != EDirection(2))nextDir = EDirection (-2);


             }

         }

          move(curDir);

    }

    /*!
        Uses for controls ghost's behavior in SCATTER mode.
        Each ghost has own corner in maze to move.
     */
    virtual void scatter() = 0;

    /*!
        Checks if counter to ghost release is started.
        \return true if started , false if not.
     */
    bool counterStarted() const
    {

        return startCounter;

    }

    /*!
        Set amount of current eaten bonuses.
        \param [in] bonuses eatenBonuses.
     */
    void setBonusCount(unsigned int bonuses )
    {

        releaseCounter = bonuses;

    }

    /*!
        Start ghost's eaten bonuses release counter.
     */
    void counterStart()
    {

        startCounter = true;

        bonusesToRelease += releaseCounter;

    }

    /*!
        Uses to start or stop release counter.
        \param [in] start
     */
    void setStartCounter( bool start )
    {

        startCounter = start;

    }

    /*!
        Getter for ghost's forward position.

        \return ghost's forward position.
     */
    CCoords getForwardPos() const
    {

        return forwardPos;

    }

    /*!
        Set ghosts's new forward position.
        \param [in] nPos new forward position.
     */
    void  setForwardPos( const CCoords & nPos ) const
    {

        forwardPos = pos;

    }

    /*!
        Getter for ghost's initial direction.
        \return ghost's initial direction
     */
    EDirection getInitDir() const
    {

        return initDir;

    }
/*!
        Getter for ghost's current eaten bonus counter
        \return eaten bonus counter.
     */
    unsigned int getReleaseCounter() const
    {

        return releaseCounter;

    }

/*!
        Getter for amount of eaten bonuses needed to release ghost.

       \return bonuses amount to release.
     */
    unsigned int getBonusesToRelease() const
    {

        return bonusesToRelease;

    }

/*!
        Sets ghost's next direction.
        \param [in] dir next direction to move.
     */
    void setNextDir( const EDirection & dir )
    {

        nextDir = dir;

    }

    /*!
        Sets ghost's current direction.
        \param [in] dir current direction to move.
     */
    void setCurDir( const EDirection & dir )
    {

        curDir = dir;

    }


protected:

    EGhostmode mode = EGhostmode::CHASE; ///< Ghost's current mode

    EDirection initDir; ///< Ghost's initial direction.

    EDirection curDir; ///< Ghost's current direction.

    EDirection nextDir; ///< Ghost's next direction.

    CCoords pacmanPos; ///< Pacman position.

    unsigned int releaseCounter = 0 ; ///< EatenBonuses counter.

    unsigned int bonusesToRelease; ///< Needed amount of eaten bonuses to release.

    bool startCounter = false; ///< Release counter's flag.

    mutable CCoords forwardPos; ///< Ghost's forward position.

};

#endif