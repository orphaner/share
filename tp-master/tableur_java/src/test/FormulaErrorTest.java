
package test;

import tableur.Sheet;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class FormulaErrorTest extends TestCase {

	public FormulaErrorTest(String name) {
		super(name);
	}

	public void test2J_testSimpleFormulaError() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7*");

		assertEquals("Missing operand", "#Error", sheet.get("A1"));
	}

	public void test2K_testParenthesisError() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=(((((7))");
		assertEquals("Error", "#Error", sheet.get("A1"));
	}

	public void testDivideByZero() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=5/0");

		assertEquals("Divide by 0", "#Error", sheet.get("A1"));
	}

	public void testOverflow() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=" + Integer.MAX_VALUE + "+1");

		assertEquals("Overflow", "-2147483648", sheet.get("A1"));
	}

	public void testUnderflow() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=" + Integer.MIN_VALUE + "-1");

		assertEquals("Underflow", "#Error", sheet.get("A1"));
	}

	public void testMissingOperand1() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=+");

		assertEquals("Missing operand", "#Error", sheet.get("A1"));
	}

	public void testMissingOperand2() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=5+");

		assertEquals("Missing operand", "#Error", sheet.get("A1"));
	}

	public void testMissingOperand2b() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=5*");

		assertEquals("Missing operand", "#Error", sheet.get("A1"));
	}
	
	public void testMissingOperand3() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=+5+");

		assertEquals("Missing operand", "#Error", sheet.get("A1"));
	}

	public void testMissingOperand4() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7+)");

		assertEquals("Missing operator", "#Error", sheet.get("A1"));
	}

	public void testNoOperand() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=++");

		assertEquals("No operand", "#Error", sheet.get("A1"));
	}

	public void testParenMismatch1() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=(");

		assertEquals("Paren mismatch", "#Error", sheet.get("A1"));
	}

	public void testParenMismatch2() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7+(");

		assertEquals("Paren mismatch", "#Error", sheet.get("A1"));
	}

	public void testParenMismatch3() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=)");

		assertEquals("Paren mismatch", "#Error", sheet.get("A1"));
	}

	public void testExtraOperators() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7+++7");

		assertEquals("Extra operator", "#Error", sheet.get("A1"));
	}

	public void testInvalidCharacter1() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7G");

		assertEquals("Invalid character", "#Error", sheet.get("A1"));
	}

	public void testInvalidCharacter2() {
		Sheet sheet = new Sheet();
		sheet.put("A1", "=7!");

		assertEquals("Invalid character", "#Error", sheet.get("A1"));
	}

	public static Test suite() {
		return new TestSuite(FormulaErrorTest.class);
	}

	public static void main(String args[]) {
		junit.textui.TestRunner.run(suite());
	}
}
