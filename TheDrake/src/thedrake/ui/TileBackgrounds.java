package thedrake.ui;

import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.paint.Color;
import thedrake.*;

public class TileBackgrounds {


    public static final Background EMPTY_BG = new Background(
            new BackgroundFill(new Color(0.5, 0.5, 0.5, 0.5), null, null));

    private final Background mountainBg;


    public TileBackgrounds() {
        Image img = new Image(getClass().getResourceAsStream("/assets/cliff7.png"));

        this.mountainBg = new Background(
                new BackgroundImage(img, null, null, null, null));
    }

    public Background get(Tile tile) {
        if (tile.hasTroop()) {
            TroopTile armyTile = ((TroopTile) tile);
            return getTroop(armyTile.troop(), armyTile.side(), armyTile.face());
        }

        if (tile == BoardTile.MOUNTAIN) {
            return mountainBg;
        }

        Image newImg = new Image(getClass().getResourceAsStream("/assets/grass.png"));

        return new Background( new BackgroundImage( newImg , null , null , null , null) );
    }

    public Background getTroop(Troop info, PlayingSide side, TroopFace face) {
        TroopImageSet images = new TroopImageSet(info.name());
        BackgroundImage bgImage = new BackgroundImage(
                images.get(side, face), null, null, null, null);

        return new Background(bgImage);
    }
}
