/*
 * Créé le 4 déc. 2004
 *
 */
package tableur;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author nicolas
 */
public class CSVFile {
    private final int MAX_LIGNES = 100;
    private final int MAX_COLONES = 50; 
    private final static Character CELL_SEPARATOR = new Character (',');
    private Sheet theSheet;
    
    /**
     * Constructeur
     */
    public CSVFile() {
    }
    
    /**
     * @param fileName
     * @return
     */
    public Sheet readFile (String fileName) {
        theSheet = new Sheet ();
        FileReader fileReader;
        try {
            fileReader = new FileReader (fileName); 
        }
        catch (IOException e) {   
            System.err.println("Error opening CSV file: " + e.toString());
            return null;
        }        
        
        if (decouperEnLignes (fileReader) == false) {
            return null;
        }
        return theSheet;
    }
    
    /**
     * @param fileReader
     * @return
     */
    private boolean decouperEnLignes(FileReader fileReader) {
        BufferedReader buffReader;
        try {
            buffReader = new BufferedReader(fileReader);
            String ligneData = buffReader.readLine();
            int rowIndex = 0;
            while (ligneData != null) {
                if (decouperLigneEnColones (ligneData, rowIndex) == false) {
                    return false;
                }
                ligneData = buffReader.readLine();
                rowIndex++;
            }
        }
        catch (IOException e) {   
            System.err.println("Error reading CSV file: " + e.toString());
            return false;
        } 
        return true;
    }
    
    /**
     * @param ligneData
     * @param rowIndex
     * @return
     */
    private boolean decouperLigneEnColones (String ligneData, int rowIndex) {
        Pattern p = Pattern.compile ("\"([^\"]|\"\")*\"("+CELL_SEPARATOR+"|$)|[^"+CELL_SEPARATOR+"]*("+CELL_SEPARATOR+"|$)");
        Matcher m = p.matcher(ligneData);
        int columnIndex = 1;
        while (m.find()) {
            String s = m.group ();
            if (!s.equals("")) {
                if (s.endsWith(CELL_SEPARATOR.toString())) {
                    s = s.substring(0,s.length()-1);
                }
                if (s.startsWith("\"") && s.endsWith("\"")){
                    s = s.substring(1,s.length()-1);
                }
                else {
                    return false;
                }
                theSheet.put(SheetTableModel.getCellName(rowIndex, columnIndex), s);
                columnIndex++;
            }
        }
        return true;
    }
    
    /**
     * @param model
     * @param fileName
     * @return
     */
    public boolean writeFile (SheetTableModel model, String fileName) {
        FileWriter fileWriter = ouvrirFichierEcriture(fileName);
        if (fileWriter == null) {
            return false;
        }
        try {
            BufferedWriter writer = new BufferedWriter(fileWriter);
            for (int i = 0; i < MAX_LIGNES; i++) {
                ecrireColonesDansFichier(model, writer, i);
                if (i + 1 < MAX_LIGNES) {
                    writer.write("\n");
                }
            }
            writer.flush();
            writer.close();
        }
        catch (IOException e) {
            System.err.println("Error writing CSV file: " + e.toString());
            return false;
        }
        return true;
    }
    
    /**
     * @param model
     * @param writer
     * @param i
     * @throws IOException
     */
    private void ecrireColonesDansFichier (SheetTableModel model, BufferedWriter writer, int i) throws IOException {
        int nbVide = 0;
        for (int j = 1; j < MAX_COLONES; j++) {
            String valeur =  model.getLiteralValueAt(i, j);
            if (valeur.equals ("")) {
                nbVide++;
            }
            else {
                for (int k = 0 ; k < nbVide ; k++) {
                    writer.write ("\"\""+CELL_SEPARATOR);
                }
                writer.write ("\""+valeur+"\"");
                if (j + 1 < MAX_COLONES) {
                    writer.write(CELL_SEPARATOR.toString());
                }
                nbVide = 0;
            }
        }
    }

    /**
     * @param fileName
     * @return
     */
    private FileWriter ouvrirFichierEcriture (String fileName) {
        FileWriter fileWriter;
        try {
            fileWriter = new FileWriter (fileName);
            return fileWriter;
        }
        catch (IOException e) {
            System.err.println("Error opening CSV file: " + e.toString());
            return null;
        }
    }

    public static void main (String args[]) {
        CSVFile csv = new CSVFile();
        csv.readFile("/home/nicolas/Desktop/file.csv");
    }
}
