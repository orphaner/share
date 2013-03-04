/*
 * Créé le 6 oct. 2004
 *
 * TODO Pour changer le modèle de ce fichier généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
package test;


import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * @author leberre
 *
 * TODO Pour changer le modèle de ce commentaire de type généré, allez à :
 * Fenêtre - Préférences - Java - Style de code - Modèles de code
 */
public class AllTests {
    
    public static Test suite() {
        TestSuite suite = new TestSuite("Test for tableur");
        //$JUnit-BEGIN$
        suite.addTestSuite(Challenge1.class);
        suite.addTestSuite(Challenge2.class);
        suite.addTestSuite(Challenge3.class);
        suite.addTestSuite(Challenge4.class);
        suite.addTestSuite(Challenge5.class);
        suite.addTestSuite(AdditionalParserTesting.class);
        suite.addTest(FormulaErrorTest.suite());
        //$JUnit-END$
        return suite;
    }
}
