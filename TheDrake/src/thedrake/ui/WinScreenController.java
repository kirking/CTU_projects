package thedrake.ui;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.stage.Stage;
import thedrake.ui.GUI.Controller;
import thedrake.ui.GUI.JavaFXmain;

public class WinScreenController {
    @FXML
    private Button okButton;

    public WinScreenController() {
    }

    @FXML
    private void onOK() {
        Stage stage = (Stage)this.okButton.getScene().getWindow();
        stage.close();
        JavaFXmain newScreen = new JavaFXmain();

        try {
            newScreen.start(new Stage());
        } catch (Exception var4) {
            var4.printStackTrace();
        }

    }
}
