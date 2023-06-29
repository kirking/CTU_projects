package thedrake;

import java.util.ArrayList;
import java.util.List;

public class StrikeAction extends TroopAction {

    public StrikeAction(Offset2D offset) {
        super(offset);
    }

public StrikeAction( int offSetX, int offSetY ){

        super(offSetX, offSetY);

}




    public List<Move> movesFrom(BoardPos origin, PlayingSide side, GameState state ){

        List<Move> outMove = new ArrayList<>();

        TilePos target = origin.stepByPlayingSide(offset(), side);

        if( state.canCapture(origin, target)  )
              outMove.add( new CaptureOnly( origin,(BoardPos) target));

           return outMove;

    }





}