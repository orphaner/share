/*
 * Créé le 16 oct. 2004
 *
 */
package tableur;

/**
 * @author nicolas
 *
 */
public class CelluleNumerique extends Cellule {
    
    /**
     * @param theValue
     */
    public CelluleNumerique(String theValue) {
        super(theValue);
    }
    
    /* (non-Javadoc)
     * @see tableur.Cellule#get()
     */
    public String evaluer() {
        return theValue.trim();
    }
    
}

