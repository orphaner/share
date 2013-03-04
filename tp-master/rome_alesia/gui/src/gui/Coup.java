/*
 * Created on 13 janv. 2005
 *
 */
package gui;

/**
 * @author nicolas
 *  
 */
public class Coup {
    private int resultat;
    private int positionConflit;
    private int carteRome;
    private int carteAlesia;
    private int armeesRomeTotal;
    private int armeesAlesiaTotal;
    private int armeesRomeJouees;
    private int armeesAlesiaJouees;

    /**
     * @param s
     */
    public Coup(String s) {
        String result[] = s.split (":", 9);
        int resInt[] = new int[8];
        for (int i = 0; i < 8; i++) {
            resInt[i] = Integer.parseInt (result[i]);
        }
        coupInit (resInt[0], resInt[1], resInt[2], resInt[3], resInt[4], resInt[5], resInt[6], resInt[7]);
    }

    /**
     * @param resultat
     * @param positionConflit
     * @param carteRome
     * @param carteAlesia
     * @param armeesRomeTotal
     * @param armeesAlesiaTotal
     * @param armeesRomeJouees
     * @param armeesAlesiaJouees
     */
    public void coupInit (int resultat, int positionConflit, int carteRome, int carteAlesia,
            int armeesRomeTotal, int armeesAlesiaTotal, int armeesRomeJouees, int armeesAlesiaJouees) {
        this.resultat = resultat;
        this.positionConflit = positionConflit;
        this.carteRome = carteRome;
        this.carteAlesia = carteAlesia;
        this.armeesRomeTotal = armeesRomeTotal;
        this.armeesAlesiaTotal = armeesAlesiaTotal;
        this.armeesRomeJouees = armeesRomeJouees;
        this.armeesAlesiaJouees = armeesAlesiaJouees;
    }

    /**
     * @return Renvoie armeesAlesiaJouees.
     */
    public int getArmeesAlesiaJouees () {
        return armeesAlesiaJouees;
    }

    /**
     * @return Renvoie armeesAlesiaTotal.
     */
    public int getArmeesAlesiaTotal () {
        return armeesAlesiaTotal;
    }

    /**
     * @return Renvoie armeesRomeJouees.
     */
    public int getArmeesRomeJouees () {
        return armeesRomeJouees;
    }

    /**
     * @return Renvoie armeesRomeTotal.
     */
    public int getArmeesRomeTotal () {
        return armeesRomeTotal;
    }

    /**
     * @return Renvoie carteAlesia.
     */
    public int getCarteAlesia () {
        return carteAlesia;
    }

    /**
     * @return Renvoie carteRome.
     */
    public int getCarteRome () {
        return carteRome;
    }

    /**
     * @return Renvoie positionConflit.
     */
    public int getPositionConflit () {
        return positionConflit;
    }

    /**
     * @return Renvoie resultat.
     */
    public int getResultat () {
        return resultat;
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.lang.Object#toString()
     */
    public String toString () {
        String s;
        s = "R:" + getArmeesRomeJouees ();
        s += "   A:" + getArmeesAlesiaJouees ();
        s += "\n";
        
        if (getResultat() == 0) {
            s += "PARTIE NULLE\n";
        }
        else if (getResultat() == 1) {
            s += "ROME GAGNE\n";
        }
        else if (getResultat() == 2) {
            s += "ALESIA GAGNE\n";
        }
        
        return s;
    }
}
