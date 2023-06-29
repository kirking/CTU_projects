package thedrake.ui;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import thedrake.PlayingSide;


public class WinScreen extends Application {
    String winMessage = "DRAW";
    String bgColor = "#00FF00";

    public WinScreen() {
    }

    void winSide(PlayingSide side) {
        if (side == PlayingSide.PINK) {
            this.winMessage = "PINK WINS ";
            this.bgColor = "pink";
        } else {
            this.winMessage = "BLUE WINS ";
            this.bgColor = "blue";
        }

    }

    public void start(Stage primaryStage) throws Exception {
        Parent root = (Parent) FXMLLoader.load(this.getClass().getResource("WinScreen.fxml"));
        Label lblData = (Label)root.lookup("#lblTest");
        lblData.setText(this.winMessage);

        BorderPane brPane = (BorderPane)root.lookup("#border");
        if( this.bgColor == "blue" ){

            Image image = new Image(getClass().getResourceAsStream("rem.png"));



            brPane.setBackground( new Background(new BackgroundImage(image , null ,null,null,null) ));


        }
        else if( this.bgColor == "pink" ){

            Image image = new Image(getClass().getResourceAsStream("ram.png"));



            brPane.setBackground( new Background(new BackgroundImage(image , null ,null,null,null) ));
        }
        else{

            Image image = new Image(getClass().getResourceAsStream("draw.jpg"));



            brPane.setBackground( new Background(new BackgroundImage(image , null ,null,null,null) ));





        }



        primaryStage.setTitle("The Drake");
        primaryStage.setScene(new Scene(root));
        primaryStage.show();
    }
}