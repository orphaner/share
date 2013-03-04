/*
 * Created on 8 déc. 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package test;

import tableur.Sheet;
import tableur.SheetFrame;
import tableur.SheetTableModel;

import junit.framework.TestCase;

/**
 * @author nicolas
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class AdditionalNicoTesting extends TestCase {
    private Sheet sheet;
    private SheetFrame frame;
    private SheetTableModel model;
    
    public AdditionalNicoTesting(String name) {
        super(name);
    }
    protected void setUp() throws Exception {
        sheet = new Sheet();
        model = new SheetTableModel(sheet);
        frame = new SheetFrame(model);
    }
    
    public void testThatSpacesAreAllOK () {
        sheet.put("A1", "= 1+ 45 + B3 ");
        sheet.put("B3", "=45");
        assertEquals("spaces ok", "91", sheet.get("A1"));
    }
    
    public void testMinusCell () {
        sheet.put("a1", "4");
        assertEquals("minus insert", "4", sheet.get("A1"));
        sheet.put("B1", "4");
        assertEquals("minus insert", "4", sheet.get("b1"));
    }
    
    public void testLabelAlwaysUpdated () {
        frame.table.changeSelection (0, 1, false, false);
        assertEquals("A1", frame.label.getText());
        
        frame.table.changeSelection (0, 2, false, false);
        assertEquals("A2", frame.label.getText());
    }
    
    public void testTextIntoFormula () {
        sheet.put("A1", "=(15+5)domage!!+2");
        assertEquals("c'est vraiment domage !", "#Error", sheet.get("A1"));
    }
}
