#ifndef C_LOGIC
#define C_LOGIC

#include <vector>
#include <iostream>
#include <fstream>
#include <SDL_timer.h>
#include "Logic/Units/CPacman.h"
#include "Logic/Units/CBlinky.h"
#include "Logic/Units/CPinky.h"
#include "Logic/Units/CInky.h"
#include "Logic/Units/CClyde.h"
#include "CGameDifficulty.h"
#include "Logic/Map/CMaze.h"

/*!
    \brief Game logic class.
     Controls all game events,
     save and load game , unite all game mechanics.
     */
class CLogic
{

public:


    CLogic() = default;

/*!
       Destructor for Logic.

       Delete difficulty and
       set all static variables to default.
    */
    ~CLogic()
    {

        ghosts.clear();

        eatenBonuses = 0;

        score = 0;

        ghostMultiplier = 1;

        pacDir = EDirection:: LEFT;

        delete difficulty;

    }
/*!
       Method uses check game logic state.
       \return true if game running ,
       else false.
     */
    bool isRunning() const
    {

        return run;

    }

    /*!
       Method uses to start game logic.
     */
    void start()
    {

        run = true;

    }

    /*!
       Method uses to set new ghostmode
       to ghosts.
       \param[in] mode new ghost mode.
     */
    void setGhostMode( const EGhostmode & mode )
    {

        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

            ghost->changeMode( mode );

        }

    }
/*!
       Method uses to set new speed
       to ghosts.
       \param[in] speed new ghost speed.
     */
    void setGhostSpeed( float speed )
    {


        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

            ghost->setSpeed( speed );

        }

    }
/*!
       Method uses to release ghost,
       which must be released.
     */
    void releaseTheGhost()
    {

        bool startCounter = false;

        for(std::shared_ptr<CGhost> & ghost : ghosts )
        {

            if (ghost->getMode() == EGhostmode::IDLE && ghost->timeToRelease() ) {

                map.openTheGate();

                ghost->goOut();

                startCounter = true;

            }
            else if(startCounter)
            {

                ghost->counterStart();

                startCounter = false;

            }

        }

    }

    /*!
       Method uses to check
       if some unit is entering the tunnel
       and transport it through.
     */
    void checkTunnel()
    {

        CTunnel tmp = dynamic_cast<  CTunnel & >( CMaze::getTile(map.getTunnelPos()));

        if( pacmanPos == tmp.getPos() && pacDir == tmp.getDir() )
        {

            pacman.setPos( tmp.getSecPos());

            SDL_Delay(200);

        }

        else if ( pacmanPos == tmp.getSecPos() && pacDir == dynamic_cast<  CTunnel & >( CMaze::getTile( tmp.getSecPos())).getDir() )
        {

            pacman.setPos( tmp.getPos());

            SDL_Delay(200);

        }

        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

            if( ghost->getPos() == tmp.getPos() && ghost->getCurDir() == tmp.getDir() )
            {

                ghost->tunnelTp(tmp.getSecPos());

                ghost->setSpeed(difficulty->getGhostSpeed(2));



            }
            else if( ghost->getPos() == tmp.getSecPos() && ghost->getCurDir() == dynamic_cast<  CTunnel & >( CMaze::getTile( tmp.getSecPos())).getDir() )
            {

                ghost->tunnelTp(tmp.getPos());

                ghost->setSpeed(difficulty->getGhostSpeed(2));

            }

        }


    }

/*!
       Method uses to set pacman's current postion to ghosts.
     */
    void pacPosToGhost()
    {

        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

            ghost->setPacmanPos( pacmanPos );

        }

    }
/*!
       Getter for current score.
     */
    int getScore() const
    {

        return score;

    }
