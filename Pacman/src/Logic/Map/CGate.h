#ifndef C_GATE
#define C_GATE


#include "CTile.h"
/*!
    \brief Gates class.

	 Represents gates in the maze.
     */
class CGate : public CTile
{

public:

    /*!
        Constructor for gate, set sym to 'G'.
     */
    CGate()
    {

        sym = 'G';
    }

    ~CGate() = default;

    /*!
        Overrided method .
        \return false , cause units can't step on the gate
        (only ghosts in RELEASE mode)

     */
    bool canStep() const override{

        return false;

    }
/*!
       Change gate's symbol when gate is open.

     */
    void openGate()
    {

        sym = 'g';

    }

    /*!
       Change gate's symbol when gate is closed.

     */
    void closeGate()
    {

        sym = 'G';

    }
/*!
        Overrided method from CTile class.
        Casts CGate's pointer to base class's pointer( CTile ).
        \return base class's shared_ptr.
     */
    std::shared_ptr<CTile> clone() const override
    {

        return std::make_shared<CGate>(*this);

    }



};

#endif