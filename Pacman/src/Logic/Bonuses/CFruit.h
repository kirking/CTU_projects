#ifndef C_FRUIT
#define C_FRUIT

#include <memory>
#include "CBonus.h"
/*!
	\brief Fruit bonus's class.

	Bonus which has different points ,
    depends on game level.
*/
class CFruit : public CBonus
{

public:
/*!
        Constructor for fruit, uses Parent's constructor.
        \param[in] nPoints bonus's points.
     */
    CFruit(int nPoints):CBonus(nPoints){};

/*!
        Overrided method ,
        uses to get bonus symbol.
        \return fruit's symbol.

     */
    char getSym() const override
    {

        return 'F';

    }
/*!
        Overrided method from CBonus class.
        Casts CFruit's pointer to base class's pointer( CBonus ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CBonus> clone() const
    {

        return std::make_shared<CFruit>(*this);

    }

};

#endif