package thedrake;

import java.io.PrintWriter;

public class GameState implements JSONSerializable{
	private final Board board;
	private final PlayingSide sideOnTurn;
	private final Army blueArmy;
	private final Army orangeArmy;
	private final GameResult result;
	
	public GameState(
			Board board, 
			Army blueArmy, 
			Army orangeArmy) {
		this(board, blueArmy, orangeArmy, PlayingSide.BLUE, GameResult.IN_PLAY);
	}
	
	public GameState(
			Board board, 
			Army blueArmy, 
			Army orangeArmy, 
			PlayingSide sideOnTurn, 
			GameResult result) {
		this.board = board;
		this.sideOnTurn = sideOnTurn;
		this.blueArmy = blueArmy;
		this.orangeArmy = orangeArmy;
		this.result = result;
	}



	public Board board() {
		return board;
	}
	
	public PlayingSide sideOnTurn() {
		return sideOnTurn;
	}
	
	public GameResult result() {
		return result;
	}
	
	public Army army(PlayingSide side) {
		if(side == PlayingSide.BLUE) {
			return blueArmy;
		}
		
		return orangeArmy;
	}
	
	public Army armyOnTurn() {
		return army(sideOnTurn);
	}
	
	public Army armyNotOnTurn() {
		if(sideOnTurn == PlayingSide.BLUE)
			return orangeArmy;
		
		return blueArmy;
	}
	
	public Tile tileAt(BoardPos pos) {
		// Vrátí dlaždici, která se nachází na hrací desce na pozici pos.
      // Musí tedy zkontrolovat, jestli na této pozici není jednotka z
      // armády nějakého hráče a pokud ne, vrátí dlaždici z objektu board

		if ( armyOnTurn().boardTroops().at(pos).isPresent() )
			  return armyOnTurn().boardTroops().at(pos).get();

		if ( armyNotOnTurn().boardTroops().at(pos).isPresent() )
			return armyNotOnTurn().boardTroops().at(pos).get();


		return board().at(pos);

	}
	
	private boolean canStepFrom(TilePos origin) {
		// Vrátí true, pokud je možné ze zadané pozice začít tah nějakou
		// jednotkou. Vrací false, pokud stav hry není IN_PLAY, pokud
       // na dané pozici nestojí žádné jednotka nebo pokud na pozici
      // stojí jednotka hráče, který zrovna není na tahu.
		// Při implementaci vemte v úvahu zahájení hry. Dokud nejsou
		// postaveny stráže, žádné pohyby jednotek po desce nejsou možné.

		return result() == GameResult.IN_PLAY
				&& armyOnTurn().boardTroops().isLeaderPlaced()
				&& !armyOnTurn().boardTroops().isPlacingGuards()
				&& armyOnTurn().boardTroops().at(origin).isPresent()
				&& armyNotOnTurn().boardTroops().at(origin).isEmpty();


	}

	private boolean canStepTo(TilePos target) {
		// Místo pro váš kód
		// Vrátí true, pokud je možné na zadanou pozici dokončit tah nějakou
        // jednotkou. Vrací false, pokud stav hry není IN_PLAY nebo pokud
        // na zadanou dlaždici nelze vstoupit (metoda Tile.canStepOn).


		return result() == GameResult.IN_PLAY
				&& target instanceof BoardPos
				&& board().at((BoardPos) target).canStepOn()
				&& armyOnTurn().boardTroops().isLeaderPlaced()
				&& !armyOnTurn().boardTroops().isPlacingGuards()
				&& armyOnTurn().boardTroops().at(target).isEmpty()
				&& armyNotOnTurn().boardTroops().at(target).isEmpty();


	}
	
	private boolean canCaptureOn(TilePos target) {
		// Místo pro váš kód
		// Vrátí true, pokud je možné na zadané pozici vyhodit soupeřovu jednotku.
		// Vrací false, pokud stav hry není IN_PLAY nebo pokud
        // na zadané pozici nestojí jednotka hráče, který zrovna není na tahu.

		return result == GameResult.IN_PLAY &&
				armyOnTurn().boardTroops().isLeaderPlaced() &&
				!armyOnTurn().boardTroops().isPlacingGuards() &&
				armyOnTurn().boardTroops().at(target).isEmpty() &&
				armyNotOnTurn().boardTroops().at(target).isPresent();

	}
	
	public boolean canStep(TilePos origin, TilePos target)  {
		return canStepFrom(origin) && canStepTo(target);
	}
	
