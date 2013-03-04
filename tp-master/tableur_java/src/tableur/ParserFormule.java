/*
 * Created on 22 oct. 2004
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
public class ParserFormule {
    public static final String ERROR = "#Error";
    public static final String DIV0 = "#Div0";
    public static final String CIRCULAR = "#Circular";
    
    private final String formule;
    private final Sheet theSheet;
    private final StringTokenizer st;
    
    private String token = "";
    
    /**
     * 
     * @param formule
     * @param theSheet
     */
    public ParserFormule (String formule, Sheet theSheet) {
        this.formule = formule;
        this.theSheet = theSheet;
        this.st = new StringTokenizer (this.formule, "()/*-+ ", true);
    }
    
    /**
     * @return
     */
    public String evaluate() {
        // System.out.println("evaluate <<"+formule+">>");
        try {
            return String.valueOf(parseE());
        }
        catch (IllegalArgumentException e) {
            return ERROR;
        }
        catch (ArithmeticException e) {
            return e.getMessage();
        }
    }
    
    /**
     * Met à jour le token courant
     * Passe les espaces qui ne sont pas des tokens !
     */
    private void nextToken() {
        if (st.hasMoreTokens()) {
            token = st.nextToken();
            
            // Suppression des espaces
            while (token.equals(" ") && st.hasMoreTokens())
                token = st.nextToken();
        }
        else
            token = "";
    }
    
    /**
     * @return
     */
    private int parseE() {
        int retour = parseT();
        while (token.equals("+") || token.equals("-")){
            if (token.equals("+")){
                retour = retour + parseT();
            }
            else {
                retour = retour - parseT();
            }
        }
        return retour;
    }
    
    /**
     * @return
     */
    private int parseT() {
        int retour = parseU();
        while (token.equals("*") || token.equals("/")){
            if (token.equals("*")){
                retour = retour * parseU();
            }
            else {
                int div = parseU();
                if (div == 0)
                    throw new ArithmeticException(DIV0);
                retour = retour / div;
            }
        }
        return retour;
    }
    
    /**
     * @return
     */
    private int parseU() {
        nextToken();
        
        if (token.equals("-")) {
            nextToken();
            return -parseF();
        }
        else if (token.equals("+")) {
            nextToken();
            return parseF();
        }
        return parseF();
    }
    
    /**
     * @return
     */
    private int parseF() {
        int retour = 0;
        
        // Nombre
        if (Cellule.isNumeric(token)) {
            retour = Integer.parseInt(token);
        }
        
        // Parenthèse ouvrante
        else if (token.equals("(")) {
            retour = parseE();
            if (token.equals("") || !token.equals(")")) {
                throw new IllegalArgumentException (ERROR);
            }
        }
        
        // Référence à une Cellule
        else if (isRefCellule(token)){
            retour = Integer.parseInt(theSheet.get(token));
        }
        
        // Erreur sinon
        else
            throw new IllegalArgumentException (ERROR);
        
        nextToken();
        return retour;
    }
    
    /**
     * Détermine si un token est de la forme d'une celulle
     * (une suite d'au moins une lettre suivie d'une suite d'au moins un chiffre)
     * @param token2 Le token à analyser
     * @return True si le forme est de la forme d'une cellule. False sinon.
     */
    private boolean isRefCellule(String token2) {
        Pattern p = Pattern.compile("[a-zA-Z]*[0-9]*");
        Matcher m = p.matcher(token2);
        return m.matches();
    }
}
