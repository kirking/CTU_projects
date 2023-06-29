#ifndef C_ENERGIZER
#define C_ENERGIZER


#include <memory>
#include "CBonus.h"
/*!
	\brief Energizer bonus's class.

	Bonus which change game mode.
*/
class CEnergizer : public CBonus
{
public:


    CEnergizer():CBonus(){};

    /*!
        Constructor for energizer, uses Parent's constructor.
        \param[in] nPoints bonus's points.
     */
    CEnergizer(int nPoints):CBonus(nPoints){};

/*!
        Overrided method ,
        change game mode when eaten.

     */
    void doAction() override;

/*!
        Overrided method ,
        uses to get bonus symbol.
        \return energizer's symbol.

     */
    char getSym() const override
    {

        return 'O';

    }

    /*!
        Overrided method from CBonus class.
        Casts CEnergizer's pointer to base class's pointer( CBonus ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CBonus> clone() const override
    {

        return std::make_shared<CEnergizer>(*this);

    }

};

#endif