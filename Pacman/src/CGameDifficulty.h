#ifndef C_GAME_DIFFICULTY
#define C_GAME_DIFFICULTY

#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include "EDirection.h"
#include "CTimer.h"
/*!
    \brief Game difficulty class.

    Contains game configuration ,
    also controls game timings and
    units's speed.
     */
class CGameDifficulty
{

public:

    /*!
       Constructor for Game difficulty
       Uses defaultGame() method.

    */
    CGameDifficulty()
    {

        defaultGame();

    }

    /*!
       Method uses to load and configure
       custom game difficulty.
       \param [in] filename name of custom file.
       \return true if difficulty successful loaded,
       false if not.
     */
    bool customGame( const char * filename )
    {

        std::string line;
        std::ifstream in(filename);

        if (in.is_open())
        {
            try {
                        //Creating order to load values from file
                std::queue<int*> intQueue;
                intQueue.push(&fruitRate);
                intQueue.push(&scatterTiming);
                intQueue.push(&chaseTiming);
                intQueue.push(&scareTime);
                intQueue.push(&eatenBonusesToWin);
                intQueue.push(&levelToStopFright);
                intQueue.push(&chaseGain);
                intQueue.push(&pacmanLives);

                std::queue<float*> floatQueue;
                floatQueue.push(&defaultSpeed);
                floatQueue.push(&defaultSpeedGain);
                floatQueue.push(&tunnelSpeed);
                floatQueue.push(&tunnelSpeedGain);
                floatQueue.push(&frightSpeed);
                floatQueue.push(&frightSpeedGain);
                floatQueue.push(&pacmanSpeed);
                floatQueue.push(&pacmanSpeedGain);
                floatQueue.push(&pacmanFrightSpeed);
                floatQueue.push(&pacmanFrightSpeedGain);

                getline(in, line);

                while( !intQueue.empty() )
                {
                    getline(in, line);

                    if(line.empty() ) // Incorrect file structure control
                    {
                        std::cerr << "ERROR: Empty File\n";
                        return false;
                    }
                    line.erase(0, line.find('=') + 1);

                    if( std::stoi(line) < 0 )
                    {
                        std::cerr << "File ERROR : Negative value\n";

                        return false;
                    }


                    *(intQueue.front()) = std::stoi(line);

                    intQueue.pop();
                }

                getline(in, line);
                line.erase(0, line.find('=') + 1);
                pacInitDir = EDirection ( std::stoi(line) );

                while( !floatQueue.empty() )
                {
                    getline(in, line);
                    line.erase(0, line.find('=') + 1);
                    if(line.empty() )
                    {
                        std::cerr << "ERROR: Empty File\n";
                        return false;
                    }

                    if( std::stof(line) < 0 )   //Negative value control
                    {
                        std::cerr << "File ERROR : Negative value\n";
                        return false;
                    }

                    *(floatQueue.front()) = std::stof(line);

                    floatQueue.pop();
                }

            } catch (const std::invalid_argument& e)
            {
                std::cerr << "Custom file read error : "<<e.what() << std::endl;
                return false;
            }
            custom = true;
        }

        in.close();
        return true;
    }
/*!
     Method checks , if it's time to change
     game mode to SCATTER.
     \param [in] afterScare flag , that controls
     change mode cycle , if SCATTER comes
     after FRIGHTENED game mode then change
     cycle won't increment.
     \return true if it's time to change mode ,
	 false if not.
     */
    bool timeToScatter( bool afterScare )
    {
        if( !stopScatter ) {
            if (!scatter && modeTimer.checkTime()) {

                modeTimer.setTimer(scatterTiming); //Set timer for scatter mode
                scatter = true;
                if(!afterScare)changeCycle++; //Increment change mode cycles only after chase mode
                if( custom ) customTimings();
            }
            if (scatter && modeTimer.checkTime())
            {
                modeTimer.stopTimer();
                scatter = false;
            }
            return scatter;
        }
        else return false;
    }
/*!
     Method checks , if it's time to change
     game mode to CHASE.
     \return true if it's time to change mode ,
	 false if not.
     */
    bool timeToChase()
    {
        if( stopScatter ) return true;

        if( !scatter && modeTimer.checkTime() )
        {
            modeTimer.setTimer( chaseTiming );

            scatter = false;
        }
        if( !scatter && modeTimer.checkTime() )
        {
            modeTimer.stopTimer();
            scatter = true;
        }
        return !scatter;
    }

