package thedrake;

import java.util.List;
import java.util.ArrayList;

public class SlideAction extends TroopAction {

    public SlideAction(Offset2D offset) {
        super(offset);
    }


    public SlideAction(int offSetX, int offSetY) {

        super(offSetX, offSetY);


    }

    public List<Move> movesFrom(BoardPos origin, PlayingSide side, GameState state) {
        List<Move> result = new ArrayList<>();
        TilePos target = origin.stepByPlayingSide(offset(), side);

        while( !target.equals( TilePos.OFF_BOARD ) ) {

            if(state.canStep( origin, target )){
                result.add(new StepOnly(origin,  (BoardPos) target));
            }
            else  if ( state.canCapture(origin, target) )
                result.add( new StepAndCapture( origin, ( BoardPos )target ) );
            else break;
            target = target.stepByPlayingSide( offset(), side );

        }


        return result;

    }

}