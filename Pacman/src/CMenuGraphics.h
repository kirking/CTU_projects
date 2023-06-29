#ifndef C_MENU_GRAPHICS
#define C_MENU_GRAPHICS

#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
/*!
    \brief Game menu graphics class.
     Render and display game menu.
     */
class CMenuGraphics
{

public:

    CMenuGraphics() = default;

    /*!
       Destructor for Menu graphics.

       Uses clean() method.
    */
    ~CMenuGraphics()
    {

        clean();

    };

/*!
       Method uses to initialise graphics.
       Create renderer , window , textures ,
       count highscore.
       \param[in] score high score to display.
     */
    void init( int score )
    {

        SDL_Init( SDL_INIT_EVERYTHING);

        window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 896, 992,
                                              SDL_WINDOW_SHOWN);

        renderer = SDL_CreateRenderer( window , -1, 0);

        SDL_SetRenderDrawColor( renderer , 18 , 6, 138 , 255);

        //Initialise score
        int tmp = score;

        while (tmp) {

            scoreVector.push_back(tmp % 10);

            tmp /= 10;

        }

        SDL_Surface * tmpSurface = nullptr;

        //Load numerals textures
        for( int i = 0 ; i < 10 ; ++i )
        {

            std::string tmpString = "../poligkir/examples/Assets/n";

            tmpString += (std::to_string(i) + ".png");

            tmpSurface = IMG_Load( tmpString.c_str() );

            scoreTex[i] = SDL_CreateTextureFromSurface( renderer , tmpSurface);

            SDL_FreeSurface(tmpSurface);

        }
        //Texture rects initialisation
        scoreRect.x = 230;

        scoreRect.y = 700;

        scoreRect.h = 200;

        scoreRect.w = 300;

        titleRect.x = 300;

        titleRect.y = 50;

        titleRect.h = 200;

        titleRect.w = 300;

        newGameRect.x = 300;

        newGameRect.y = 200;

        newGameRect.h = 200;

        newGameRect.w = 300;

        loadRect.x = 300;

        loadRect.y = 300;

        loadRect.h = 200;

        loadRect.w = 300;

        customRect.x = 300;

        customRect.y = 400;

        customRect.h = 200;

        customRect.w = 300;

        exitRect.x = 300;

        exitRect.y = 500;

        exitRect.h = 200;

        exitRect.w = 300;

        //Textures initialisation

        tmpSurface = IMG_Load("../poligkir/examples/Assets/highScore.png");

        highScore = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/title.png");

        title = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/newGame.png");

        newGame = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/loadGame.png");

        loadGame = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/customGame.png");

        customGame = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/exit.png");

        exit = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/newGameSelected.png");

        newGameSelected = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/loadGameSelected.png");

        loadGameSelected = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/customGameSelected.png");

        customGameSelected = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

        tmpSurface = IMG_Load("../poligkir/examples/Assets/exitSelected.png");

        exitSelected = SDL_CreateTextureFromSurface( renderer , tmpSurface);

        SDL_FreeSurface( tmpSurface );

    }

    /*!
       Method uses update selected button in menu.
     */
    void update( int button )
    {

        selection = button;


    }

/*!
       Method uses to render and display
       menu.
     */
    void render()
    {
        SDL_RenderClear(renderer);

        numRect.h = 30;

        numRect.w = 32;

        numRect.y = 771;

        numRect.x = 500;

       //Render textures
        SDL_RenderCopy(renderer, highScore , nullptr, &scoreRect);

        SDL_RenderCopy(renderer, title, nullptr, &titleRect );

        if (selection == 0) {

            SDL_RenderCopy(renderer, newGameSelected, nullptr, &newGameRect);

        } else {

            SDL_RenderCopy(renderer, newGame, nullptr, &newGameRect);

        }

        if( selection == 1 )
        {

            SDL_RenderCopy(renderer, loadGameSelected, nullptr, &loadRect );

        }
        else
            SDL_RenderCopy(renderer, loadGame, nullptr, &loadRect );

        if( selection == 2 )
        {

            SDL_RenderCopy(renderer, customGameSelected, nullptr, &customRect );

        }
        else SDL_RenderCopy(renderer, customGame, nullptr, &customRect );

        if( selection == 3 )
        {

            SDL_RenderCopy(renderer, exitSelected, nullptr, &exitRect );

        }
        else SDL_RenderCopy(renderer, exit, nullptr, &exitRect );

        //Render highScore
        for( int i = (int)scoreVector.size() - 1; i >= 0 ; -- i )
        {

            numRect.x += 32;

            SDL_RenderCopy(renderer, scoreTex[scoreVector[i]], nullptr, &numRect);

        }

        SDL_RenderPresent(renderer);

    }

    /*!
       Method uses to delete all textures ,
       renderer and window.
     */
    void clean()
    {

        SDL_RenderClear(renderer);

        for (SDL_Texture * i : scoreTex)
        {

            SDL_DestroyTexture(i);


        }

        SDL_DestroyTexture(title);

        SDL_DestroyTexture(newGame);

        SDL_DestroyTexture(loadGame);

        SDL_DestroyTexture(customGame);

        SDL_DestroyTexture(exit);

        SDL_DestroyTexture(newGameSelected);

        SDL_DestroyTexture(loadGameSelected);

        SDL_DestroyTexture(customGameSelected);

        SDL_DestroyTexture(exitSelected);

        scoreVector.clear();

        SDL_DestroyWindow( window );

        SDL_DestroyRenderer( renderer );

        SDL_Quit();
    }




private:

    int selection = 0; ///< number of selcted button in menu

    std::vector<int> scoreVector; ///< high score as numerals

    SDL_Texture * highScore = nullptr; ///< high score title texture

    SDL_Texture * scoreTex[10]; ///< numerals textures

    SDL_Window * window = nullptr; ///< window

    SDL_Renderer * renderer = nullptr; ///< renderer

    SDL_Texture * title = nullptr; ///< titles texture

    SDL_Texture * newGame = nullptr; ///< new game button texture

    SDL_Texture * loadGame = nullptr; ///< load game button texture

    SDL_Texture * customGame = nullptr; ///< custom game button texture

    SDL_Texture * exit = nullptr; ///< exit button texture

    SDL_Texture * newGameSelected = nullptr; ///< new game selected button texture

    SDL_Texture * loadGameSelected = nullptr; ///< load game selected button texture

    SDL_Texture * customGameSelected = nullptr; ///< custom game selected button texture

    SDL_Texture * exitSelected = nullptr; ///< exit selected button texture

    SDL_Rect titleRect , ///< title texture rectangle
    newGameRect , ///< new game button texture rectangle
    loadRect , ///< load game button texture rectangle
    customRect ,///< custom game button texture rectangle
    exitRect ,///< exit button texture rectangle
    scoreRect , ///< score title texture rectangle
    numRect; ///< score numerals texture rectangle


};

#endif