/*!
       Check gate and allow to pass trough the ghost ,
       which mode is RELEASE.
     */
    void checkGate()
    {

        if( CMaze::getTile(map.getGatesPos()).getSym() == 'g' )
        {

            bool flag = true;

            for( std::shared_ptr<CGhost> & ghost : ghosts )
            {

                if( ghost->getMode() == EGhostmode::RELEASE )
                {

                    flag = false;

                }


            }

            if( flag ) map.closeTheGate();

        }

    }
/*!
       Method uses to check if it's time to
       spawn fruit , if it is ,
       randomly generate time ( between 9 and 10.5 seconds )
       set scareTimer and spawn fruit.
     */
    void fruitCheck()
    {

        if( difficulty->timeToSpawnFruit(eatenBonuses)  && fruitTimer.checkTime() )
        {
            map.setFruit(difficulty->fruitPoints() );

            //Generate random time value

            srand(time(nullptr));
            static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
            double fruitTime = (rand() * fraction * 2 + 9 ) * 1000 ;

            fruitTimer.setTimer( fruitTime );

        }
        else if( fruitTimer.checkTime() && !fruitTimer.isStopped())
        {

            map.removeFruit();
            fruitTimer.stopTimer();

        }

    }

    /*!
       Method uses to update objects and maze states.
     */
    void update()
    {

        nextLevel();

        fruitCheck();

        gameEvents();

        ghostAction();

        checkPacmanState();

        pacPosToGhost();

        pacmanGo();

        ghostGo();

    }

    /*!
       Method uses to set current amount
       of eaten bonuses to all ghosts.

     */
    void bonusesToGhost()
    {

        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

            ghost->setBonusCount(eatenBonuses);

        }

    }

/*!
       Method uses to control ghosts moves.
     */
    void ghostGo()
    {

        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

            ghost->go();

        }


    }
/*!
       Getter for ghostmode.
       \param[in] i
       : 0 - Blinky,
          1 - Pinky,
          2 - Inky,
          3 - Clyde.
     */
    EGhostmode getMode( int i )
    {

        return ghosts[i]->getMode();

    }
/*!
       Method uses to check
       if any ghost kills pacman
       or pacman eats ghost.
     */
    void ghostAction()
    {

        for (std::shared_ptr<CGhost> & ghost : ghosts)
        {

                ghost->action();

        }

    }
/*!
       Method uses to check pacman life
       status and controls pacman lives,
       if lives are over - game ended,
       if pacman dies respawn all units.
     */
    void checkPacmanState()
    {

        if( map.pacmanStat() )
        {

            pacman.die();


        }

        if( pacman.isDead() )
        {

            pacmanLives--;

            if( !pacmanLives )
            {

                endGame();

            }
            else
                {
                pacman.revive();
                map.revivePacman();

                    for (std::shared_ptr<CGhost> & ghost : ghosts)
                    {

                        if( ghost->getMode() != EGhostmode::RELEASE
                        && ghost->getMode() != EGhostmode::IDLE ) {
                            ghost->revive();

                            map.openTheGate();

                            ghost->goOut();

                        }

                    }

                pacDir = difficulty->getPacInitDir();
                }

            SDL_Delay(400);

        }

        pacmanPos = pacman.getPos();

    }

    /*!
       Method uses to set pacman current direction.
     */
    void pacmanGo()
    {

            pacman.move(pacDir);

    }
