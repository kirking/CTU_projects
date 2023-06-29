package thedrake;

import java.io.PrintWriter;

import java.util.*;
import java.util.stream.Stream;

public class Board implements JSONSerializable{
      private int dimension;
	  private BoardTile[][] board ;
	// Konstruktor. Vytvoří čtvercovou hrací desku zadaného rozměru, kde všechny dlaždice jsou prázdné, tedy BoardTile.EMPTY
	public Board(int dimension) {
		// Místo pro váš kód
		  this.dimension=dimension;
		  this.board = new BoardTile[dimension][dimension];
		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < dimension; j++) {
                board[i][j]=BoardTile.EMPTY;
			}
		}
	}
	// Rozměr hrací desky
	public int dimension() {
		return this.dimension;

	}

	// Vrací dlaždici na zvolené pozici.
	public BoardTile at(BoardPos pos) {
		// Místo pro váš kód
         return board[pos.i()][pos.j()];
	}

	// Vytváří novou hrací desku s novými dlaždicemi. Všechny ostatní dlaždice zůstávají stejné
	public Board withTiles(TileAt ...ats) {
		// Místo pro váš kód
		Board newBoard = new Board(dimension);
           newBoard.board=board.clone();
          for(int i = 0 ; i < board.length; i++){
           	newBoard.board[i]=board[i].clone();
          }

		for (TileAt tmp: ats )

			newBoard.board[tmp.pos.i()][tmp.pos.j()]=tmp.tile;


          return newBoard;

	}
	// Vytvoří instanci PositionFactory pro výrobu pozic na tomto hracím plánu
	public PositionFactory positionFactory() {
		// Místo pro váš kód
            return new PositionFactory(dimension);



	}

	@Override
	public void toJSON(PrintWriter writer) {

		writer.print("\"board\":{" + "\"dimension\":" + dimension + ",\"tiles\":[");



		  for (int i = 0; i < dimension; i++) {

		  	     for ( int j = 0; j < dimension; j++  ) {



					 board[j][i].toJSON(writer);

					 if (i != 3 || j != 3) writer.print(",");

				 }

		  }

		  writer.printf("]}");
	}

	public static class TileAt {
		public final BoardPos pos;
		public final BoardTile tile;
		
		public TileAt(BoardPos pos, BoardTile tile) {
			this.pos = pos;
			this.tile = tile;
		}
	}




}

