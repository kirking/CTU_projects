#ifndef C_ROAD
#define C_ROAD

#include <iostream>
#include "CTile.h"
#include "../Bonuses/CBonus.h"

/*!
    \brief Road class.

	 Represents roads in the maze.
     */
class CRoad : public CTile
{

public:

    /*!
        Constructor for road, set sym to 'r'.
     */
    CRoad()
    {

        sym ='r';

    };

    ~CRoad() = default;

    /*!
        Overrided method from CTile class.
        Casts CRoad's pointer to base class's pointer( CTile ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CTile> clone() const override
    {

        return std::make_shared<CRoad>(*this);

    }
/*!
        Method uses to set bonus on the road.
        \param [in] nBonus bonus to set.
     */
    void setBonus( const std::shared_ptr<CBonus> & nBonus )
    {

        bonus = nBonus;

        bonusFlag = true;

        sym = nBonus->getSym();

    }
/*!
        Method uses to get bonus from the road and
        activate bonus action.
     */
    void getBonus( ){

        if( bonusFlag )
        {

            bonus->doAction();

            bonusFlag = false;

            sym = 'r';

        }

    }
/*!
        Method uses to remove bonus from the road.
     */
    void removeBonus( ){

        if( bonusFlag )
        {

            bonusFlag = false;

            sym = 'r';

        }

    }

    /*!
        Overrided method .
        \return true , cause units can step on the road.

     */
    bool canStep() const override
    {

        return true;

    }

    /*!
        Getter for bonus flag.
        \return true if road contains bonus ,
        false if not.

     */
    bool hasBonus() const
    {

        return bonusFlag;

    }


private:

    std::shared_ptr<CBonus> bonus; ///< bonus on the road

    bool bonusFlag = false;///< contain bonus indicator


};

#endif