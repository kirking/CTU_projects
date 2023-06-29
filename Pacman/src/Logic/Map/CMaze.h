#ifndef C_MAZE
#define C_MAZE

#include <fstream>
#include "../../CCoords.h"
#include "CGate.h"
#include "CTunnel.h"
#include "CRoad.h"
#include "CWall.h"
#include "../Units/EGhostMode.h"
#include "../Bonuses/CEnergizer.h"
#include "../Bonuses/CFruit.h"

class CLogic;
/*!
	\brief Maze class.

	Represents maze in the game , contains all maze information.
*/
class CMaze
{
public:

    CMaze() = default;

    ~CMaze()
    {

        pacmanDied = false;

        frightened = false;

        gatesPos = CCoords(14 , 12);

    };
/*!
        Method uses for loading and configuring
        custom maze from a file .
        \param [in] filename custom file name.
        \return true if maze was loaded and
        successful configured , else false;
     */
    bool customGame( const char * filename )
    {

        std::string line;

        std::ifstream in(filename);

        if (in.is_open()) {


          //Skip file until Map: line
            while (line != "Map:")
            {
                getline(in, line);

                if(line.empty() )
                {
                    std::cerr << "ERROR: Empty File\n";
                    return false;
                }
            }

            try {

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                tunPos.xPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                tunPos.yPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                tunDir = EDirection(std::stoi(line));

                if( (int)tunDir == 0 )
                {

                    std::cerr << "Generation ERROR : incorrect tunnels direction\n" << std::endl;

                    return false;


                }

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                gatesPos.xPos = std::stoi(line);


                getline(in, line);

                line.erase(0, line.find('=') + 1);

                gatesPos.yPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                fruitPos.xPos = std::stoi(line);


                getline(in, line);

                line.erase(0, line.find('=') + 1);

                fruitPos.yPos = std::stoi(line);

            }
            catch(const std::invalid_argument& e) // If file has incorrect structure return false and write error
            {

                std::cerr << "Custom file read error : "<<e.what() << std::endl;

                return false;

            }
            //Check if fruit's position correct
            if( tunPos.xPos < 0 || tunPos.yPos < 0 || tunPos.xPos >= 28 || tunPos.yPos >= 31 )
            {

                std::cerr << "File ERROR : Tunnel value\n" << std::endl;

                return false;

            }
            if( fruitPos.xPos < 0 || fruitPos.yPos < 0 || fruitPos.xPos >= 28 || fruitPos.yPos >= 31 )
            {

                std::cerr << "File ERROR : Fruit value\n" << std::endl;

                return false;

            }
            //Save initial map
            for( int i = 0 ;  i < 28 ; ++i )
            {

                getline(in, line);

                for( int j = 0 ; j < 31 ; ++j )
                {

                    charMap[i][j] = line[j];

                }

            }
            //Generate map
            if ( !generateFromCharMap() )
            {

                return false;

            }

        }
        else
        {

            std::cerr << "Cannot open custom file \n";

            return false;

        }

        in.close();

        return true;

    }

/*!
        Method uses for loading and configuring
        saved maze from a file .
        \param[in] fileName  saved file name.
        \return true if maze was loaded and
        successful configured , else false;
     */
    bool loadMap( const char * fileName )
    {

        std::string line;

        std::ifstream in(fileName);

        if (in.is_open())
        {

            //Skip file until Map: line
            while( line !="Map:")
            {
                getline(in, line);

                if(line.empty() )
                {
                    std::cerr << "ERROR: Empty File\n";
                    return false;
                }
            }

            try {

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                tunPos.xPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                tunPos.yPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                tunDir = EDirection(std::stoi(line));

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                gatesPos.xPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                gatesPos.yPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                fruitPoints = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                fruitPos.xPos = std::stoi(line);

                getline(in, line);

                line.erase(0, line.find('=') + 1);

                fruitPos.yPos = std::stoi(line);

            }
            catch (const std::invalid_argument& e) // If file has incorrect structure return false and write error
            {

                std::cerr << "Custom file read error : "<<e.what() << std::endl;

                return false;

            }
           //Save initial map
            for( int i = 0 ;  i < 28 ; ++i )
            {

                getline(in, line);

                for( int j = 0 ; j < 31 ; ++j )
                {

                    charMap[i][j] = line[j];

                }

            }
            getline(in, line);
           //Generate current map
            for( int x = 0 ;  x < 28 ; ++x )
            {

                getline(in, line);

                for( int y = 0 ; y < 31 ; ++y )
                {

                            if( line[y] == 'G')
                            {
                                setTile(CCoords( x , y ), CGate());

                            }
                            else if( line[y] == 'g')
                            {

                                CGate tmp = CGate();

                                tmp.openGate();

                                setTile(CCoords( x , y ), tmp);


                            }
                            else if( line[y] == 'T')
                            {

                                setTile( CCoords(x , y ) , CTunnel( CCoords(x , y ) , tunDir ,  tunPos ));

                                tunPos = CCoords( x , y );

                                tunDir = EDirection ( -(int)tunDir );



                            }
                            else if( line[y] == 'o' )
                            {

                                CRoad road;
                                road.setBonus(CBonus(10).clone());
                                setTile(CCoords(x , y ), road);

                            }
                            else if( line[y] == 'O'  )
                            {

                                CRoad road;
                                road.setBonus(CEnergizer(50).clone());
                                setTile(CCoords(x , y ), road);

                            }
                            else if( line[y] == 'r'  )
                            {
                                CRoad road;
                                setTile(CCoords(x , y ), road);

                            }
                            else if( line[y] == 'F' )
                            {

                                CRoad road;
                                road.setBonus(CFruit(fruitPoints).clone());
                                setTile(CCoords(x , y ), road);

                            }
                            else if( charMap[x][y] == 'W' )
                            {

                                setTile(CCoords(x , y ), CWall());

                            }
                            else
                            {
                                std::cerr << "Generation ERROR : cannot resolve symbol\n" << std::endl;

                                return false;

                            }

                }

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
        Method uses for save maze configuration.
        \return string which contains all maze
        configuration.
     */
    std::string saveMap()
    {
        //Create string save of all map information.
        std::string mapString = "Map:\ntunPos.x =" + std::to_string(tunPos.xPos) +
                "\ntunPos.y =" + std::to_string(tunPos.yPos) +
                "\ntunDir =" + std::to_string((int)tunDir) +
                "\ngatesPos.x =" + std::to_string(gatesPos.xPos)+
                "\ngatesPos.y =" + std::to_string(gatesPos.yPos)+"\nfruitPoints ="
                + std::to_string(fruitPoints) +
                "\nFruitPos.x =" + std::to_string(fruitPos.xPos)+
                "\nFruitPos.y =" + std::to_string(fruitPos.yPos) + "\n";

          for(auto & i : charMap)
          {

              for(char j : i)
              {

                  mapString += j;

              }

              mapString += "\n";

          }

          mapString += "saveMap\n";

        for( int i = 0 ; i < 28 ; ++ i )
        {

           for( int j = 0 ; j < 31 ; ++ j )
           {

               mapString += maze[i][j]->getSym();

           }

            mapString += "\n";

        }


        return mapString;

    }
/*!
        Getter for tile in the maze.
        \param[in] coords tile coordinates.
        \return Found tile.
     */
    static CTile & getTile( const CCoords & coords )
    {
           if( coords.xPos >= 28 || coords.xPos < 0 || coords.yPos >= 31 ||coords.yPos < 0 )
           {

               std::cerr << "MAZE ERROR\n";

           }
        return *( maze[coords.xPos][coords.yPos].get());

    }

    /*!
        Getter for tile symbol in the maze.
        \param[in] coords tile coordinates.
        \return Found tile's symbol.
     */
    char getTileSym( const CCoords & coords ) const
    {

        return getTile(coords).getSym();

    }

/*!
        Method uses to set tile in the maze.
        \param[in] coords tile coordinates.
        \param[in] tile tile to set.
     */
    void setTile( const CCoords & coords  , const CTile & tile )
    {

        maze[coords.xPos][coords.yPos] = tile.clone();

    }

/*!
        Method uses to generate default maze configuration.

     */
    void generateDefault()
    {

        fruitPos = CCoords( 13 , 17 );

        tunPos =  CCoords(27 , 14);

        tunDir = EDirection::LEFT;

        char stndMap[28][31] =  {

                 {'W','W','W','W','W','W','W','W','W','W','r','r','r','W','T','W','r','r','r','W','W','W','W','W','W','W','W','W','W','W','W'}
                ,{'W','o','o','O','o','o','o','o','o','W','r','r','r','W','r','W','r','r','r','W','o','o','o','O','W','W','o','o','o','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','r','r','r','W','r','W','r','r','r','W','o','W','W','o','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','r','r','r','W','r','W','r','r','r','W','o','W','W','o','o','o','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','r','r','r','W','r','W','r','r','r','W','o','W','W','W','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','W','W','W','W','r','W','W','W','W','W','o','W','W','W','W','W','o','W','W','o','W'}
                ,{'W','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','W','W','W','W','W','W','r','W','W','W','W','W','o','W','W','o','W','W','W','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','W','W','W','W','W','W','r','W','W','W','W','W','o','W','W','o','W','W','W','W','W','o','W'}
                ,{'W','o','W','W','W','o','o','o','o','W','W','r','r','r','r','r','r','r','r','r','o','W','W','o','o','o','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','W','r','W','W','W','W','W','r','W','W','o','W','W','o','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','W','r','W','r','r','r','W','r','W','W','o','W','W','o','W','W','o','W','W','o','W'}
                ,{'W','o','o','o','o','o','W','W','o','r','r','r','W','r','r','r','W','r','W','W','o','o','o','o','W','W','o','o','o','o','W'}
                ,{'W','W','W','W','W','o','W','W','W','W','W','r','G','r','r','r','W','r','W','W','W','W','W','r','W','W','W','W','W','o','W'}
                ,{'W','W','W','W','W','o','W','W','W','W','W','r','G','r','r','r','W','r','W','W','W','W','W','r','W','W','W','W','W','o','W'}
                ,{'W','o','o','o','o','o','W','W','o','r','r','r','W','r','r','r','W','r','W','W','o','o','o','o','W','W','o','o','o','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','W','r','W','r','r','r','W','r','W','W','o','W','W','o','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','W','r','W','W','W','W','W','r','W','W','o','W','W','o','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','o','o','o','W','W','r','r','r','r','r','r','r','r','r','o','W','W','o','o','o','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','W','W','W','W','W','W','r','W','W','W','W','W','o','W','W','o','W','W','W','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','W','W','W','W','W','W','r','W','W','W','W','W','o','W','W','o','W','W','W','W','W','o','W'}
                ,{'W','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','W','W','W','W','r','W','W','W','W','W','o','W','W','W','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','r','r','r','W','r','W','r','r','r','W','o','W','W','W','W','W','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','r','r','r','W','r','W','r','r','r','W','o','W','W','o','o','o','o','W','W','o','W'}
                ,{'W','o','W','W','W','o','W','W','o','W','r','r','r','W','r','W','r','r','r','W','o','W','W','o','W','W','o','W','W','o','W'}
                ,{'W','o','o','O','o','o','o','o','o','W','r','r','r','W','r','W','r','r','r','W','o','o','o','O','W','W','o','o','o','o','W'}
                ,{'W','W','W','W','W','W','W','W','W','W','r','r','r','W','T','W','r','r','r','W','W','W','W','W','W','W','W','W','W','W','W'}
        };


        for( int x = 0 ; x < 28 ; ++x )
        {

            for( int y = 0 ; y < 31 ; ++y )
            {

                charMap[x][y] = stndMap[x][y];

            }

        }

        generateFromCharMap();


    }

    /*!
        Method uses to generate maze from
        initial maze configuration.
        \return true if map succesful generated ,
        else false.
     */
    bool generateFromCharMap()
    {


        for( int x = 0 ; x < 28 ; ++x )
        {

            for( int y = 0 ; y < 31 ; ++y )
            {

                if( charMap[x][y] == 'G')
                {

                    setTile(CCoords( x , y ), CGate());

                    setGatePos( CCoords( x , y ) );



                }
                else if( charMap[x][y] == 'T')
                {

                    setTile( CCoords(x , y ) , CTunnel( CCoords(x , y ) , tunDir ,  tunPos ));

                    tunPos = CCoords( x , y );

                    tunDir = EDirection ( -(int)tunDir );

                }
                else if( charMap[x][y] == 'o' )
                {

                    CRoad road;
                    road.setBonus(CBonus(10).clone());
                    setTile(CCoords(x , y ), road);

                }
                else if( charMap[x][y] == 'O'  )
                {

                    CRoad road;
                    road.setBonus(CEnergizer(50).clone());
                    setTile(CCoords(x , y ), road);

                }
                else if( charMap[x][y] == 'r'  )
                {
                    CRoad road;
                    setTile(CCoords(x , y ), road);

                }
                else if( charMap[x][y] == 'F' )
                {

                    CRoad road;
                    road.setBonus(CEnergizer(50).clone());
                    setTile(CCoords(x , y ), road);

                }
                else if( charMap[x][y] == 'W' )
                {

                    setTile(CCoords(x , y ), CWall());

                }
                else
                {
                        std::cerr << "Generation ERROR : cannot resolve symbol\n" << std::endl;

                        return false;

                }

            }

        }

        return true;

    }

    /*!
        Method uses to set fruit in the maze.
        \param[in] points amount of points which fruit will contain.
     */
    void setFruit( int points )
    {

            dynamic_cast< CRoad & >( getTile( fruitPos  )).setBonus( CFruit( points ).clone());

            fruitPoints = points;

    }
/*!
        Method uses to remove fruit from the maze.
     */
    void removeFruit()
    {

        dynamic_cast< CRoad & >( getTile( fruitPos  )).removeBonus();

    }

    /*!
        Method uses to check if ghosts must be in FRIGHTENED mode.
        \return true if must , false if not.
     */
    bool areScared() const
    {

        return frightened;

    }

    /*!
        Method uses to cancel FRIGHTENED mode trigger.
     */
    void calmDown()
    {

        frightened = false;

    }
/*!
        Method uses to add eaten bonuses to the game.
     */
    static void addEatenBonus();
/*!
        Method uses to add points to the game score.
        \param[in] nPoints amount of points to add.
     */
    static void addPoints( int nPoints ) ;

    /*!
        Method uses to activate FRIGHTENED trigger.
        \param[in] mode depends on mode activate trigger.
     */
    static void changeGhostState( const EGhostmode & mode ) ;

/*!
        Method uses to change pacman's death indicator to true.
     */
    static void killPacman()
    {

        pacmanDied = true;

    }
/*!
        Method uses to change pacman's death indicator to false.
     */
    void revivePacman()
    {

        pacmanDied = false;

    }
/*!
        Getter for pacman's death indicator.
     */
    bool pacmanStat() const
    {

        return  pacmanDied;

    }
/*!
        Setter for gate position in the maze.
        \param[in] coords gate's coordinates.
     */
    void setGatePos( const CCoords & coords  )
    {

        gatesPos = coords;

    }
/*!
        Method uses to open the gate in the maze.
     */
    void openTheGate()
    {

        dynamic_cast< CGate & >(getTile( gatesPos ) ).openGate();

        dynamic_cast< CGate & >(getTile( CCoords( gatesPos.xPos - 1 , gatesPos.yPos ) )) .openGate();

    }
/*!
        Method uses to close the gate in the maze.
     */
    void closeTheGate()
    {

        dynamic_cast< CGate & >(getTile( gatesPos )).closeGate();

        dynamic_cast< CGate & >(getTile( CCoords( gatesPos.xPos - 1 , gatesPos.yPos )) ).closeGate();



    }

    /*!
        Method uses to regenerate maze after change level.
     */
    void regenerateMaze()
    {

        calmDown();

        tunPos = dynamic_cast<CTunnel &>( getTile(tunPos) ).getSecPos();

        tunDir = dynamic_cast<CTunnel &>( getTile(tunPos) ).getDir();

        generateFromCharMap();

    }

    /*!
        Method uses to add score points for eaten ghost to the game score.
        \param [in] nPoints amount of points.
     */
    static void addGhostPoints( int nPoints );

/*!
        Getter for tunnel position in the maze.
        \return tunnel position.
     */
    CCoords getTunnelPos()
    {

        return tunPos;

    }
/*!
        Getter for tunnel position in the maze.
        \return tunnel position.
     */
    static CCoords getGatesPos();



private:

    static std::shared_ptr<CTile> maze[28][31]; ///< maze array of tiles.

    char charMap[28][31]; ///< initial maze configuration.

    int fruitPoints = 0; ///< fruit points for fruit bonus.

    CCoords  tunPos , ///< tunnel position in the maze.
    fruitPos ;        ///< fruit position in the maze.

    EDirection tunDir = EDirection::IDLE; ///< tunnel entrance direction.

    static bool frightened; ///< FRIGHTENED game mode trigger.

    static bool pacmanDied; ///< pacman's death indicator.

    static CCoords gatesPos;  ///< gate's position in the maze.
};

#endif