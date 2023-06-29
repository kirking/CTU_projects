package thedrake;

import java.io.PrintWriter;
import java.util.*;

import static thedrake.TroopFace.AVERS;

public class BoardTroops implements JSONSerializable {
	private final PlayingSide playingSide;
	private final Map<BoardPos, TroopTile> troopMap;
	private final TilePos leaderPosition;
	private final int guards;

	public BoardTroops(PlayingSide playingSide) {
		// Místo pro váš kód

		this.playingSide = playingSide;
		this.troopMap = Collections.EMPTY_MAP;
		this.leaderPosition = TilePos.OFF_BOARD;
		this.guards = 0;

	}

	public BoardTroops(
			PlayingSide playingSide,
			Map<BoardPos, TroopTile> troopMap,
			TilePos leaderPosition,
			int guards) {
		// Místo pro váš kód
		this.playingSide = playingSide;
		this.troopMap = troopMap;
		this.leaderPosition = leaderPosition;
		this.guards = guards;


	}

	public Optional<TroopTile> at(TilePos pos) {
		// Místo pro váš kód
		if (troopMap.containsKey(pos)) {

			TroopTile tmp = troopMap.get(pos);

			Optional<TroopTile> out = Optional.of(tmp);

			return out;
		} else return Optional.empty();


	}

	public PlayingSide playingSide() {
		// Místo pro váš kód

		return playingSide;


	}

	public TilePos leaderPosition() {
		// Místo pro váš kód

		return leaderPosition;
	}

	public int guards() {
		// Místo pro váš kód

		return guards;

	}

	public boolean isLeaderPlaced() {
		// Místo pro váš kód
		return leaderPosition != TilePos.OFF_BOARD;


	}

	public boolean isPlacingGuards() {
		// Místo pro váš kód
		if (!isLeaderPlaced()) return false;

		else return guards < 2;

	}

	public Set<BoardPos> troopPositions() {
		// Místo pro váš kód

		return troopMap.keySet();


	}

	public BoardTroops placeTroop(Troop troop, BoardPos target) {
		// Vrací novou instanci BoardTroops s novou dlaždicí TroopTile na pozici target obsahující jednotku troop lícovou stranou nahoru.
		// Tato metoda vyhazuje výjimku IllegalArgumentException, pokud je již zadaná pozice obsazena jinou dlaždicí.
		// Pokud pomocí této metody stavíme úplně první jednotku, bere se tato jednotka jako vůdce a je potřeba nastavit pozici leaderPosition.
		// Tímto hra přechází do fáze stavění stráží.
		//
		//Pokud pomocí této metody stavíme druhou a třetí jednotku, jsme ve fázi stavění stráží.
		// Tato fáze konči ve chvíli, kdy jsou obě stráže postaveny.


		if (at(target).isPresent())
			throw new IllegalArgumentException();

		else if (!this.isLeaderPlaced()) {

			Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);


			TroopTile newTroopTile = new TroopTile(troop, playingSide, TroopFace.AVERS);

			newTroops.put(target, newTroopTile);

			return new BoardTroops(playingSide(), newTroops, target, guards());
		} else if (this.isPlacingGuards()) {


			Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);

			TroopTile newTroopTile = new TroopTile(troop, playingSide, TroopFace.AVERS);

			newTroops.put(target, newTroopTile);

			int newGuards = guards() + 1;