/*!
       Method uses to check , if
       amount of eaten bonuses is enough to change
       level , respawn maze , units , update difficulty ,
       and change level.
     */
    void nextLevel()
    {

        if( difficulty->checkNextLevel(eatenBonuses) )
        {

            SDL_Delay(1000);

            difficulty->nextLevel();

            map.regenerateMaze(); //Regenerate maze

            //Set pacman configuration to initial

            pacmanPos = pacman.getInitPos();

            pacman.setSpeed( difficulty->getPacmanSpeed(0) );

            pacDir = difficulty->getPacInitDir();

            pacman.resetTick();

            pacman.revive();

            eatenBonuses = 0;

            //Set ghosts's configurations to initial
            CCoords blinkyInit = ghosts[0]->getInitPos() ,
            pinkyInit = ghosts[1]->getInitPos() ,
            inkyInit = ghosts[2]->getInitPos(),
            clydeInit = ghosts[3]->getInitPos();


            ghosts.clear();
            ghosts.push_back( (CBlinky( blinkyInit ,  EGhostmode::SCATTER , EDirection::LEFT , 0) ).clone());
            ghosts.push_back( (CPinky( pinkyInit,  EGhostmode::IDLE , EDirection::UP , 0) ).clone());
            ghosts.push_back( (CInky( inkyInit ,  EGhostmode::IDLE , EDirection::UP, 30 )).clone());
            ghosts.push_back( (CClyde( clydeInit,  EGhostmode::IDLE , EDirection::UP, 60 ) ).clone());


            float tmpSpeed = difficulty->getGhostSpeed(0);

            setGhostSpeed(tmpSpeed);



        }

    }


    /*!
         Method uses to control game modes,
         gates , tunnels and ghost's release.
       */
    void gameEvents()
    {

        difficulty->changeTimings();

        releaseTheGhost();

        checkGate();

        checkTunnel();

        if( checkGhostsState() ) {


            if (difficulty->timeToScatter(curMode == EGhostmode::FRIGHTENED ))
            {

                //Setting pacman and ghosts's current mode speed
                setGhostSpeed(difficulty->getGhostSpeed(0));
                pacman.setSpeed(difficulty->getPacmanSpeed(0));
                setGhostMode(EGhostmode::SCATTER);

                ghostMultiplier = 1;

                curMode = EGhostmode::SCATTER;


            } else if( difficulty->timeToChase() )
            {
                pacman.setSpeed(difficulty->getPacmanSpeed(0));
                setGhostSpeed(difficulty->getGhostSpeed(0));
                setGhostMode(EGhostmode::CHASE);

                ghostMultiplier = 1;

                curMode = EGhostmode::CHASE;

            }
        }


    }
/*!
       Method uses to control game modes.
     */
    bool ghostState()
    {

        bonusesToGhost();

        if( map.areScared() && difficulty->getScareTime() != 0 )
        {

            setGhostSpeed(difficulty->getGhostSpeed(1));

            pacman.setSpeed(difficulty->getPacmanSpeed(1));

            if(difficulty->canFright() )
            {

                setGhostMode(EGhostmode::FRIGHTENED);

                curMode = EGhostmode::FRIGHTENED;

            }

            scareTimer.setTimer(difficulty->getScareTime() );

            map.calmDown();

            return false;
        }

        return scareTimer.checkTime();

    }
/*!
       Method uses to check ghost's life status ,
       and respawn them.
     */
    bool checkGhostsState()
    {
        bool tmp = ghostState();

            for (std::shared_ptr<CGhost> & ghost : ghosts)
            {

                if (ghost->isDead()) {

                    ghost->revive();

                    map.openTheGate();

                    ghost->goOut();

                }

            }


           if(tmp) scareTimer.stopTimer();

           return tmp;

    }

