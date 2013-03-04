/*
 * Created on 8 janv. 2005
 *
 */
package gui;

import java.util.ArrayList;
import java.util.List;

import javax.swing.table.AbstractTableModel;

/**
 * @author nicolas
 *
 */
public class ListePartieTableModel extends AbstractTableModel {

    /**
     * Commentaire pour <code>gameList</code>
     */
    private List<Partie> gameList = new ArrayList<Partie> (0);

    /**
     * Commentaire pour <code>columnNames</code>
     */
    private static final String[] columnNames = { "Nombre de villes", "Nombre d'armées", "Ville dispo",
            "Créateur" };

    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getColumnName(int)
     */
    public String getColumnName (int col) {
        return columnNames[col];
    }

    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getRowCount()
     */
    public int getRowCount () {
        return gameList.size ();
    }

    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getColumnCount()
     */
    public int getColumnCount () {
        return 4;
    }

    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getValueAt(int, int)
     */
    public Object getValueAt (int rowIndex, int columnIndex) {
        Partie game = gameList.get (rowIndex);
        switch (columnIndex) {
        case 0:
            return game.getNbVilles ();
        case 1:
            return game.getNbArmees ();
        case 2:
            return game.getCampLibre ();
        case 3:
            return game.getCreateur ();
        default:
            return "";
        }
    }

    /**
     * @param list
     */
    public void setGameList (List<Partie> list) {
        gameList = new ArrayList<Partie> (list);
        fireTableStructureChanged ();
    }

    /**
     * 
     */
    public void reset () {
        gameList = new ArrayList<Partie> (0);
        fireTableStructureChanged ();
    }

    /**
     * @param rowIndex
     * @return
     */
    public int getGameId (int rowIndex) {
        if (rowIndex >= 0) {
            return gameList.get (rowIndex).getId ();
        }
        return 0;
    }

    /**
     * @param rowIndex
     * @return
     */
    public Partie getGame (int rowIndex) {
        if (rowIndex >= 0) {
            return gameList.get (rowIndex);
        }
        return null;
    }
}