	public boolean canCapture(TilePos origin, TilePos target)  {
		return canStepFrom(origin) && canCaptureOn(target);
	}
	
	public boolean canPlaceFromStack(TilePos target) {
		// Místo pro váš kód
		// Vrátí true, pokud je možné na zadanou pozici položit jednotku ze
       // zásobníku.. Vrací false, pokud stav hry není IN_PLAY, pokud je zásobník
        // armády, která je zrovna na tahu prázdný, pokud není možné na danou
      // dlaždici vstoupit. Při implementaci vemte v úvahu zahájení hry, kdy
       // se vkládání jednotek řídí jinými pravidly než ve střední hře.

		if(( armyOnTurn().stack().isEmpty()
				|| !(target instanceof BoardPos)
				|| !board().at((BoardPos) target).canStepOn()
				|| result != GameResult.IN_PLAY
				|| armyOnTurn().boardTroops().at(target).isPresent()
				|| armyNotOnTurn().boardTroops().at(target).isPresent())
			    || (!armyOnTurn().boardTroops().isLeaderPlaced()
				&& ((armyOnTurn() == blueArmy && target.row() != 1)
				|| (armyOnTurn() == orangeArmy && target.row() != board.dimension()))) )
			return false;

		int a, b;
		return !armyOnTurn().boardTroops().isPlacingGuards() ||
				(((a = Math.abs(armyOnTurn().boardTroops().leaderPosition().i() - target.i())) <= 1) &&
						((b = Math.abs(armyOnTurn().boardTroops().leaderPosition().j() - target.j())) <= 1) &&
						(a != 1 || b != 1));


	}
	
	public GameState stepOnly(BoardPos origin, BoardPos target) {		
		if(canStep(origin, target))		 
			return createNewGameState(
					armyNotOnTurn(),
					armyOnTurn().troopStep(origin, target), GameResult.IN_PLAY);
		
		throw new IllegalArgumentException();
	}
	
	public GameState stepAndCapture(BoardPos origin, BoardPos target) {
		if(canCapture(origin, target)) {
			Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
			GameResult newResult = GameResult.IN_PLAY;
			
			if(armyNotOnTurn().boardTroops().leaderPosition().equals(target))
				newResult = GameResult.VICTORY;
			
			return createNewGameState(
					armyNotOnTurn().removeTroop(target), 
					armyOnTurn().troopStep(origin, target).capture(captured), newResult);
		}
		
		throw new IllegalArgumentException();
	}
	
	public GameState captureOnly(BoardPos origin, BoardPos target) {
		if(canCapture(origin, target)) {
			Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
			GameResult newResult = GameResult.IN_PLAY;
			
			if(armyNotOnTurn().boardTroops().leaderPosition().equals(target))
				newResult = GameResult.VICTORY;
			
			return createNewGameState(
					armyNotOnTurn().removeTroop(target),
					armyOnTurn().troopFlip(origin).capture(captured), newResult);
		}
		
		throw new IllegalArgumentException();
	}
	
	public GameState placeFromStack(BoardPos target) {
		if(canPlaceFromStack(target)) {
			return createNewGameState(
					armyNotOnTurn(), 
					armyOnTurn().placeFromStack(target), 
					GameResult.IN_PLAY);
		}
		
		throw new IllegalArgumentException();
	}
	
	public GameState resign() {
		return createNewGameState(
				armyNotOnTurn(), 
				armyOnTurn(), 
				GameResult.VICTORY);
	}
	
	public GameState draw() {
		return createNewGameState(
				armyOnTurn(), 
				armyNotOnTurn(), 
				GameResult.DRAW);
	}
	
	private GameState createNewGameState(Army armyOnTurn, Army armyNotOnTurn, GameResult result) {
		if(armyOnTurn.side() == PlayingSide.BLUE) {
			return new GameState(board, armyOnTurn, armyNotOnTurn, PlayingSide.BLUE, result);
		}
		
		return new GameState(board, armyNotOnTurn, armyOnTurn, PlayingSide.PINK, result);
	}

	@Override
	public void toJSON(PrintWriter writer) {

		writer.printf("{\"result\":\"%s\"", result());
		writer.print(",");
		board.toJSON(writer);
		writer.print(",");
		writer.print("\"blueArmy\":");
		blueArmy.toJSON(writer);
		writer.print(",");
		writer.print("\"orangeArmy\":");
		orangeArmy.toJSON(writer);
		writer.print("}");
	}



	}



