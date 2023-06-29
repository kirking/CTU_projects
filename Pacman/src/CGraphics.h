#ifndef C_GRAPHICS
#define C_GRAPHICS

#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "CLogic.h"

/*!
    \brief Game graphics class.
     Render all objects in the game.
     */
class CGraphics {

public:

    CGraphics() = default;

    /*!
       Constructor for Graphics.

        \param[in] log pointer to game logic.
    */
    CGraphics(CLogic *log) : logic(log) {};


    ~CGraphics() = default;

/*!
       Method uses to initialize graphic.
       \param[in] title game window titles
       \param[in] xPos game window x position
       \param[in] yPos game window y position
       \param[in] width game window width
       \param[in] height game window heiht
     */
    void init(const char *title, int xPos, int yPos, int width, int height) {

        SDL_Init(SDL_INIT_EVERYTHING);

        window = SDL_CreateWindow(title, xPos, yPos, width, height, SDL_WINDOW_SHOWN);

        renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_Surface *tmpSurface = nullptr;

        //Rects initialisation
        pacmanRect.h = 32;
        pacmanRect.w = 32;

        blinkyRect.h = 32;
        blinkyRect.w = 32;

        pinkyRect.h = 32;
        pinkyRect.w = 32;

        inkyRect.h = 32;
        inkyRect.w = 32;

        clydeRect.h = 32;
        clydeRect.w = 32;

        //Loading numerals textures
        for (int i = 0; i < 10; ++i) {


            std::string tmpString = "../poligkir/examples/Assets/n";

            tmpString += (std::to_string(i) + ".png");

            tmpSurface = IMG_Load(tmpString.c_str());

            scoreTex[i] = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        }

        //Loading other textures
        tmpSurface = IMG_Load("../poligkir/examples/Assets/scared1.png");

        scaredGhost = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/fruit.png");

        fruitTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/wall.png");

        wall = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/road.png");

        road = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/closedGate.png");

        closedGate = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/openedGate.png");

        openedGate = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/roadEnergizer.png");

        roadEnergizer = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/roadBonus.png");

        roadBonus = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/level.png");

        levelTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);


        tmpSurface = IMG_Load("../poligkir/examples/Assets/PacTmp.png");

        pacman = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/blinky.png");

        blinky = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/pinky.png");

        pinky = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/inky.png");

        inky = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/clyde.png");

        clyde = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/scared1.png");

        scaredGhost = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        SDL_FreeSurface(tmpSurface);


        pacmanRect.y = logic->getPacmanPos().yPos * 32;

        pacmanRect.x = logic->getPacmanPos().xPos * 34;

        tmpSurface = IMG_Load("../poligkir/examples/Assets/score.png");


        score = SDL_CreateTextureFromSurface(renderer, tmpSurface);


        SDL_FreeSurface(tmpSurface);

        tmpSurface = IMG_Load("../poligkir/examples/Assets/life.png");

        life = SDL_CreateTextureFromSurface(renderer, tmpSurface);

        scoreNum.push_back(0);

        levelNum.push_back(1);


    }

/*!
       Method uses to update game objects positions,
       states , count current score and current level.
     */
    void update() {


        // Update objects positions
        blinkyRect.x = logic->getGhostPos(0).xPos * 32;
        blinkyRect.y = logic->getGhostPos(0).yPos * 30;

        pinkyRect.x = logic->getGhostPos(1).xPos * 32;
        pinkyRect.y = logic->getGhostPos(1).yPos * 30;

        inkyRect.x = logic->getGhostPos(2).xPos * 32;
        inkyRect.y = logic->getGhostPos(2).yPos * 30;

        clydeRect.x = logic->getGhostPos(3).xPos * 32;
        clydeRect.y = logic->getGhostPos(3).yPos * 30;

        pacmanRect.x = logic->getPacmanPos().xPos * 32;

        pacmanRect.y = logic->getPacmanPos().yPos * 30;

       //Computing score
        if (scorePoints != logic->getScore()) {

            scorePoints = logic->getScore();

            scoreNum.clear();

            int tmp = scorePoints;

            while (tmp) {

                scoreNum.push_back(tmp % 10);

                tmp /= 10;

            }

        }
        //Computing level
        if (level != logic->getLevel()) {

            level = logic->getLevel();

            levelNum.clear();

            int tmp = level;

            while (tmp) {

                levelNum.push_back(tmp % 10);

                tmp /= 10;

            }


        }

    }

