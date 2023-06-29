#ifndef C_GAME
#define C_GAME

#include "CLogic.h"
#include "CGraphics.h"

/*!
    \brief Game class.
     Controls game input,
     unite game logic and game graphics.
     */

class CGame
{

public:

    CGame() = default;

    ~CGame() = default;

/*!
       Method uses to load saved game .
       \param [in] filename name of saved game file.
       \return true if game successful loaded,
       false if not.
     */
    bool loadGame( const char * filename )
    {
        logic = new CLogic( );

           if( !logic->loadGame( filename ) )
        {
            return false;
        }

        graphics = new CGraphics(logic);

        graphics->init("Loaded Game" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 896 , 992 );

        return true;

    }

    /*!
       Method uses to save game.
       \param [in] filename name of save file.

     */
    void saveGame( const char * filename)
    {

        std::ofstream out(filename);

        if(out.is_open() )
        {

            out << logic->saveGame();

        }
        else
        {

            std::cerr<<" Save error\n";

            return;
        }

        out.close();

    }
/*!
       Method uses to load custom game .
       \param [in] filename name of custom game file.
       \return true if game successful loaded,
       false if not.
     */
    bool customGame( const char * filename )
    {
        logic = new CLogic();

        if( !logic->customGame( filename ) )
        {

            return false;

        }
        graphics = new CGraphics(logic);

        graphics->init("Custom Game" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 896 , 992 );

        return true;
    }

    /*!
       Method uses to create default game.
     */
    void defaultGame( )
    {
        logic = new CLogic();

        logic->defaultInit();

        graphics = new CGraphics(logic);

        graphics->init("Pacman" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 896 , 992 );
    }

    /*!
       Method uses to update game logic
       and graphics.

     */
    void update()
    {
        logic->update();

        graphics->update();
    }

    /*!
      Method uses to render game graphics.

    */
    void render()
    {
        graphics->render();
    }

    /*!
      Method uses to start game.
      Create game loop until
      game over.
    */
    void startGame()
    {
        logic->start();

        while( logic->isRunning() )
        {
            handleEvents();

            update();

            render();
        }

        graphics->clean();

        delete logic;
        delete graphics;

    }

    /*!
          Method uses to handle keyboard
          inputs and transfer them to game logic.

        */
    void handleEvents()
    {
        SDL_Event event = {};
        SDL_PollEvent(&event);

        if( event.type == SDL_KEYUP )
        {
            switch( event.key.keysym.sym)
            {                                  //Controlling pacman moves is possible , only when game unpaused.
                case SDLK_UP:
                    if(!paused)logic->setPacDir(EDirection::UP);
                    break;

                case SDLK_DOWN:
                    if(!paused)logic->setPacDir(EDirection::DOWN);
                    break;

                case SDLK_LEFT:
                    if(!paused)logic->setPacDir(EDirection::LEFT);
                    break;

                case SDLK_RIGHT:
                    if(!paused)logic->setPacDir(EDirection::RIGHT);
                    break;

                case SDLK_SPACE:
                    if(!paused) pause();
                    else unpause();
                    break;

                case SDLK_ESCAPE:
                    if(paused) unpause();
                    logic->endGame();
                    graphics->clean();
                    break;

                case SDLK_F5:   // Game won't save custom mode
                    if(!logic->isCustom())saveGame("../poligkir/examples/saveFile.txt");
                    break;

                    default: break;
            }
        }
        switch( event.type )
        {
            case SDL_QUIT:

                logic->endGame();
                graphics->clean();
                break;

            default:
                break;
        }
    }

    /*!
      Method uses to pause game.

    */
    void pause()
    {
        paused = true;

        while( paused )
        {
            handleEvents();
        }
    }
    /*!
          Method uses to unpause game.

        */
    void unpause()
    {
        paused = false;
    }

private:

    bool paused = false; ///<pause indicator.

    CLogic * logic; ///< pointer to game logic.

    CGraphics * graphics; ///< pointer to game graphics.

};

#endif