/*!
       Method uses to load and configure saved game.
       \param[in] fileName name of saved game.
       \return true if game successfuly loaded
       and configured , else false.
     */
    bool loadGame(const char * fileName)
    {

        if( !map.loadMap(fileName) )
        {

            return false;

        }

        difficulty = new CGameDifficulty();

         if ( !difficulty->loadFromFile( fileName) )

         {

             return false;

         }

        std::string line;

        std::ifstream in(fileName);

        if (in.is_open())
        {


            try {
                getline(in, line);

                if(line.empty() )
                {
                    std::cerr << "ERROR: Empty File\n";
                    return false;
                }

                line.erase(0, line.find('=') + 1);

                score = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                eatenBonuses = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacmanLives = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacDir = EDirection(std::stoi(line));

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacmanPos.xPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacmanPos.yPos = std::stoi(line);

                pacman = CPacman(pacmanPos);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacmanPos.xPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacmanPos.yPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                pacman.setSpeed(std::stof(line));

                pacman.setPos(pacmanPos);

           //Load ghosts
                for (int i = 0; i < 4; ++i) {

                    CCoords tmp;

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    tmp.xPos = std::stoi(line);

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    tmp.yPos = std::stoi(line);

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    EGhostmode tmpMode = EGhostmode(std::stoi(line));

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    EDirection tmpDir = EDirection(std::stoi(line));

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    switch (i) {

                        case 0:
                            ghosts.push_back(CBlinky(tmp, tmpMode, tmpDir, std::stoi(line)).clone());
                            break;
                        case 1:
                            ghosts.push_back(CPinky(tmp, tmpMode, tmpDir, std::stoi(line)).clone());
                            break;
                        case 2:
                            ghosts.push_back(CInky(tmp, tmpMode, tmpDir, std::stoi(line)).clone());
                            break;
                        case 3:
                            ghosts.push_back(CClyde(tmp, tmpMode, tmpDir, std::stoi(line)).clone());
                            break;
                        default:
                            break;

                    }


                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    tmp.xPos = std::stoi(line);

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    tmp.yPos = std::stoi(line);

                    ghosts[i]->setPos(tmp);

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    ghosts[i]->setNextDir(EDirection(std::stoi(line)));

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    ghosts[i]->setCurDir(EDirection(std::stoi(line)));

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    ghosts[i]->setBonusCount(std::stoi(line));

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    ghosts[i]->setStartCounter(std::stoi(line));

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    tmp.xPos = std::stoi(line);

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    tmp.yPos = std::stoi(line);

                    ghosts[i]->setForwardPos(tmp);

                    getline(in, line);

                    line.erase(0, line.find('=') + 1);

                    ghosts[i]->setSpeed(std::stof(line));

                }

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                if (std::stoi(line)) {

                    fruitTimer.setTimer(std::stoi(line));

                }

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                if (std::stoi(line)) {

                    scareTimer.setTimer(std::stoi(line));

                }

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                curMode = EGhostmode ( std::stoi(line));

            }
            catch (const std::invalid_argument& e)
            {

                std::cerr << "Custom file read error : "<<e.what() << std::endl;

                return false;

            }

        }
        else
        {

            std::cerr << "Cannot open save file \n";

            return false;

        }

        in.close();

        return true;

    }

    /*!
       Method uses to update current game score.
     */
    static void updateScore( int points );
/*!
       Method uses to add points for eaten ghosts to game score.
       \param[in] points amount of points.
     */
    static void ghostPoints( int points );

    /*!
       Method uses to end game ,
       and update high score.
     */
    void endGame()
    {

        run = false;

        saveScore("../poligkir/examples/highScore.txt");

    }

    /*!
       Method uses to update high score file.
       \param[in] filename name of high score file.
     */
    void saveScore( const char * filename ) const
    {

        std::string line;

        std::ifstream in(filename );
        if (in.is_open())
        {
            getline(in, line);

        }
        else
        {


            std::ofstream out(filename);

            if(out.is_open() )
            {

                out << score << std::endl;

            }
            else
            {

                std::cerr<<" Write score error\n";

                return;
            }
        }

        try
        {

            std::stoi(line);

        }
        catch (const std::invalid_argument& e)
        {


            return;

        }


        if( std::stoi(line) < score )
        {

            std::ofstream out(filename);

            if(out.is_open() )
            {

                out << score << std::endl;

                out.close();

            }
            else
            {

                std::cerr<<" Write score error\n";

                return;
            }

        }

        in.close();


    }
