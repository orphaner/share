/*
 * Créé le 16 oct. 2004
 *
 */
package tableur;

/**
 * @author nicolas
 *
 */
public class CelluleTexte extends Cellule {
    
    /**
     * @param theValue
     */
    public CelluleTexte(String theValue) {
        super(theValue);
    }
    
    public String evaluer (){
        return theValue;
    }
}

