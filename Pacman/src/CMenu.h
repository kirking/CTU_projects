#ifndef C_MENU
#define C_MENU

#include "CGame.h"
#include "CMenuGraphics.h"
/*!
    \brief Game menu class.
     Controls menu input,
     menu logic and display
     menu to the screen.
     */
class CMenu
{


public:

    CMenu() = default;

    ~CMenu() = default;

    /*!
       Method uses to start menu.
       Create loop until game starts.
     */
    void start()
    {

        loadHighScore();

        startMenuGraphics();

        run = true;

        while( run )
        {

            graphics->update(menuButton);

            graphics->render();

            handleEvents();


        }


    }

    /*!
       Method uses to open menu after closed game window.
     */
    void unPause()
    {

        loadHighScore();

        run = true;


       startMenuGraphics();

    }

    /*!
       Method uses to control
       all keyboard inputs and events.
     */
    void handleEvents()
    {

        SDL_Event event = {};
        SDL_PollEvent(&event);

        if( event.type == SDL_KEYUP )
        {

            switch( event.key.keysym.sym)
            {

                case SDLK_SPACE:

                    if( menuButton == 0 ) //New Game
                    {

                        stop();

                        newGame();

                        unPause();

                    }
                    else if( menuButton == 1) //Load Game
                    {

                        stop();

                        loadGame();

                        unPause();


                    }
                    else if( menuButton == 2) //Custom Game
                    {

                    stop();

                    customGame();

                    unPause();

                    }
                    else if( menuButton == 3) //Exit
                    {

                     stop();

                    }
                    break;

                case SDLK_DOWN:
                  if(menuButton < 3 )  menuButton++; //Move selection down
                  else menuButton = 0;
                  break;

                case SDLK_UP:
                    if(menuButton > 0 )  menuButton--; //Move selection up
                    else menuButton = 3;
                    break;

                case SDLK_ESCAPE:
                    stop();
                    break;

                default: break;



            }


        }

        switch( event.type )
        {

            case SDL_QUIT:
                stop();
                break;

            default:
                break;


        }



    }
    /*!
        Method uses to stop menu.
        While game is running or after
        exit.
      */
    void stop()
    {

        run = false;

        delete graphics;

        graphics = nullptr;

    }

    /*!
       Method uses to initialise menu graphics.
     */
    void startMenuGraphics()
    {


        graphics = new CMenuGraphics();
        graphics->init( score );

    }
    /*!
       Method uses to load high score.
     */
    void loadHighScore()
    {

        std::string line;

        std::ifstream in("../poligkir/examples/highScore.txt");

        if (in.is_open())
        {
            getline(in, line);

           if( !line.empty() ) score = stoi(line);

        }
        else
            {

            std::cerr << "No High Score\n";

            score = 0;
        }

    }
/*!
       Method uses to load saved and start game .
       \return true if game successful loaded,
       false if not.
     */
    bool loadGame()
    {

        game = new CGame();

        if ( !game->loadGame("../poligkir/examples/saveFile.txt") )
        {

            delete game;

            return false;

        }

        game->startGame();

        delete game;

        game = nullptr;

        return true;

    }

    /*!
       Method uses to load  and start custom game .
       \return true if game successful loaded,
       false if not.
     */
    bool customGame()
    {

        game = new CGame();

        if ( !game->customGame("../poligkir/examples/customFile.txt") )
        {

            delete game;

            return false;

        }

        game->startGame();

        delete game;

        game = nullptr;

        return true;
    }

    /*!
       Method uses to start default game.
     */
    void newGame()
    {

        game = new CGame();

        game->defaultGame();

        game->startGame();

        delete game;

        game = nullptr;

    }


private:

    int menuButton = 0 , ///< number of selected button

    score = 0; ///< high score

    CGame * game = nullptr; ///< game

    CMenuGraphics * graphics = nullptr; ///< menu graphics

    bool run = false;/// run indicator
};

#endif