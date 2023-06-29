package thedrake.ui.GUI;

import javafx.fxml.FXML;
import javafx.stage.Stage;
import thedrake.ui.TheDrakeApp;


public class Controller {

  @FXML
    private javafx.scene.control.Button exitButton;

  @FXML
    private void onExit(){

      Stage stage = (Stage) exitButton.getScene().getWindow();

      stage.close();



  }
  @FXML
  private javafx.scene.control.Button pvpButton;

  @FXML

  private void PvpBattle(){

    this.onExit();

    TheDrakeApp main = new TheDrakeApp();

    try {
      main.start(new Stage());
    } catch (Exception var3) {
      var3.printStackTrace();
    }




  }


}