/*!
       Method uses to create string save file.
     */
    std::string saveGame()
    {

        std::string logicString = "score =" + std::to_string(score)
                +"\neatenBonuses =" + std::to_string(eatenBonuses)
                +"\npacmanLifes =" + std::to_string(pacmanLives)
                +"\npacDir =" + std::to_string((int)pacDir)
                +"\npacmanInitPos.x =" + std::to_string(pacman.getInitPos().xPos)
                +"\npacmanInitPos.y =" + std::to_string(pacman.getInitPos().yPos)
                +"\npacmanPos.x =" + std::to_string(pacmanPos.xPos)
                +"\npacmanPos.y =" + std::to_string(pacmanPos.yPos)
                +"\npacSpeed =" + std::to_string(pacman.getSpeed());

        for( int i = 0 ; i < (int)ghosts.size() ; ++ i )
        {

            std::string ghostName;

            switch(i)
            {

                case 0:
                    ghostName = "Blinky";
                    break;
                case 1:
                    ghostName = "Pinky";
                    break;
                case 2:
                    ghostName = "Inky";
                    break;
                case 3:
                    ghostName = "Clyde";
                    break;
                default:
                    break;

            }

            logicString+= (
                           "\n"+ghostName + "InitPos.x =" + std::to_string(ghosts[i]->getInitPos().xPos)
                           +"\n"+ ghostName + "InitPos.y =" + std::to_string(ghosts[i]->getInitPos().yPos)
                           +"\n"+ ghostName + "State ="+ std::to_string((int)ghosts[i]->getMode()) //ENUM
                           +"\n"+ghostName + "InitDir =" + std::to_string((int)ghosts[i]->getInitDir())
                           +"\n"+ghostName + "BonusesToRelease =" + std::to_string((int)ghosts[i]->getBonusesToRelease())
                           +"\n"+ ghostName + "Pos.x ="+ std::to_string(ghosts[i]->getPos().xPos)
                           +"\n"+ ghostName + "Pos.y ="+ std::to_string(ghosts[i]->getPos().yPos)
                           +"\n"+ghostName + "nextDir =" + std::to_string((int)ghosts[i]->getInitDir())
                           +"\n"+ghostName + "CurDir =" + std::to_string((int)ghosts[i]->getCurDir())
                           +"\n"+ghostName + "ReleaseCounter =" + std::to_string((int)ghosts[i]->getReleaseCounter())
                           +"\n"+ghostName + "startCounter =" + std::to_string((int)ghosts[i]->counterStarted())
            +"\n"+ ghostName + "ForwardPos.x ="+ std::to_string(ghosts[i]->getForwardPos().xPos)
            +"\n"+ ghostName + "ForwardPos.y ="+ std::to_string(ghosts[i]->getForwardPos().yPos)
            +"\n"+ ghostName + "Speed ="+ std::to_string(ghosts[i]->getSpeed()));

        }

   logicString+=("\nfruitTimer =" + std::to_string( fruitTimer.getLastTime() )
   +"\nscareTimer =" + std::to_string(scareTimer.getLastTime() ) );



        logicString +=( "\ncurMode =" +std::to_string((int)curMode) +"\n") ;

        logicString+=map.saveMap();

        logicString+=difficulty->saveDifficulty();

        return logicString;


    }

    /*!
       Method uses for
       default game initialization , without custom options and default map
     */
    void defaultInit()
    {

        map.generateDefault();

        difficulty = new CGameDifficulty();

        pacmanLives = difficulty->getPacmanLives();

        pacman = CPacman( CCoords(14 , 23)  );

        ghosts.push_back( (CBlinky( CCoords(13 , 11) ,  EGhostmode::SCATTER , EDirection::LEFT , 0) ).clone());
        ghosts.push_back( (CPinky( CCoords(13 , 14) ,  EGhostmode::IDLE , EDirection::UP , 0) ).clone());
        ghosts.push_back( (CInky( CCoords(11 , 14) ,  EGhostmode::IDLE , EDirection::UP, 30 )).clone());
        ghosts.push_back( (CClyde( CCoords(15 , 14) ,  EGhostmode::IDLE , EDirection::UP, 60 ) ).clone());


        setGhostSpeed( difficulty->getGhostSpeed(0) );

    }

    /*!
       Method uses to load and configure
       custom game.
       \param[in] filename name of custom game file.
     */
    bool customGame( const char * filename )
    {

        if( !map.customGame( filename ) )
        {

            return false;

        }

        difficulty = new CGameDifficulty();

       if( !difficulty->customGame( filename ) )
       {

           return false;
       }

        pacmanLives = difficulty->getPacmanLives();

        pacman = CPacman( CCoords(14 , 23)  );

        ghosts.push_back( (CBlinky( CCoords(13 , 11) ,  EGhostmode::SCATTER , EDirection::LEFT , 0) ).clone());
        ghosts.push_back( (CPinky( CCoords(13 , 14) ,  EGhostmode::IDLE , EDirection::UP , 0) ).clone());
        ghosts.push_back( (CInky( CCoords(11 , 14) ,  EGhostmode::IDLE , EDirection::UP, 30 )).clone());
        ghosts.push_back( (CClyde( CCoords(15 , 14) ,  EGhostmode::IDLE , EDirection::UP, 60 ) ).clone());

        setGhostSpeed( difficulty->getGhostSpeed(0) );

        pacDir = difficulty->getPacInitDir();


        return true;

    }

    /*!
       Getter for pacman's current position.
       \return pacman's current position.
     */
    CCoords getPacmanPos() const
    {

        return pacmanPos;

    }
