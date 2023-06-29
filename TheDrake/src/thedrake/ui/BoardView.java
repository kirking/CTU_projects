package thedrake.ui;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.ListView;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import thedrake.*;

public class BoardView extends GridPane implements TileViewContext {

    private GameState gameState;

    private ValidMoves validMoves;

    private TileView selected;

    private ListView<String> blueStack;

    private ListView<String> orangeStack;

    private ListView<String> blueCaptured;

    private ListView<String> orangeCaptured;


    public BoardView(GameState gameState, ListView<String> blueStack,
                     ListView<String> orangeStack, ListView<String> blueCaptured,
                     ListView<String> orangeCaptured) {

        this.gameState = gameState;

        this.validMoves = new ValidMoves(gameState);

        this.orangeCaptured = orangeCaptured;

        this.blueStack = blueStack;

        this.orangeStack = orangeStack;

        this.blueCaptured = blueCaptured;

        PositionFactory positionFactory = gameState.board().positionFactory();

        for (int y = 0; y < gameState.board().dimension(); y++) {
            for (int x = 0; x < gameState.board().dimension(); x++) {
                int i = x;
                int j = gameState.board().dimension() - 1 - y;
                BoardPos boardPos = positionFactory.pos(i, j);
                add(new TileView(boardPos, gameState.tileAt(boardPos), this), x, y);
            }
        }

        setHgap(5);
        setVgap(5);
        setPadding(new Insets(15));
        setAlignment(Pos.CENTER);

        updateListViews();
    }

    private void clearMoves() {
        for (Node node : getChildren()) {
            TileView tileView = (TileView) node;
            tileView.clearMove();
        }
    }

    private void showMoves(List<Move> moveList) {
        for (Move move : moveList) {
            tileViewAt(move.target()).setMove(move);
        }
    }

    private TileView tileViewAt(BoardPos target) {
        int index = (gameState.board().dimension() - 1  - target.j()) * gameState.board().dimension() + target.i();
        return (TileView) getChildren().get(index);

    }

    @Override
    public void tileViewSelected(TileView tileView) {
        if (selected != null && selected != tileView) {
            selected.unselect();
        }

        selected = tileView;
        clearMoves();
        showMoves(validMoves.boardMoves(tileView.position()));
    }

    @Override
    public void executeMove(Move move) {
        selected.unselect();
        selected = null;
        clearMoves();
        gameState = move.execute(gameState);
        validMoves = new ValidMoves(gameState);
        updateTiles();
    }

    private void updateTiles() {

        List<Move> list = new ArrayList<>();
        for (Node node : getChildren()) {
            TileView tileView = (TileView) node;
            tileView.setTile(gameState.tileAt(tileView.position()));
            tileView.update();
            list.addAll(validMoves.boardMoves(tileView.position()));
        }

        if(list.isEmpty() && gameState.result()!=GameResult.VICTORY){
            gameState=gameState.draw();
        }

        if(gameState.result()==GameResult.VICTORY){
            Stage stage = (Stage) getScene().getWindow();
            stage.close();

            WinScreen newWin= new WinScreen();
            newWin.winSide(gameState.armyNotOnTurn().side());

            try {
                newWin.start(new Stage());
            } catch (Exception e) {
                e.printStackTrace();
            }

        }else if (gameState.result()==GameResult.DRAW){
            Stage stage = (Stage) getScene().getWindow();
            stage.close();

            WinScreen newWin= new WinScreen();
            try {
                newWin.start(new Stage());
            } catch (Exception e) {
                e.printStackTrace();
            }

        }
        for (Node node : getChildren()) {
            TileView tileView = (TileView) node;
            tileView.setTile(gameState.tileAt(tileView.position()));
            tileView.update();
        }

        updateListViews();
    }

    private void updateListViews() {

        if(gameState.armyOnTurn().side() == PlayingSide.BLUE) {
            setBackground(new Background(new BackgroundFill(Color.LIGHTSKYBLUE, CornerRadii.EMPTY, Insets.EMPTY)));

        }
        else {
            setBackground(new Background(new BackgroundFill(Color.HOTPINK, CornerRadii.EMPTY, Insets.EMPTY)));
        }

        blueStack.getItems().clear();
        blueStack.getItems().addAll(gameState.army(PlayingSide.BLUE).stack().stream().map(Troop::name).collect(Collectors.toList()));
        orangeStack.getItems().clear();
        orangeStack.getItems().addAll(gameState.army(PlayingSide.PINK).stack().stream().map(Troop::name).collect(Collectors.toList()));
        blueCaptured.getItems().clear();
        blueCaptured.getItems().addAll(gameState.army(PlayingSide.BLUE).captured().stream().map(Troop::name).collect(Collectors.toList()));
        orangeCaptured.getItems().clear();
        orangeCaptured.getItems().addAll(gameState.army(PlayingSide.PINK).captured().stream().map(Troop::name).collect(Collectors.toList()));
    }


}



