package thedrake.ui;

import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Orientation;

import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.image.Image;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import thedrake.*;


public class TheDrakeApp extends Application {


    @Override
    public void start(Stage primaryStage) throws Exception {


        GameState gameState = createSampleGameState();

        PositionFactory positionFactory = gameState.board().positionFactory();

        Label blueStackLbl = new Label("Blue Stack: ");
        Label orangeStackLbl = new Label("Pink Stack: ");
        Label blueCapturedLbl = new Label("Blue Captured: ");
        Label orangeCapturedLbl = new Label("Pink Captured: ");

        ListView<String> BlueStack = new ListView<String>();
        BlueStack.setOrientation(Orientation.VERTICAL);
        BlueStack.setMouseTransparent(true);
        BlueStack.setFocusTraversable(false);

        ListView<String> OrangeStack = new ListView<String>();
        OrangeStack.setOrientation(Orientation.VERTICAL);
        OrangeStack.setMouseTransparent(true);
        OrangeStack.setFocusTraversable(false);

        ListView<String> OrangeCaptured = new ListView<String>();
        OrangeCaptured.setOrientation(Orientation.VERTICAL);
        OrangeCaptured.setMouseTransparent(true);
        OrangeCaptured.setFocusTraversable(false);

        ListView<String> BlueCaptured = new ListView<String>();
        BlueCaptured.setOrientation(Orientation.VERTICAL);
        BlueCaptured.setMouseTransparent(true);
        BlueCaptured.setFocusTraversable(false);

        VBox blueStackBox = new VBox();
        blueStackBox.setSpacing(5.0);
        blueStackBox.getChildren().addAll(blueStackLbl,BlueStack);
        blueStackBox.setMaxWidth(180.0);

        VBox orangeStackBox = new VBox();
        orangeStackBox.setSpacing(5.0);
        orangeStackBox.setMaxWidth(180.0);
        orangeStackBox.getChildren().addAll(orangeStackLbl, OrangeStack);

        VBox blueCapturedBox = new VBox();
        blueCapturedBox.setSpacing(5.0);
        blueCapturedBox.getChildren().addAll(blueCapturedLbl, BlueCaptured);
        blueCapturedBox.setMaxWidth(180.0);

        VBox orangeCapturedBox = new VBox();
        orangeCapturedBox.setSpacing(5.0);
        orangeCapturedBox.getChildren().addAll(orangeCapturedLbl, OrangeCaptured);
        orangeCapturedBox.setMaxWidth(180.0);

        BoardView boardView = new BoardView(gameState, BlueStack, OrangeStack, BlueCaptured, OrangeCaptured);

        GridPane pane = new GridPane();


        pane.setHgap(10.0);
        pane.setVgap(5.0);
        pane.setPadding(new Insets(10,1,0,145));





        pane.add(blueStackBox, 1, 1);
        pane.add(blueCapturedBox, 1, 2);
        pane.add(orangeStackBox, 3, 1);
        pane.add(orangeCapturedBox, 3, 2);
        pane.add(boardView, 2, 0, 1, 3);

        Button exitButton = new Button("EXIT" );

        exitButton.setOnMouseClicked( (e)-> {
            primaryStage.close();

        });


        pane.add(exitButton , 4 ,2);


        primaryStage.setScene(new Scene(pane, 1550, 910));

        primaryStage.setTitle("The Drake");

        primaryStage.show();
    }

    private static GameState createSampleGameState() {
        Board board = new Board(8);
        PositionFactory positionFactory = board.positionFactory();



        for ( int i = 0 ;  i < 6 ; i++){

            double iRnd = Math.random()*(7-1) + 1;

            double jRnd = Math.random()*(7-1) + 1;

            board = board.withTiles(new Board.TileAt(positionFactory.pos((int)iRnd, (int)jRnd), BoardTile.MOUNTAIN));

        }

        return new StandardDrakeSetup().startState(board);
    }

}
