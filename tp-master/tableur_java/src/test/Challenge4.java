/*
 * Créé le 5 oct. 2004
 *
 * TODO Pour changer le modèle de ce fichier généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
package test;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.TableModel;

import junit.framework.TestCase;
import tableur.Sheet;
import tableur.SheetTableModel;

/**
 * @author leberre
 *
 * TODO Pour changer le modèle de ce commentaire de type généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
public class Challenge4 extends TestCase {
    //	Challenge Tests
    //	We'll start by using a feature of JUnit that we haven't used before: a
    // setUp() method. To use this, declare your variables at the class level,
    // and initialize them in the setUp() method. There's a corresponding
    // tearDown() method that we won't need, but which can be useful for some
    // tests. JUnit works like this: call setUp(), call one of your tests, call
    // tearDown(), call setUp(), call your next test, call tearDown(), etc. So,
    // each test can assume it's got things just as setUp() left them.
    //	Quick Design
    //	The easiest thing to use in the GUI will be a JTable, which provides the
    // 2-d grid we expect to see. The best way to use a JTable is usually to
    // give it a TableModel. The TableModel acts as a wrapper between the
    // GUI-oriented JTable and your object.
    //
    //	The easiest way to create a TableModel is to subclass AbstractTableModel,
    // so that's what we'll do. The TableModel is 0-based, so we'll use column 0
    // to hold the row's index, and we'll store spreadsheet row n in
    // TableModel's row n-1.
    
    Sheet sheet;
    TableModel table;
    
    public void setUp() {
        sheet = new Sheet();
        table = new SheetTableModel (sheet);        
    }
    
//  As usual, do one test at a time and refactor after each.
    
//  For now, we're willing to hard-code a maximum spreadsheet size.
//  A future story can deal with this.
    
    int LAST_COLUMN_INDEX = 49;
    int LAST_ROW_INDEX = 99;
    
    public void testTableModelRequiredOverrides() {
        assertTrue (table.getColumnCount() > LAST_COLUMN_INDEX);
        assertTrue (table.getRowCount() > LAST_ROW_INDEX);
        assertEquals ("", table.getValueAt(10,10));
    }
    
//  Take a look at AbstractTableModel's documentation before doing this test.
    
    public void testColumnNames() {
        assertEquals ("", table.getColumnName(0));
        assertEquals ("A", table.getColumnName(1));
        assertEquals ("Z", table.getColumnName(26));
        assertEquals ("AW", table.getColumnName(LAST_COLUMN_INDEX));
    }
    
    public void testThatColumn0ContainsIndex() {
        assertEquals ("1", table.getValueAt(0,0));
        assertEquals ("50", table.getValueAt(49, 0));
        assertEquals ("100", table.getValueAt(LAST_ROW_INDEX,0));
    }
    
//  Remember, one test at a time, followed by refactoring.
    
    public void testThatMainColumnsHaveContents() {
        sheet.put ("A1", "upper left");
        assertEquals ("upper left", table.getValueAt(0,1));
        
        sheet.put ("A100", "lower left");
        assertEquals ("lower left", table.getValueAt(LAST_ROW_INDEX, 1));
        
        sheet.put ("AW1", "upper right");
        assertEquals ("upper right", table.getValueAt(0, LAST_COLUMN_INDEX));
        
        sheet.put ("AW100", "lower right");
        assertEquals ("lower right", table.getValueAt(LAST_ROW_INDEX, LAST_COLUMN_INDEX));
    }
    
    public void testThatStoresWorkThroughTableModel() {
        table.setValueAt("21", 0, 1);
        table.setValueAt("=A1", 1, 1);
        
        assertEquals("21", table.getValueAt(0,1));
        assertEquals("21", table.getValueAt(1,1));
        
        table.setValueAt("22", 0, 1);
        assertEquals("22", table.getValueAt(0,1));
        assertEquals("22", table.getValueAt(1,1));
    }
    
    
    //	 We've established that the table model can get and set values.
    //	 But JTable uses an event notification mechanism to find out
    //	 about the changes.
    
    //	 To test this, we'll introduce a test helper class. It's a very
    //	 simple listener, and will assure us that notifications are
    //	 sent when changes are made.
    
    //	 There's a couple of design decisions implicit here. One is that
    //	 we won't attempt to be specific about which cells change; we'll
    //	 just say that the table data has changed and let JTable refresh
    //	 its view of whichever cells it wants. (Because of cell dependencies,
    //	 changes in one cell could potentially no others, all others,
    //	 or anything in between.) We might revisit this decision during
    //	 performance tuning, and try to issue finer-grained notifications.
    
    //	 The other decision is that we have no mechanism for our Sheet
    //	 to tell the table model about changes. So changes will either need
    //	 to come in through the table model, or we'll have to add some
    //	 notification mechanism to Sheet. For now, just make changes through the table model.
    
    public class TestTableModelListener implements TableModelListener {
        public boolean wasNotified = false;
        
        public void tableChanged(TableModelEvent e) {wasNotified = true;}
    }
    
    public void testThatTableModelNotifies() {
        TestTableModelListener listener = new TestTableModelListener();
        table.addTableModelListener (listener);
        assertTrue (!listener.wasNotified);
        
        table.setValueAt("22", 0, 1);
        
        assertTrue (listener.wasNotified);
    }
    
    
    //	 Note the cast in our test here. Previous tests have been straight
    //	 implementations of TableModel functions; now we're saying that 
    //	 our model has some extra functions. We'll face a small tradeoff later
    //	 when we want access to the feature: if we get the model back from JTable,
    //	 we'll have to cast it; if we don't want to cast it we'll have to
    //	 track it somewhere.
    
    public void testThatSheetTableModelCanGetLiteral() {
        sheet.put("A1", "=7");
        String contents = ((SheetTableModel)table).getLiteralValueAt(0, 1);
        
        assertEquals("=7", contents);
    }
    
    //	 We've left isCellEditable() false, on the assumption that the way to edit
    //	 the cell is to go to a textbox provided for that purpose (rather than
    //	 in place). 
}
