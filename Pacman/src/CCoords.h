#ifndef CCOORDS
#define CCOORDS

/*!
    \brief Coords class.

	 Represents coordinates.
     */
class CCoords
{
public:

    int xPos, ///< x coordinate
    yPos;  ///< y coordinate

    /*!
       Constructor for Coords.
       \param[in] x x position
       \param[in] y y position

    */
    CCoords( int x , int y ):xPos(x),yPos(y){};

    /*!
      Default CCoords constructor.

    */
    CCoords():xPos(0), yPos(0){};

    /*!
      Overloaded = operator.
      */
    CCoords & operator = ( const CCoords & nCoords )
    {

        if( &nCoords == this)
        {
            return *this;
        }

        xPos = nCoords.xPos;
        yPos = nCoords.yPos;

        return *this;
    }

/*!
      Overloaded == operator.

      \return coordinate's equality.
      */
    bool operator == ( const CCoords & nCoords ) const
    {
        return ( xPos == nCoords.xPos && yPos == nCoords.yPos );
    }

    /*!
      Overloaded != operator.

      \return coordinate's non-equality.
      */
    bool operator != ( const CCoords & nCoords ) const
    {
        return !((*this) == nCoords) ;
    }

};

#endif