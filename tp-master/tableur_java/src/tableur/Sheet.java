/*
 * Cr�� le 6 oct. 2004
 */
package tableur;

import java.util.HashMap;
import java.util.Map;

/**
 * @author lassalle
 */
public class Sheet {
    private final Map<String,Cellule> theSheet = new HashMap<String,Cellule>();
    
    /**
     * @param args
     */
    public static void main(String[] args) {
    }
    
    /**
     * @param theCell
     * @return
     */
    private Cellule getTheCell(String theCell) {
        theCell = theCell.toUpperCase();
        if (theSheet.containsKey(theCell)) {
            return theSheet.get(theCell);
        }
        return null;
    }
    
    /**
     * @param theCell
     * @param string
     */
    public void put(String theCell, String theValue) {
        theSheet.put(theCell.toUpperCase(), Cellule.newCellule(theValue, this));
    }
    
    /**
     * @param string
     * @return
     */
    public String get(String theCell) {
        Cellule cel = getTheCell(theCell); 
        if (cel != null) {
            return cel.evaluer();
        }
        return "";
    }
    
    /**
     * @param theCell
     * @return
     */
    public String getLiteral(String theCell) {
        Cellule cel = getTheCell(theCell);
        if (cel == null) {
            return "";
        }
        return cel.toString();
    }
}
