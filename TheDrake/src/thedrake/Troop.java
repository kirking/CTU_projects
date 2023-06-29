package thedrake;

import java.util.List;

public class Troop
{
 private final String name;
 private final Offset2D aversPivot;
 private final Offset2D reversPivot;
 private final List<TroopAction> aversAction;
 private final List<TroopAction> reversAction;

    public Troop(String name, Offset2D aversPivot, Offset2D reversPivot, List<TroopAction> aversAction, List<TroopAction> reversAction)
    {
        this.name = name;
        this.aversPivot = aversPivot;
        this.reversPivot = reversPivot;
        this.aversAction = aversAction;
        this.reversAction = reversAction;
    }

    public Troop(String name, Offset2D pivot, List<TroopAction> aversAction, List<TroopAction> reversAction )
    {
              this.name = name;
              this.aversPivot = pivot;
              this.reversPivot = pivot;
              this.aversAction = aversAction;
              this.reversAction = reversAction;
    }

    public Troop(String name, List<TroopAction> aversAction, List<TroopAction> reversAction)
    {

        final Offset2D pos=new Offset2D(1,1);
        this.name = name;
        this.aversPivot = pos;
        this.reversPivot = pos;
        this.aversAction = aversAction;
        this.reversAction = reversAction;

    }

    public List<TroopAction> actions(TroopFace face)
    {

        if( face == TroopFace.AVERS )
            return aversAction;

        return reversAction;


    }





  public String name(){
        return this.name;


  }

 public Offset2D pivot(TroopFace face){
          if (face==TroopFace.AVERS)
             return this.aversPivot;
          else
              return this.reversPivot;
    }



}
