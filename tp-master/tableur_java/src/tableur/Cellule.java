/*
 * Créé le 16 oct. 2004
 *
 */
package tableur;

/**
 * @author nicolas
 *
 */
public abstract class Cellule {
    protected String theValue; 
    
    /**
     * @param theValue
     * @return
     */
    private static boolean isFormule (String theValue) {
        return theValue.startsWith ("=");
    }
    
    /**
     * @param theValue
     * @return
     */
    protected static boolean isNumeric (String theValue) {
        try {
            Float.parseFloat (theValue.trim ());
            return true;
        }
        catch (NumberFormatException e) {
            return false;
        }
    }
    
    /**
     * @param sheet TODO
     * @param theValue2
     */
    public Cellule (String theValue) {
        this.theValue = theValue;
    }
    
    /**
     * 
     * @param theValue
     * @return
     */
    public static Cellule newCellule (String theValue, Sheet sheet) {
        if (Cellule.isFormule (theValue)) {
            return new CelluleFormule (theValue, sheet);
        }
        else if (Cellule.isNumeric (theValue)) {
            return new CelluleNumerique (theValue);
        }
        else {
            return new CelluleTexte (theValue);
        }
    }
    
    /**
     * 
     * @return
     */
    public abstract String evaluer();
    
    /**
     * @return
     */
    public String toString () {
        return theValue;
    }
}