    /*!
       Method checks if it's time to spawn
       bonus fruit.
       \param [in] eatenBonuses eaten bonuses ,
       fruit spawn depends on this bonuses.
       \return true if it's time , false if not.

     */
    bool timeToSpawnFruit( int eatenBonuses )
    {
        if( eatenBonuses == fruitRate * 10 && fruitRate!= 0  ) //Controls fruit rate
        {
            if( fruitRate < 8 )
            {
                fruitRate++;
            }
            return true;
        }
        else return false;
    }

    /*!
       Getter for points , which
       will be set to bonus fruit.
       Value depends on current level.
       \return fruit points.
     */
    int fruitPoints() const
    {
        return level * 200 + 50;
    }

    /*!
       Method uses to load and configure
       saved game difficulty.
       \param[in] filename name of save file.
       \return true if difficulty successful loaded,
       false if not.
     */
    bool loadFromFile( const char * filename )
    {
        std::string line;
        std::ifstream in(filename);

        if (in.is_open())
        {
            try {
                while (line != "difficulty parametrs:") {

                    getline(in, line);

                    if(line.empty() )
                    {
                        std::cerr << "ERROR: Empty File\n";
                        return false;
                    }

                }
                      //Create order to loading values
                std::queue<int*> intQueue;
                intQueue.push(&fruitRate);
                intQueue.push(&scatterTiming);
                intQueue.push(&chaseTiming);
                intQueue.push(&scareTime);
                intQueue.push(&level);
                intQueue.push(&changeCycle);

                std::queue<float*> floatQueue;
                floatQueue.push(&defaultSpeed);
                floatQueue.push(&tunnelSpeed);
                floatQueue.push(&frightSpeed);
                floatQueue.push(&pacmanSpeed);
                floatQueue.push(&pacmanFrightSpeed);

                while( !intQueue.empty() )
                {
                    getline(in, line);

                    if(line.empty() ) // Incorrect file structure control
                    {
                        std::cerr << "ERROR: Empty File\n";
                        return false;
                    }
                    line.erase(0, line.find('=') + 1);

                    if( std::stoi(line) < 0 ) //Negative value control
                    {
                        std::cerr << "File ERROR : Negative value\n";

                        return false;
                    }

                    *(intQueue.front()) = std::stoi(line);

                    intQueue.pop();
                }

                getline(in, line);
                line.erase(0, line.find('=') + 1);
                pacInitDir = EDirection ( std::stoi(line) );

                while( !floatQueue.empty() )
                {
                    getline(in, line);
                    line.erase(0, line.find('=') + 1);

                    if( std::stof(line) < 0 )
                    {
                        std::cerr << "File ERROR : Negative value\n";
                        return false;
                    }

                    *(floatQueue.front()) = std::stof(line);

                    floatQueue.pop();
                }

                getline(in, line);
                line.erase(0, line.find('=') + 1);
                scatter = std::stoi(line);

                getline(in, line);
                line.erase(0, line.find('=') + 1);
                stopScatter = std::stoi(line);


            }catch (const std::invalid_argument& e)
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
       Method uses to set default game
       difficulty configuration.
     */
    void defaultGame()
    {

        pacInitDir = EDirection::LEFT;
        fruitRate = 5;
        defaultSpeed = 0.45;
        tunnelSpeed = 0.3;
        frightSpeed = 0.2;
        pacmanSpeed = 0.8;
        pacmanFrightSpeed = 0.9;
        pacmanLives = 3;
        scatterTiming = 7000;
        chaseTiming = 20000;
        scareTime = 8000;
        chaseGain = 0;
        level = 1;
        levelToStopFright = 21;
        changeCycle = 0;
        pacmanSpeedGain  =
                defaultSpeedGain =  0.10;
        pacmanFrightSpeedGain =
                tunnelSpeedGain =
                        frightSpeedGain = 0.05;

    }

    /*!
       Getter for pacman's initial direction.
       \return pacman's initial direction.
     */
    EDirection getPacInitDir() const
    {
        return pacInitDir;
    }

    /*!
       Getter for FRIGHTENED game mode duration
       \return FRIGHTENED game mode duration
     */
    unsigned int getScareTime() const
    {
        return scareTime;
    }

    /*!
       Method uses to control units's speed.
       Speed changes depends on current level.
     */
    void changeSpeed()
    {
       if( level == 2 || level == 5 )
       {

           defaultSpeed += defaultSpeedGain;
           tunnelSpeed += tunnelSpeedGain;
           frightSpeed += frightSpeedGain;
           pacmanSpeed += pacmanSpeedGain;
           pacmanFrightSpeed += pacmanFrightSpeedGain;

       }
       else if( level == 21 )
       {
           pacmanSpeed -= 0.10;
       }
    }

    /*!
       Method uses to save current game difficulty
       state and configuration.
       \return string which contains difficulty configuration.
     */
    std::string saveDifficulty()
    {
        std::string saveFile = "difficulty parametrs:\nfruitRate ="
                +std::to_string(fruitRate) +
                "\nscatterTiming =" + std::to_string(scatterTiming) +
                "\nchaseTiming =" + std::to_string(chaseTiming) +
                "\nscareTime =" + std::to_string(scareTime) +
                "\nlevel =" + std::to_string(level) +
                "\nchangeCycle =" + std::to_string(changeCycle) +
                "\npacInitDir =" + std::to_string((int)pacInitDir) +
                "\ndefaultSpeed =" + std::to_string(defaultSpeed) +
                "\ntunnelSpeed =" + std::to_string(tunnelSpeed) +
                "\nfrightSpeed =" + std::to_string(frightSpeed) +
                "\npacmanSpeed =" + std::to_string(pacmanSpeed) +
                "\npacmanFrightSpeed =" + std::to_string(pacmanFrightSpeed) +
                "\nscatter =" + std::to_string(scatter) +
                "\nstopScatter =" + std::to_string(stopScatter) +"\n";

        return saveFile;
    }

    /*!
       Method uses to pass game difficulty to the next level.
       Change modes timings , resets change cycle and
       use method changeSpeed() to change units's speed.
     */
    void nextLevel()
    {

        if( custom )
        {
            scatterTiming += changeCycle * chaseGain * level;
            chaseTiming -= changeCycle * chaseGain;
        } else
            {

            scatterTiming = 5000;

            chaseTiming = 20000;

            }
        changeCycle = 0 ;
        scatter = false;
        stopScatter = false;
        level++;
        modeTimer.stopTimer();
        changeSpeed();

    }

    /*!
       Method uses to get ghost's speed.
       \param[in] i speed flag
       \return i== 1 ghost speed in FRIGHTENED mode,
               i== 2 ghost speed after tunnel,
       in another case default ghost speed , which depends
       on level .
     */
    float getGhostSpeed( unsigned int i ) const
    {
        switch (i)
        {
            //Frightened
            case 1:
                return frightSpeed;

                //tunnel
            case 2:
                return tunnelSpeed;

                //default
            default:
                return defaultSpeed;

        }

    }

    /*!
        Method uses to change game timings in custom game.
      */
    void customTimings()
    {
        if( scatterTiming > 0 )
        {
            scatterTiming -= chaseGain * level ;
        }
        if( scatterTiming <= 0 ) stopScatter = true;

        chaseTiming += chaseGain;

    }

    /*!
       Method uses to change game timings in default game.
     */
    void changeTimings()
    {

        if(!custom ) {
            switch (level) {
                case 1:
                    if (changeCycle == 2) {
                        scatterTiming = 5000;

                    } else if (changeCycle == 4) {
                        stopScatter = true;
                    }
                    break;

                case 2:
                    if (changeCycle == 2) {
                        scatterTiming = 5000;
                        chaseTiming = 1033000;
                        break;

                    } else if (changeCycle == 3) {
                        scatterTiming = 16;
                        changeCycle++;
                        break;

                    } else if (changeCycle == 4) {
                        stopScatter = true;
                        break;
                    }
                    break;

                case 3:
                    if (changeCycle == 2) {
                        scatterTiming = 5000;
                        chaseTiming = 1033000;
                        break;

                    } else if (changeCycle == 3) {
                        scatterTiming = 16;
                        changeCycle++;
                        break;

                    } else if (changeCycle == 4) {
                        stopScatter = true;
                        break;
                    }
                    break;

                case 4:
                    if (changeCycle == 2) {
                        scatterTiming = 5000;
                        chaseTiming = 1033000;
                        break;

                    } else if (changeCycle == 3) {
                        scatterTiming = 16;
                        changeCycle++;
                        break;

                    } else if (changeCycle == 4) {
                        stopScatter = true;
                        break;
                    }
                    break;

                default:
                    if (changeCycle == 3) {
                        chaseTiming = 1037000;
                        changeCycle++;
                        break;

                    } else if (changeCycle == 4) {
                        scatterTiming = 16;
                        changeCycle++;
                        break;

                    } else if (changeCycle == 5) {
                        stopScatter = true;
                        break;
                    }
                    break;
            }
        }
    }

    /*!
         Method uses to check if game mode can be switched to
         FRIGHTENED on current level.
       */
    bool canFright() const
    {
        return level < levelToStopFright;
    }

    /*!
       Getter for current game level.
     */
    int getLevel() const
    {
        return level;
    }

    /*!
       Method uses to get pacman's speed.
       \param[in] i speed flag.
       \return i== 1 pacman speed in FRIGHTENED mode,
       in another case default ghost speed , which depends
       on level.
     */
    float getPacmanSpeed(unsigned int i ) const
    {
        switch (i)
        {
            case 1:
                return pacmanFrightSpeed;
            default:
                return pacmanSpeed;
        }
    }

    /*!
       Method checks if current eaten bonuses
       enough to set next level.
       \param [in] bonuses current eaten bonuses.
       \return true if enough , false if not.

     */
    bool checkNextLevel( int bonuses ) const
    {
        return eatenBonusesToWin == bonuses;
    }
/*!
       Getter for pacman's initial lives.
       \return pacman's initial lives.

     */
    int getPacmanLives() const
    {
        return pacmanLives;
    }

    /*!
       Method checks if game is custom.
       \return true if game is custom ,
       false if isn't.

     */
    bool isCustom() const
    {
        return custom;
    }

private:

    int    fruitRate = 0,  ///< fruit rate configurtion
            pacmanLives = 0, ///< pacman lives configurtion
            level = 1, ///< level configurtion
            chaseTiming = 0, ///< chase mode timing configurtion
            scatterTiming = 0, ///< scatter mode  timing configurtion
            changeCycle = 0, ///< change modes cycle configurtion
            levelToStopFright = 21, ///< level , from which ghosts won't change mode to FRIGHTENED
            eatenBonusesToWin = 244, ///< bonuses to win configurtion
            chaseGain = 0, ///< chase timing gain configurtion
            scareTime = 0; ///< scatter timing gain configurtion

    float frightSpeed = 0, ///< ghosts's speed in FRIGHTENED mode configurtion
          tunnelSpeed = 0, ///< ghosts's speed after tunnel configurtion
          defaultSpeed = 0, ///< ghosts's default speed configurtion
          pacmanSpeed = 0, ///< pacman's default speed configurtion
          pacmanFrightSpeed = 0,  ///< pacman's speed in FRIGHTENED game mode configurtion
          defaultSpeedGain = 0, ///< ghosts's default speed gain configurtion
          frightSpeedGain = 0, ///< ghosts's speed gain in FRIGHTENED mode configurtion
          tunnelSpeedGain = 0, ///< ghosts's speed gain after tunnel configurtion
          pacmanSpeedGain = 0,  ///< pacman's default speed gain onfigurtion
          pacmanFrightSpeedGain = 0; ///< pacman's speed gain in FRIGHTENED game mode configurtion

    EDirection pacInitDir; ///< pacman's initial direction configuration

    CTimer modeTimer; ///< scareTimer for game modes.

    bool scatter = false; ///< flag that indicates , if current game mode is scatter.
    bool stopScatter = false; ///< flag that indicates ability to change game mode to SCATTER.
    bool custom = false; ///< flag that indicates , if current game is custom.
};

#endif