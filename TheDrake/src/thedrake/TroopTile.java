package thedrake;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public final class TroopTile implements Tile,JSONSerializable{


        final private Troop troop;
        final private PlayingSide side;
        final private TroopFace face;


        public TroopTile(Troop troop, PlayingSide side, TroopFace face) {
            this.troop = troop;
            this.side = side;
            this.face = face;
        }


        public List<Move> movesFrom(BoardPos pos, GameState state){

          List<Move> outMoves = new ArrayList<>();

          for ( TroopAction action : troop.actions(face)  ) {

              outMoves.addAll( action.movesFrom( pos , side, state));

          }

          return outMoves;

        }



      public PlayingSide side(){

            return this.side;

      }

     public TroopFace face(){
            return this.face;

     }

     public Troop troop(){

            return troop;


     }

    public boolean canStepOn() {

        return false;
    }

    public boolean hasTroop(){
            return true;
    }

    public TroopTile flipped(){
            final TroopFace tmpFace;
            if (face == TroopFace.REVERS) tmpFace = TroopFace.AVERS;
            else tmpFace = TroopFace.REVERS;


            return new TroopTile(troop, side, tmpFace);


    }


    @Override
    public void toJSON(PrintWriter writer) {
        writer.print("\"troop\":\"" + troop.name() + "\"," +
                "\"side\":\"" + side + "\"," +
                "\"face\":\"" + face + "\"");
    }





}
