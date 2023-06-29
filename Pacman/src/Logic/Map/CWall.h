#ifndef C_WALL
#define C_WALL

#include "CTile.h"
/*!
    \brief Wall class.

	 Represents walls in the maze.
     */
class CWall : public CTile
{

public:


    /*!
        Constructor for wall, set sym to 'W'.
     */
    CWall()
    {

        sym = 'W';

    };

    ~CWall() = default;

    /*!
        Overrided method .
        \return false , cause units can't step on the wall.

     */
    bool canStep() const override{

        return false;

    }
/*!
        Overrided method from CTile class.
        Casts CWall's pointer to base class's pointer( CTile ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CTile> clone() const override
    {

        return std::make_shared<CWall>(*this);

    }




};

#endif