/*!
       Getter for ghost's current position.
       \param[in] i
       : 0 - Blinky,
          1 - Pinky,
          2 - Inky,
          3 - Clyde.
     */
    CCoords getGhostPos( int i ) const
    {

        return ghosts[i]->getPos();

    }
/*!
       Setter for pacman's current direction.
       \param dir new pacman's direction.
     */
    void setPacDir( const EDirection & dir )
    {

        if(pacman.canStep(dir))
        pacDir = dir;

    }

/*!
       Getter for pacman's current direction.
       \return current pacman's direction.
     */
    static EDirection getPacDir();

/*!
       Getter for Blinky's current position.
       Uses by Inky to chase pacman.
       \return current Blinky's position.
     */
    static CCoords getBlinkyPos();

    /*!
       Method uses to update amount of
       current eaten bonuses.
     */
    static void updateEatenBonuses();

    /*!
       Getter for map tile symbol.
       \param[in] coords tile's coordinates.
       \return found tile symbol.
     */
    char getTileSym(  const CCoords & coords  ) const
    {

        return map.getTileSym( coords );

    }
/*!
       Getter for pacman's current lives.
       \return current pacman's direction.
     */
    int getPacLives() const
    {

        return pacmanLives;

    }
/*!
       Getter for current game level.
       \return current game level.
     */
    int getLevel() const
    {

        return difficulty->getLevel();

    }

    /*!
       Method uses to check , if game
       is custom.
       \return true if custom , else false.
     */
    bool isCustom() const
    {

        return difficulty->isCustom();

    }


private:

    static int score ; ///< game score
    static int eatenBonuses; ///< amount of eaten bonuses
    static int ghostMultiplier; ///< multiplier for ghosts's deaths

    static EDirection pacDir; ///< pacman current direction
    static std::vector< std::shared_ptr<CGhost> > ghosts; ///< vector stores all ghosts

    int pacmanLives = 3; ///< amount of pacman lives

    EGhostmode curMode; ///< current game mode

    bool run = false ; ///< game run status

    CGameDifficulty * difficulty; ///< game difficulty

    CTimer scareTimer , ///< FRIGHTENED game mode timer

    fruitTimer; ///< fruit existance timer

    CMaze map; ///< game map

    CPacman pacman; ///< Pacman

    CCoords pacmanPos; ///< Pacman current position


};

#endif