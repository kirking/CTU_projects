#ifndef C_BONUS
#define C_BONUS

#include <memory>

class CMaze;
/*!
	\brief Bonuses class.

	Parent class of all bonuses in the game.
*/
class CBonus
{

public:


    CBonus() = default;

    ~CBonus() = default;

/*!
        Constructor for bonuses.
        \param[in] nPoints bonus's points.
     */
    CBonus( int nPoints )
    {

        points = nPoints;

    }
/*!
        Virtual method ,
        uses for adding game points
        and change game mode ,
        depends on bonus type.
        Increments game eaten bonuses.
     */
    virtual void doAction();

/*!
        Virtual method ,
        uses to get bonus symbol.
        \return bonus's symbol.

     */
    virtual char getSym() const
    {

        return 'o';

    }
/*!
        Method uses in inherited classes to
        make possible polymorphic call ,
        also simplify memory management
        ( uses shared_ptr instead of common pointer).
        \return base class's  CBonus shared_ptr.
     */
    virtual std::shared_ptr<CBonus> clone() const
    {

        return std::make_shared<CBonus>(*this);

    }


protected:

    int points = 0 ; ///< Bonus's points.

};

#endif