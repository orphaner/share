/*
 * Created on 11 janv. 2005
 *
 */
package reseau;

import gui.Coup;
import gui.CoupFourre;
import gui.Partie;

import java.util.List;

/**
 * @author nicolas
 *
 */
public interface RomeAlesia {
    public final static String CIN_PSEUDO = "1";
    public final static String CIN_NEW_GAME = "2";
    public final static String CIN_GET_LIST = "3";
    public final static String CIN_JOIN_GAME = "4";
    public final static String CIN_QUIT_GAME = "5";
    public final static String CIN_QUIT_SERVER = "6";
    public final static String CIN_GET_PLAYER = "7";
    public final static String CIN_GET_CARDS = "8";
    public static final String CIN_GET_MY_GAME = "9";
    public static final String CIN_PLAY = "a";
    public static final String CIN_GET_RESULT = "b";
    public static final String CIN_CAN_PLAY = "c";
    public static final String CIN_GET_OTHER_IP = "d";

    public final static char RES_GAME_NOT_FOUND = '0';
    public final static char RES_GAME_JOIN_OK = '1';
    public final static char RES_PSEUDO_USED = '2';
    public final static char RES_PSEUDO_OK = '3';
    public final static char RES_NOT_YET_RESULT = '4';
    public final static char RES_ROME_WIN = '5';
    public final static char RES_ALESIA_WIN = '6';
    public final static char RES_RESULT = '7';
    public final static char RES_NO_RESULT = '8';
    public final static char RES_CAN_PLAY = '9';
    public final static char RES_CANT_PLAY = 'a';

    /**
     * @param pseudo
     * @return
     */
    public abstract char commandePseudo (String pseudo);

    /**
     * @param nbArmees
     * @param nbVilles
     * @param camp
     */
    public abstract void commandeNewGame (int nbArmees, int nbVilles, char camp);

    /**
     * @return
     */
    public abstract List<Partie> commandeGetList ();

    /**
     * @param idPartie
     */
    public abstract boolean commandeJoinGame (int idPartie);

    /**
     * 
     */
    public abstract void commandeQuitGame ();

    /**
     * 
     */
    public abstract void commandeQuitServer ();

    /**
     * @return
     */
    public abstract boolean commandeGetPlayer ();

    /**
     * @return
     */
    public abstract List<CoupFourre> commandeGetCards ();

    /**
     * @return
     */
    public abstract Partie commandeGetMyGame ();

    /**
     * @param carte
     * @param armees
     */
    public abstract void commandePlay (int carte, int armees);

    /**
     * @return
     */
    public abstract Coup commandeGetResult ();

    /**
     * @return
     */
    public abstract boolean commandeCanPlay ();

    /**
     * @return
     */
    public abstract String getOtherIp ();
}
