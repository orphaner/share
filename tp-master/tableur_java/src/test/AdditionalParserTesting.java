/*
 * Créé le 13 oct. 2004
 *
 * TODO Pour changer le modèle de ce fichier généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
package test;

import tableur.Sheet;

import junit.framework.TestCase;

/**
 * @author leberre
 * 
 * Tests additionnels proposés par Mike Clark pour le parser.
 */
public class AdditionalParserTesting extends TestCase {

		// utilisation d'une "Fixture"
	
		private Sheet sheet;
		
	    public AdditionalParserTesting(String name) {
	        super(name);
	    }

	    
	/* 
	 * Cette méthode est appelée avant chaque test.
	 * (avant chaque appel de methode testXXX)
	 * Elle permet de factoriser le code commun entre
	 * les tests.
	 */
	protected void setUp() throws Exception {
		sheet = new Sheet();
	}
	    public void testNumber() {
			sheet.put("A1","=1");
	        assertEquals("1", sheet.get("A1"));
	    }

	    public void testParensAndNumber() {
	    	sheet.put("A1","=(12)");
	        assertEquals("12", sheet.get("A1"));
	    }

	    public void testAdd2Numbers() {
	    	sheet.put("A1","=1+2");
	        assertEquals("3", sheet.get("A1"));
	    }

	    public void testAdd3Numbers() {
	    	sheet.put("A1","=1+2+3");
	        assertEquals("6", sheet.get("A1"));
	    }

	    public void testSubtract2Numbers() {
	    	sheet.put("A1","=2-1");
	        assertEquals("1", sheet.get("A1"));
	    }

	    public void testSubtract3Numbers() {
	        sheet.put("A1","=1+2-1");
	        assertEquals("2", sheet.get("A1"));
	    }

	    public void testSubtract3NumbersLeftAssociation() {
	    	sheet.put("A1","=1-2-1");
	        assertEquals("-2", sheet.get("A1"));
	    }
	    
	    public void testDivide2Numbers() {
	    	sheet.put("A1","=10/5");
	        assertEquals("2", sheet.get("A1"));
	    }

	    public void testDivide3Numbers() {
	    	sheet.put("A1","=10/5*2");
	        assertEquals("4", sheet.get("A1"));
	    }

	    public void testAddMultiply() {
	    	sheet.put("A1","=1+2*3");
	        assertEquals("7", sheet.get("A1"));
	    }

	    public void testSubtractDivide() {
	    	sheet.put("A1","=3-10/5");
	        assertEquals("1", sheet.get("A1"));
	    }

	    public void testMultiplyAdd() {
	    	sheet.put("A1","=1*2+3");
	        assertEquals("5", sheet.get("A1"));
	    }

	    public void testMultiplyAddWithParens() {
	    	sheet.put("A1","=1*(2+4)");
	        assertEquals("6", sheet.get("A1"));
	    }

	    public void test2TermsWithoutParens() {
	    	sheet.put("A1","=1*2+3*4");
	        assertEquals("14", sheet.get("A1"));
	    }

	    public void test2TermsWithParens() {
	    	sheet.put("A1","=((1+2)*(3+4))");
	        assertEquals("21", sheet.get("A1"));
	    }

	    public void testComplex() {
	    	sheet.put("A1","=7*(2+3)*((((2+1))))");
	        assertEquals("105", sheet.get("A1"));
	    }

	    public void testSpaces() {
	    	sheet.put("A1","=7 + 2 * 3");
	        assertEquals("13", sheet.get("A1"));
	    }

	    // 
	    // Error Conditions
	    //

	    public void testParens() {
	    	sheet.put("A1","=(())");
	        assertEquals("#Error", sheet.get("A1"));
	    }

	    public void testOperator() {
	    	sheet.put("A1","=+");
	        assertEquals("#Error", sheet.get("A1"));
	    }
}
