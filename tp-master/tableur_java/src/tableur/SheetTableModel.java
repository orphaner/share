/*
 * Créé le 28 oct. 2004
 *
 */
package tableur;

import javax.swing.table.AbstractTableModel;

/**
 * @author lassalle
 *
 */
public class SheetTableModel extends AbstractTableModel {
    private final int MAX_LIGNES = 100;
    private final int MAX_COLONES = 50; 
    private Sheet theSheet;
    
    /**
     * @param sheet
     */
    public SheetTableModel(Sheet sheet) {
        theSheet = sheet;
    }
    
    public void changeSheet (Sheet sheet) {
        theSheet = sheet;
        fireTableStructureChanged();
    }
    
    /**
     * 
     */
    public int getRowCount() {
        return MAX_LIGNES;
    }
    
    /**
     * 
     */
    public int getColumnCount() {
        return MAX_COLONES;
    }
    
    /**
     * 
     */
    public String getColumnName (int columnIndex) {
        return getColumnNamePerform (columnIndex);
    }
    
    /**
     * @param columnIndex
     * @return
     */
    private static String getColumnNamePerform(int columnIndex) {
        if (columnIndex == 0) {
            return "";
        }
        if (columnIndex <= 26) {
            return Character.toString((char)(columnIndex+64));
        }
        return Character.toString((char)(64+(columnIndex/26)))+((char)(64+(columnIndex%26)));
    }

    /**
     * 
     */
    public boolean isCellEditable(int rowIndex, int columnIndex) {
        return false;
    }
    
    /**
     * 
     */
    public String getValueAt (int rowIndex, int columnIndex) {
        String value;
        if (columnIndex == 0) {
            return Integer.toString(rowIndex+1);
        }
        return theSheet.get(getCellName(rowIndex, columnIndex));
    }
    
    /**
     * @param rowIndex
     * @param columnIndex
     * @return
     */
    public static String getCellName (int rowIndex, int columnIndex) {
        return getColumnNamePerform (columnIndex)+Integer.toString(rowIndex+1);
    }
    
    /**
     * 
     */
    public void setValueAt (Object theValue, int rowIndex, int columnIndex) {
        theSheet.put(getCellName(rowIndex, columnIndex),(String)theValue);
        fireTableCellUpdated(rowIndex,columnIndex);
    }
    
    /**
     * 
     * @param rowIndex
     * @param columnIndex
     * @return
     */
    public String getLiteralValueAt (int rowIndex, int columnIndex) {
        return theSheet.getLiteral(getCellName(rowIndex, columnIndex));
    }
}