			return new BoardTroops(playingSide(), newTroops, leaderPosition(), newGuards);
		} else {
			Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);


			TroopTile newTroopTile = new TroopTile(troop, playingSide, TroopFace.AVERS);

			newTroops.put(target, newTroopTile);

			return new BoardTroops(playingSide(), newTroops, leaderPosition(), guards());

		}

	}

	public BoardTroops troopStep(BoardPos origin, BoardPos target) {
		// Místo pro váš kód
		// Vrací novou instanci BoardTroops s dlaždicí TroopTile na pozici origin posunutou na pozici target a otočenou na opačnou stranu.
		// Tato metoda vyhazuje výjimku IllegalStateException pokud jsme ve stavu stavění vůdce, nebo stavění stráží,
		// neboť v těchto fázích nelze ješte s dlaždicemi pohybovat.
		// Metoda dále vyhazuje výjimku IllegalArgumentException pokud je pozice origin prázdná nebo pozice target již obsazená.
		//
		//Pozor, že pokud pomocí této metody pohybujeme s vůdce, je třeba aktualizovat jeho pozici.

		if (!isLeaderPlaced()) {
			throw new IllegalStateException(
					"Cannot move troops before the leader is placed.");
		}

		if (isPlacingGuards()) {
			throw new IllegalStateException(
					"Cannot move troops before guards are placed.");
		}

		if (!at(origin).isPresent())
			throw new IllegalArgumentException();

		if (at(target).isPresent())
			throw new IllegalArgumentException();

		TilePos newLeaderPos = leaderPosition();
		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);

		TroopTile tmpTroopTile = troopMap.get(origin);

		newTroops.put(target, tmpTroopTile);

		newTroops.remove(origin);

		if (newLeaderPos.i() == origin.i() && newLeaderPos.j() == origin.j()) newLeaderPos = target;


		return new BoardTroops(playingSide(), newTroops, newLeaderPos, guards()).troopFlip(target);


	}

	public BoardTroops troopFlip(BoardPos origin) {
		if (!isLeaderPlaced()) {
			throw new IllegalStateException(
					"Cannot move troops before the leader is placed.");
		}

		if (isPlacingGuards()) {
			throw new IllegalStateException(
					"Cannot move troops before guards are placed.");
		}

		if (!at(origin).isPresent())
			throw new IllegalArgumentException();

		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
		TroopTile tile = newTroops.remove(origin);
		newTroops.put(origin, tile.flipped());

		return new BoardTroops(playingSide(), newTroops, leaderPosition, guards);
	}


	public BoardTroops removeTroop(BoardPos target) {
		// Místo pro váš kód

		if (!isLeaderPlaced()) {
			throw new IllegalStateException(
					"Cannot move troops before the leader is placed.");
		}

		if (isPlacingGuards()) {
			throw new IllegalStateException(
					"Cannot move troops before guards are placed.");
		}

		if (!at(target).isPresent())
			throw new IllegalArgumentException();

		//BoardTroops newBoardTroops = new BoardTroops(playingSide, troopMap , leaderPosition, guards);

		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);

		newTroops.remove(target);

		TilePos newLeaderPos = leaderPosition();

		if (target.i() == newLeaderPos.i() && target.j() == newLeaderPos.j()) newLeaderPos = TilePos.OFF_BOARD;

		return new BoardTroops(playingSide(), newTroops, newLeaderPos, guards());


	}

	@Override
	public void toJSON(PrintWriter writer) {
		writer.print("\"boardTroops\":{\"side\":\"" + playingSide + "\"," +
				"\"leaderPosition\":\"" + leaderPosition + "\"," +
				"\"guards\":" + guards + ",");
		writer.print("\"troopMap\":");

		writer.print("{");
		ArrayList<Map.Entry<BoardPos, TroopTile>> troops = new ArrayList<>(troopMap.entrySet());
		troops.sort(Comparator.comparing(x -> x.getKey().toString()));
		Iterator<Map.Entry<BoardPos, TroopTile>> iter = troops.iterator();
		while (iter.hasNext()) {
			Map.Entry<BoardPos, TroopTile> tmp = iter.next();
			writer.print("\"");
			writer.print(tmp.getKey().column());
			writer.print(tmp.getKey().row());
			writer.print("\":{");
			tmp.getValue().toJSON(writer);
			writer.print("}");
			if (iter.hasNext()) writer.print(",");

		}
		writer.print("}");
		writer.print("}");

	}


}