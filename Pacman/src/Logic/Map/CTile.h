#ifndef C_TILE
#define C_TILE


#include <memory>
#include "../../CCoords.h"
/*!
    \brief Tile class.

	 Parent class of all tiles in the game.
     */
class CTile
{

public:

    CTile() = default;

    ~CTile() = default;

    /*!
        Uses to get tile symbol.
        \return tile's symbol.

     */
    char getSym() const
    {

        return sym;

    }
/*!
        Virtual method ,
        uses to check , if unit can step on this tile.
        \return true if can , false if can't.

     */
    virtual bool canStep() const = 0;
/*!
        Method uses in inherited classes to
        make possible polymorphic call ,
        also simplify memory management
        ( uses shared_ptr instead of common pointer).
        \return base class's CTile shared_ptr.
     */
    virtual std::shared_ptr<CTile> clone() const = 0;

protected:

    char sym; ///< Tile's symbol.

};

#endif