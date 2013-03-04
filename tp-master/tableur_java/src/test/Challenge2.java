/*
 * Cr�� le 5 oct. 2004
 *
 */
package test;

import tableur.Sheet;
import junit.framework.TestCase;

/**
 * @author leberre
 *

 */
public class Challenge2 extends TestCase {
//	Implement code for previous test before moving to next one.
	
	public void testFormulaSpec() {
		Sheet sheet = new Sheet();
		sheet.put("B1", " =7"); // note leading space
		assertEquals("Not a formula", " =7", sheet.get("B1"));
		assertEquals("Unchanged", " =7", sheet.getLiteral("B1"));
	}
	
	
//	Next - start on parsing expressions
	
	public void testConstantFormula() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7");
		assertEquals("Formula", "=7", sheet.getLiteral("A1"));
		assertEquals("Value", "7", sheet.get("A1"));
	}
	
//	More formula tests. You may feel the need to make up 
//	additional intermediate test cases to drive your code
//	better. (For example, you might want to test "2*3" 
//	before "2*3*4".) That's fine, go ahead and create them.
//	Just keep moving one test at a time.
	
//	We're doing expressions; you may need to do a spike
//	(investigation) if you're not familiar with parsing.
//	For background, look up "recursive descent" or
//	"operator precedence". (Other techniques can work as well.)
	
//	Order of tests - I'm familiar enough with parsing to think
//	it's probably easiest to do them in this order (highest
//	precedence to lowest). For extra credit, you might redo 
//	this part of the exercise with the tests in a different order 
//	to see what difference it makes.
	public void testParentheses() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=(7)");
		assertEquals("Parends", "7", sheet.get("A1"));
	}
	
	public void testDeepParentheses() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=((((10))))");
		assertEquals("Parends", "10", sheet.get("A1"));
	}

	public void testMultiply() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=2*3*4");
		assertEquals("Times", "24", sheet.get("A1"));
	}
	
	public void testAdd() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=71+2+3");
		assertEquals("Add", "76", sheet.get("A1"));
	}
	
	public void testPrecedence() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7+2*3");
		assertEquals("Precedence", "13", sheet.get("A1"));
	}
	
	public void testFullExpression() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7*(2+3)*((((2+1))))");
		assertEquals("Expr", "105", sheet.get("A1"));
	}
	
//	Add any test cases you feel are missing based on 
//	where your code is now.
	
//	Then try your hand at a few test cases: Add "-" and "/"
//	with normal precedence. 
	
//	Next, error handling.
	
	public void testSimpleFormulaError() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7*");
		assertEquals("ForumlaError", "#Error", sheet.get("A1"));
	}
	
	public void testParenthesisError() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=(((((7))");
		assertEquals("ParenthesisError", "#Error", sheet.get("A1")); 
	}
	
	public void testDivByZeroError () {
	    Sheet sheet = new Sheet();
		sheet.put("A1", "=7/0");
		assertEquals("DivByZeroError", "#Error", sheet.get("A1")); 
	}
	
	public void testPlusPrefixe () {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=+(7*(2+3)*((((2+1)))))");
		assertEquals("PlusPrefixe", "105", sheet.get("A1"));
	}
	
	public void testMoinsPrefixe () {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=-(7*(2+3)*((((2+1)))))");
		assertEquals("MoinsPrefixe", "-105", sheet.get("A1"));
	}
	
	public void testMoinsPlusPrefixe () {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=+(2*(-(2+3)))");
		assertEquals("MoinsPlusPrefixe", "-10", sheet.get("A1"));
	}
	
//	Add any more error cases you need. Numeric errors (e.g.,
//	divide by 0) can return #Error too.
	
//	Take a deep breath and refactor. This was a big jump.
//	Next time we'll tackle formulas involving cells.
}
