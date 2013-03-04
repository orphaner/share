/*
 * Created on 10 janv. 2005
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package gui;

/**
 * @author nicolas
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class IntToByte {

    public static String go (int i) {
        byte result[] = new byte[4];
        
        for (i=0; i<4; i++) {
            result[i] = (byte) (i>>(i*8));
        }
        return new String(result);
    }
}
