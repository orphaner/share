/*
 * Créé le 5 oct. 2004
 *
 * TODO Pour changer le modèle de ce fichier généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
package test;

import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableModel;

import junit.framework.TestCase;
import tableur.Sheet;
import tableur.SheetFrame;
import tableur.SheetTableModel;

/**
 * @author leberre
 *
 * TODO Pour changer le modèle de ce commentaire de type généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
public class Challenge5 extends TestCase {
//  One of the first XPlorations articles I wrote was to address the question, 
//  "Can you do a GUI test-first?" My answer there was, "Yes, but..." 
//  and this exercise might lead you to the same opinion.
//  
//  The goal is to make a simple GUI that looks sort of like this:
//  
//  +--------+---------------+------+
//  | Label  |  Text entry   |  OK  |
//  +--------+---------------+------+
//  |                              ^|
//  |        Grid (JTable)         ||
//  |                              v|
//  +-------------------------------+
//  
//  The idea is that if you click in the grid, the label tells which cell you're in, 
//  and the text entry field contains the literal value of that cell. You can edit it, 
//  and when you click "OK", it puts the value back in the cell. Then the whole spreadsheet 
//  updates accordingly. With this, you should have a minimalist but working spreadsheet.
//  Quick Design
//  I'll put everything in a subclass of JFrame, using the Swing objects JLabel, JTextField, 
//  JButton, and JTable. In my frame, I'll have global variables for each of these components. 
//  (I won't bother exposing intermediate panels or other stuff not important outside the class.)
//  
//  Most of the problem will be setup and hookup. I'll want to hook the table to the model, 
//  and set up listeners so that the label and textfield know when the selection has changed. 
//  I'll need a listener on the ok button to tell when to update the grid with the edited value. 
//  And all these objects have various configuration options to set up as well.
//  
//  Timothy Wall has pointed out that there are tools designed to test GUIs: 
//  http://abbot.sf.net, http://jemmy.netbeans.org, and http://jfcunit.sf.net. 
//  You might want to give one of these a try and compare the result.
//  Challenge Tests
//  Here are my declarations and setUp routine for my test now:
    
    Sheet sheet;
    TableModel table;
    SheetTableModel model;	// New for part 5
    SheetFrame frame;	// New for part 5
    
    public void setUp() {
        sheet = new Sheet();
        table = new SheetTableModel (sheet);  
        
        model = new SheetTableModel(sheet);
        frame = new SheetFrame(model);
    }
    
//  Here are the tests:
    
    public void testThatFrameHasRightParts () {
        assertNotNull(frame.table);
        assertNotNull(frame.label);
        assertNotNull(frame.editor);
        assertNotNull(frame.okButton);
        assertSame(model, frame.table.getModel());
    }
    
    public void testThatRowAndColumnSelectionAllowed() {
        assertTrue(frame.table.getRowSelectionAllowed());
        assertTrue(frame.table.getColumnSelectionAllowed());
    }
    
    public class TestSelectionListener implements ListSelectionListener {
        public boolean wasNotified = false;
        
        public TestSelectionListener() {}
        
        public void valueChanged(ListSelectionEvent e) {
            wasNotified = true;            
        }      
    }
    
//  I expect this test to pass; it verifies how I think listeners work.
//  You might call it a spike and omit it.
    
    public void testThatSelectionsNotifyListeners() {
        TestSelectionListener listener = new TestSelectionListener();
        frame.table.getSelectionModel().addListSelectionListener(listener);
        
        assertTrue (!listener.wasNotified);  
        
        frame.table.changeSelection (3, 2, false, false);
        
        assertTrue (listener.wasNotified);
        
        
        listener.wasNotified = false;
        frame.table.changeSelection (1, 1, false, false);
        assertTrue (listener.wasNotified);
    }
    
//  If you need info on hooking up a selection listener, see 
//  http://java.sun.com/docs/books/tutorial/uiswing/components/table.html#selection
    
    public void testThatLabelIsUpdatedWhenSelectionChanges() {
        assertEquals("", frame.label.getText());
        
        frame.table.changeSelection (0, 1, false, false);
        assertEquals("A1", frame.label.getText());
        
        frame.table.changeSelection (10, 10, false, false);
        assertEquals("J11", frame.label.getText());
    }
    
//  You might add a main() routine to SheetFrame and
//  see how the GUI is looking.
    
    public void testThatEditorSeesLiteralValue() {
        model.setValueAt("=7", 1, 2);
        frame.table.changeSelection(1,2,false,false);
        
        assertEquals("=7", frame.editor.getText());
    }
    
    
//  We would like to have a way to programmatically let the 
//  text field click "Enter", but I don't see a mechanism.
//  So we'll use the okButton instead.
    
    public void testThatEditedValueGetsSaved() {
        model.setValueAt("=7", 1, 1);
        frame.table.changeSelection(1,1,false,false);
        
        frame.editor.setText("=8");
        frame.okButton.doClick();
        assertEquals("=8", frame.model.getLiteralValueAt(1,1));
        assertEquals("8", frame.model.getValueAt(1,1));
    }
    
    
    public void testThatValuePropagationWorks () {
        frame.model.setValueAt("7", 0,1);
        frame.model.setValueAt("=A1+2", 2,2);
        assertEquals("9", frame.model.getValueAt(2,2));
        assertEquals("=A1+2", frame.model.getLiteralValueAt(2,2));
        
        frame.model.setValueAt("10", 0,1);
        assertEquals("12", frame.model.getValueAt(2,2));
    }
    
    
//  See discussion below on acceptance tests.
    public void testAcceptanceTest1() {
        SheetTableModel model;
        SheetFrame frame;
        
        model = new SheetTableModel(new Sheet());
        frame = new SheetFrame(model);
        
        frame.table.changeSelection(0,1,false,false);   // A1
        frame.editor.setText("8");
        frame.okButton.doClick();
        
        frame.table.changeSelection(1,1,false,false);   // A2
        frame.editor.setText("=A1*A1+A1");
        frame.okButton.doClick();
        
        assertEquals("72", frame.table.getValueAt(1,1));
        
        frame.table.changeSelection(0,1,false,false);   // A1
        frame.editor.setText("5");
        frame.okButton.doClick();
        
        assertEquals("30", frame.table.getValueAt(1,1));
    }
    
    
//  Discussion
    //	What I've found is that GUI testing works "ok" for some things but there
    // tends not to be an easy way to do other things. For example, I didn't see
    // an easy way to programmatically click "Enter" in the edited box. (I know
    // you can set up a Robot and fool with events, but I wasn't willing to try
    // that hard.) Another example is the difficulty of dealing with pop-up
    // menus, tooltips, dialogs, etc.
    //
    //	The other problem I have is that I just don't think of everything
    // important without actually seeing the GUI grow. So even if I use
    // test-first, I'll mix it with running the application and seeing how it
    // looks. (For example, we didn't address column widths, or seriously look
    // at what happens when you select multiple cells, and my solution doesn't
    // leave the grid element selected after an edit.)
    //
    //	I try to keep the GUI work limited to direct setup and hookup of objects.
    // It can be very tempting to say "oh, this is GUI so I don't need to test
    // it," but don't over-do that. Make sure that the GUI is not making
    // application-level or algorithmic decisions.
    //
    //	I do sometimes end up creating programmatic acceptance tests, like the
    // last test. I try to minimize my need for these because they're a fair bit
    // of work to program, and the customer can't really look at them and verify
    // them. In this case, the test checks the hookup, which is ok. But for
    // other spreadsheet tests, I might automate something that reads an input
    // file and a "golden" result, then compares to the actual contents of the
    // sheet. In this way I could check formulas, circularity, etc. without
    // having to hand-code so much.
}
