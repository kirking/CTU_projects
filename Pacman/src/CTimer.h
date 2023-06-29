#ifndef C_TIMER
#define C_TIMER

#include <SDL2/SDL.h>
/*!
	\brief Timer class

	Uses to control game modes timings.
*/
class CTimer
{

public:

    CTimer() = default;


    ~CTimer() = default;

    /*!
       Check if time is out.
      \return true if time is out ,  false if isn't
     */
    bool checkTime() const
    {

        if(!stop) return (  SDL_GetTicks( )   >= endTime ) ;

        else return true;

    }
     /*!
       Check how much time left.
      \return last time.
     */
    unsigned int getLastTime()
    {

        if( (int ) endTime - (int)SDL_GetTicks( )  < 0 ) return 0;

        return endTime - SDL_GetTicks( );

    }
     /*!
       Set scareTimer
       \param[in] time time for scareTimer.
     */
    void setTimer( unsigned int  time )
    {

        startTime = SDL_GetTicks();

        endTime = startTime + time;

        stop = false;

    }
    /*!
          Stop scareTimer.
     */
    void stopTimer()
    {

        stop = true;

    }
    /*!
          Check if scareTimer is stopped.
          \return  true if it's stopped , false if isn't
        */
    bool isStopped() const
    {

        return stop;

    }

private:

    unsigned int startTime = 0, ///< From this time starts scareTimer's countdown.
               endTime = 0 ;  ///< Timer's end time.

    bool stop = true; ///< Timer stop flag.


};

#endif