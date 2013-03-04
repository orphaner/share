/*
 * Créé le 16 oct. 2004
 *
 */
package tableur;

import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author nicolas
 *  
 */
public class CelluleFormule extends Cellule {
    public static final String ERROR    = "#Error";
    public static final String CIRCULAR = "#Circular";
    
    private final Sheet theSheet;
    
    private StringTokenizer st;
    private String token = "";
    private boolean parcouru;
    
    /**
     * @param theValue La valeur de la cellule formule
     */
    public CelluleFormule (String theValue, Sheet sheet) {
        super (theValue);
        this.theSheet = sheet;
        parcouru = false;
    }
    
    /**
     * 
     */
    public String evaluer () {
        if (parcouru == true) {
            return CIRCULAR;
        }
        
        String retour;
        parcouru = true;
        st = new StringTokenizer (this.theValue.substring (1), "()/*-+ ", true);
        
        try {
            //retour = String.valueOf (parseE ());
            float f = parseE ();
            Float result = new Float (f);
            if (result.floatValue() - result.intValue() == 0) {
                retour = String.valueOf (result.intValue());
            }
            else {
                retour = String.valueOf (result);
            }
        }
        /* Pour les underflow & overflow */
        catch (NumberFormatException e) {
            retour = ERROR;
        }
        catch (IllegalArgumentException e) {
            retour = e.getMessage();
        }
        catch (ArithmeticException e) {
            retour = e.getMessage();
        }
        parcouru = false;
        return retour;
    }
    
    /**
     * Met à jour le token courant
     * Passe les espaces qui ne sont pas des tokens !
     */
    private void nextToken () {
        if (st.hasMoreTokens ()) {
            token = st.nextToken ();
            
            // Suppression des espaces
            while (token.equals (" ") && st.hasMoreTokens ()) {
                token = st.nextToken ();
            }
        }
        else {
            token = "";
        }
    }
    
    /**
     * @return
     */
    private float parseE() {
        float retour = parseT ();
        while (token.equals ("+") || token.equals ("-")) {
            if (token.equals ("+")) {
                retour = retour + parseT ();
            }
            else {
                retour = retour - parseT ();
            }
        }
        return retour;
    }
    
    /**
     * @return
     */
    private float parseT () {
        float retour = parseU ();
        while (token.equals ("*") || token.equals ("/")) {
            if (token.equals ("*")) {
                retour = retour * parseU ();
            }
            else {
                float div = parseU ();
                if (div == 0) {
                    throw new ArithmeticException (ERROR);
                }
                retour = retour / div;
            }
        }
        return retour;
    }
    
    /**
     * @return
     */
    private float parseU() {
        nextToken();
        
        if (token.equals ("-")) {
            nextToken();
            return -parseF ();
        }
        else if (token.equals ("+")) {
            nextToken ();
            return parseF ();
        }
        return parseF ();
    }
    
    /**
     * @return
     */
    private float parseF () {
        float retour = 0;
        // Nombre
        if (Cellule.isNumeric (token)) {
            retour = Float.parseFloat (token);
        }
        
        // Parenthèse ouvrante
        else if (token.equals ("(")) {
            retour = parseE ();
            if (token.equals ("") || !token.equals (")")) {
                throw new IllegalArgumentException (ERROR);
            }
        }
        
        // Référence à une Cellule
        else if (isRefCellule (token) && !token.equals ("")) {
            String refCel = theSheet.get (token);
            if (refCel.equals (CIRCULAR)) {
                throw new IllegalArgumentException (CIRCULAR);
            }
            retour = Float.parseFloat (refCel);
        }
        
        // Erreur sinon
        else {
            throw new IllegalArgumentException (ERROR);
        }
        
        nextToken ();
        return retour;
    }
    
    /**
     * Détermine si un token est de la forme d'une celulle
     * (une suite d'au moins une lettre suivie d'une suite d'au moins un chiffre)
     * @param token2 Le token à analyser
     * @return True si le forme est de la forme d'une cellule. False sinon.
     */
    private boolean isRefCellule (String token2) {
        Pattern p = Pattern.compile ("[a-zA-Z]*[0-9]*");
        Matcher m = p.matcher (token2);
        return m.matches ();
    }
}