/*!
       Method uses to render all objects ,
       and dispay them to the screen.

     */
    void render() {
        SDL_RenderClear(renderer);


        SDL_Rect scoreRect, lifeRect, levelRect;

        scoreRect.x = 5;
        scoreRect.y = 900;
        scoreRect.h = 120;
        scoreRect.w = 200;

        levelRect.x = 400;
        levelRect.y = 892;
        levelRect.h = 120;
        levelRect.w = 200;

        lifeRect.x = 850;
        lifeRect.y = 930;
        lifeRect.h = 32;
        lifeRect.w = 32;

        //Map render
        for (int x = 0; x < 28; ++x) {

            SDL_Rect tmpDestR;

            tmpDestR.h = 30;

            tmpDestR.w = 32;

            for (int y = 0; y < 31; ++y) {

                tmpDestR.x = x * 32;

                tmpDestR.y = y * 30;

                char tmp = logic->getTileSym(CCoords(x, y));

                switch (tmp) {

                    case 'W':
                        SDL_RenderCopy(renderer, wall, nullptr, &tmpDestR);
                        break;

                    case 'R':
                        SDL_RenderCopy(renderer, road, nullptr, &tmpDestR);
                        break;

                    case 'o':
                        SDL_RenderCopy(renderer, roadBonus, nullptr, &tmpDestR);
                        break;

                    case 'O':
                        SDL_RenderCopy(renderer, roadEnergizer, nullptr, &tmpDestR);
                        break;

                    case 'g':
                        SDL_RenderCopy(renderer, openedGate, nullptr, &tmpDestR);
                        break;

                    case 'G':
                        SDL_RenderCopy(renderer, closedGate, nullptr, &tmpDestR);
                        break;

                    case 'F':
                        SDL_RenderCopy(renderer, fruitTex, nullptr, &tmpDestR);
                        break;

                    default:
                        SDL_RenderCopy(renderer, road, nullptr, &tmpDestR);
                        break;

                }

            }

        }

        SDL_Rect tmpRect;

        tmpRect.y = 935;

        tmpRect.x = 200;

        //Score render

        for (int i = (int) scoreNum.size() - 1; i >= 0; --i) {

            tmpRect.x += 32;

            SDL_RenderCopy(renderer, scoreTex[scoreNum[i]], nullptr, &tmpRect);

        }

        tmpRect.y = 935;

        tmpRect.x = 570;

        //Level number render

        for (int i = (int) levelNum.size() - 1; i >= 0; --i) {

            tmpRect.x += 32;

            SDL_RenderCopy(renderer, scoreTex[levelNum[i]], nullptr, &tmpRect);

        }

        //Lives render
        for (int i = logic->getPacLives(); i > 0; --i) {

            SDL_RenderCopy(renderer, life, nullptr, &lifeRect);

            lifeRect.x -= 35;

        }

        SDL_RenderCopy(renderer, pacman, nullptr, &pacmanRect);

        SDL_RenderCopy(renderer, score, nullptr, &scoreRect);

        SDL_RenderCopy(renderer, levelTex, nullptr, &levelRect);

        //Ghosts render

        if (logic->getMode(0) == EGhostmode::FRIGHTENED) {

            SDL_RenderCopy(renderer, scaredGhost, nullptr, &blinkyRect);

        } else SDL_RenderCopy(renderer, blinky, nullptr, &blinkyRect);

        if (logic->getMode(1) == EGhostmode::FRIGHTENED) {

            SDL_RenderCopy(renderer, scaredGhost, nullptr, &pinkyRect);

        } else SDL_RenderCopy(renderer, pinky, nullptr, &pinkyRect);

        if (logic->getMode(2) == EGhostmode::FRIGHTENED) {

            SDL_RenderCopy(renderer, scaredGhost, nullptr, &inkyRect);

        } else SDL_RenderCopy(renderer, inky, nullptr, &inkyRect);

        if (logic->getMode(3) == EGhostmode::FRIGHTENED) {

            SDL_RenderCopy(renderer, scaredGhost, nullptr, &clydeRect);

        } else SDL_RenderCopy(renderer, clyde, nullptr, &clydeRect);

        SDL_RenderPresent(renderer);

    }

    /*!
       Method uses to destroy renderer , window and
       textures.
     */
    void clean() {

        SDL_RenderClear(renderer);

        for (SDL_Texture *i : scoreTex) {

            SDL_DestroyTexture(i);


        }

        scoreNum.clear();

        SDL_DestroyTexture(road);
        SDL_DestroyTexture(wall);
        SDL_DestroyTexture(roadBonus);
        SDL_DestroyTexture(roadEnergizer);
        SDL_DestroyTexture(closedGate);
        SDL_DestroyTexture(openedGate);
        SDL_DestroyTexture(pacman);
        SDL_DestroyTexture(blinky);
        SDL_DestroyTexture(pinky);
        SDL_DestroyTexture(inky);
        SDL_DestroyTexture(clyde);
        SDL_DestroyTexture(scaredGhost);
        SDL_DestroyTexture(score);
        SDL_DestroyTexture(life);
        SDL_DestroyTexture(levelTex);
        SDL_DestroyTexture(fruitTex);

        SDL_DestroyWindow(window);

        SDL_DestroyRenderer(renderer);

        SDL_Quit();
    }

private:

    std::vector<int> scoreNum; ///< contains score as numerals

    std::vector<int> levelNum; ///< contains level as numerals

    int scorePoints = 0, ///< game score
            level = 1;   ///< game level

    SDL_Texture *scoreTex[10]; ///< numerals textures

    SDL_Texture *road = nullptr; ///< road texture

    SDL_Texture *wall = nullptr; ///< wall texture

    SDL_Texture *roadBonus = nullptr; ///< road with bonus texture

    SDL_Texture *roadEnergizer = nullptr; ///< road with energizer texture

    SDL_Texture *closedGate = nullptr;///< closed gate texture

    SDL_Texture *openedGate = nullptr;///< opened gate texture

    SDL_Texture *pacman = nullptr;///< pacman texture

    SDL_Texture *blinky = nullptr;///< blinky texture

    SDL_Texture *pinky = nullptr;///< pinky texture

    SDL_Texture *inky = nullptr;///< inky texture

    SDL_Texture *clyde = nullptr;///< clyde texture

    SDL_Texture *scaredGhost = nullptr;///< scared ghost texture

    SDL_Texture *score = nullptr;///< score title texture

    SDL_Texture *life = nullptr;///< life symbol texture

    SDL_Texture *levelTex = nullptr;///< level title texture

    SDL_Texture *fruitTex = nullptr;///< fruit bonus texture

    SDL_Rect pacmanRect, ///< pacman texture rectangle
    blinkyRect, ///< blinky texture rectangle
    pinkyRect,///< pinky texture rectangle
    inkyRect,///< inky texture rectangle
    clydeRect;///< clyde texture rectangle

    SDL_Window *window = nullptr; ///< game window

    SDL_Renderer *renderer = nullptr; ///< game renderer

    CLogic *logic = nullptr;///< logic to update objects


};

#endif