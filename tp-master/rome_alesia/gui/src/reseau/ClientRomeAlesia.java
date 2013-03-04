/*
 * Created on 6 janv. 2005
 *
 */
package reseau;

import gui.Coup;
import gui.CoupFourre;
import gui.Partie;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author nicolas
 *  
 */
public class ClientRomeAlesia implements RomeAlesia {
    private Socket theSock;
    private InputStream entree;
    private boolean estConnecte;
    private boolean partieEnCours;

    /*------------------------------------------------------------------------------
     Constructeur
     ----------------------------------------------------------------------------*/
    /**
     *  
     */
    public ClientRomeAlesia() {
        setEstConnecte (false);
        setPartieEnCours (false);
    }

    /*------------------------------------------------------------------------------
     Accesseurs
     ----------------------------------------------------------------------------*/
    /**
     * @param estConnecte
     *            estConnecte à définir.
     */
    public void setEstConnecte (boolean estConnecte) {
        this.estConnecte = estConnecte;
    }

    /**
     * @return Renvoie estConnecte.
     */
    public boolean getEstConnecte () {
        return estConnecte;
    }

    /**
     * @return
     */
    public boolean getPartieEnCours () {
        return partieEnCours;
    }

    /**
     * @param partieEnCours
     */
    public void setPartieEnCours (boolean partieEnCours) {
        this.partieEnCours = partieEnCours;
    }

    /*------------------------------------------------------------------------------
     Fonctions réseau
     ----------------------------------------------------------------------------*/
    /**
     * @param hote
     * @param port
     */
    public void connection (String hote, int port) {
        try {
            System.out.println ("connection !!");
            theSock = new Socket (InetAddress.getByName (hote), port);
            setEstConnecte (true);
            initFluxEntree ();
        }
        catch (UnknownHostException e) {
            System.out.println (e);
            setEstConnecte (false);
            e.printStackTrace ();
        }
        catch (IOException e) {
            System.out.println (e);
            setEstConnecte (false);
            e.printStackTrace ();
        }
    }

    /**
     *  
     */
    private void initFluxEntree () {
        try {
            entree = theSock.getInputStream ();
        }
        catch (Exception e) {
            System.out.println (e);
            e.printStackTrace ();
        }
    }

    /**
     * @param message
     * @param taille
     * @return
     */
    private boolean envoiMessage (String message, int taille) {
        if (theSock.isClosed () == true) {
            System.out.println ("socket fermée");
            return false;
        }
        System.out.println ("envoi message:  " + message);
        try {
            OutputStream sortie = theSock.getOutputStream ();
            PrintWriter ecrire = new PrintWriter (sortie);
            ecrire.write (message, 0, taille);
            ecrire.flush ();
        }
        catch (Exception e) {
            System.out.println (e);
            e.printStackTrace ();
            return false;
        }
        return true;
    }

    /**
     * @param string
     */
    private boolean envoiMessage (String message) {
        return envoiMessage (message, message.getBytes ().length);
    }

    /**
     * @param valeur
     * @return
     */
    private boolean envoiMessageInt (int valeur) {
        try {
            OutputStream sortie = theSock.getOutputStream ();
            DataOutputStream envoiInt = new DataOutputStream (sortie);
            envoiInt.writeInt (valeur);
        }
        catch (Exception e) {
            System.out.println (e);
            e.printStackTrace ();
            return false;
        }
        return true;
    }

    /**
     * @return
     */
    private char recevoirMessageChar () {
        try {
            InputStream entree = theSock.getInputStream ();
            DataInputStream recvChar = new DataInputStream (entree);
            byte b = recvChar.readByte ();
            return (char) b;
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return '\0';
    }

    /*------------------------------------------------------------------------------
     Fonctions de l'interface RomeAlesia
     ----------------------------------------------------------------------------*/
    /**
     * Enoyer son pseudo au serveur
     * 
     * @param pseudo
     * @return
     */
    public char commandePseudo (String pseudo) {
        envoiMessage (RomeAlesia.CIN_PSEUDO);
        envoiMessageInt (pseudo.length ());
        envoiMessage (pseudo);
        return recevoirMessageChar ();
    }

    /**
     * Créer une nouvelle partie sur le serveur
     * 
     * @param nbArmees
     * @param nbVilles
     * @param camp
     */
    public void commandeNewGame (int nbArmees, int nbVilles, char camp) {
        envoiMessage (RomeAlesia.CIN_NEW_GAME);
        envoiMessageInt (nbArmees);
        envoiMessageInt (nbVilles);
        String s = "" + camp;
        envoiMessage (s);
        setPartieEnCours (true);
    }

    /**
     * Récupérer la liste des parties joignables sur le serveur
     * 
     * @return
     */
    public List<Partie> commandeGetList () {
        envoiMessage (RomeAlesia.CIN_GET_LIST);
        byte[] buffer = new byte[46];
        int nbLu;
        try {
            List<Partie> gameList = new ArrayList<Partie> ();
            while ((nbLu = entree.read (buffer)) != 1) {
                String s = new String (buffer);
                gameList.add (new Partie (s));
            }
            return gameList;
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return null;
    }

    /**
     * Rejoindre une partie sur le serveur
     * 
     * @param idPartie
     */
    public boolean commandeJoinGame (int idPartie) {
        envoiMessage (RomeAlesia.CIN_JOIN_GAME);
        System.out.println ("Rejoindre partie id:" + idPartie);
        envoiMessage (String.valueOf (idPartie));
        byte[] buffer = new byte[1];
        try {
            entree.read (buffer);
            if (buffer[0] == '1') {
                System.out.println ("Partie ok rejointe");
                setPartieEnCours (true);
                return true;
            }
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return false;
    }

    /**
     * Quitter la partie en cours
     */
    public void commandeQuitGame () {
        setPartieEnCours (false);
        envoiMessage (RomeAlesia.CIN_QUIT_GAME);
    }

    /**
     * Quitter le serveur
     */
    public void commandeQuitServer () {
        envoiMessage (RomeAlesia.CIN_QUIT_SERVER);
        try {
            theSock.close ();
            setEstConnecte (false);
            setPartieEnCours (false);
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
    }

    /**
     * Regarder si un joueur c'est connecté sur sa partie crée
     * 
     * @return
     */
    public boolean commandeGetPlayer () {
        envoiMessage (RomeAlesia.CIN_GET_PLAYER);
        byte[] buffer = new byte[1];
        try {
            entree.read (buffer);
            if (buffer[0] == '1') {
                return true;
            }
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return false;
    }

    /**
     * Récupérer la liste de coups fourrés de la partie
     * 
     * @return
     */
    public List<CoupFourre> commandeGetCards () {
        envoiMessage (RomeAlesia.CIN_GET_CARDS);
        try {
            List<CoupFourre> maListe = new ArrayList<CoupFourre> (3);
            for (int i = 0; i < 3; i++) {
                byte[] buffer = new byte[512];
                entree.read (buffer);
                System.out.println ("cf reçu: " + new String (buffer));
                maListe.add (i, new CoupFourre (new String (buffer)));
            }
            System.out.println (maListe);
            return maListe;
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return null;
    }

    /**
     * @return
     */
    public Partie commandeGetMyGame () {
        envoiMessage (RomeAlesia.CIN_GET_MY_GAME);
        if (recevoirMessageChar () == '1') {
            byte[] buffer = new byte[46];
            try {
                Partie game;
                entree.read (buffer);
                String s = new String (buffer);
                System.out.println ("------>" + s);
                game = new Partie (s);
                return game;
            }
            catch (IOException e) {
                e.printStackTrace ();
            }
        }
        return null;
    }

    /**
     * @param carte
     * @param armees
     */
    public void commandePlay (int carte, int armees) {
        envoiMessage (RomeAlesia.CIN_PLAY);
        envoiMessageInt (carte);
        envoiMessageInt (armees);
    }

    /**
     *
     */
    public Coup commandeGetResult () {
        System.out.print ("résultat ?.");
        envoiMessage (RomeAlesia.CIN_GET_RESULT);
        char resultOK = recevoirMessageChar ();
        System.out.println (":" + resultOK);
        if (resultOK == RomeAlesia.RES_NO_RESULT) {
            System.out.println ("Pas encore de résultat");
            return null;
        }
        try {
            byte[] buffer = new byte[40];
            entree.read (buffer);
            String s = new String (buffer);
            System.out.println ("------>" + s);
            System.out.println ("On a un résultat");
            return new Coup (s);
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return null;
    }

    /**
     * @return
     */
    public boolean commandeCanPlay () {
        envoiMessage (RomeAlesia.CIN_CAN_PLAY);
        char c = recevoirMessageChar ();
        if (c == RomeAlesia.RES_CAN_PLAY)
            return true;
        return false;
    }

    /**
     * @return
     */
    public String getOtherIp () {
        envoiMessage (RomeAlesia.CIN_GET_OTHER_IP);
        try {
            int taille = 0;
            byte[] buffer = new byte[6];
            entree.read (buffer);
            String s = new String (buffer);
            String result[] = s.split (":", 3);
            taille = Integer.parseInt (result[1]);
            System.out.println ("taille:" + taille);
            buffer = new byte[taille];
            entree.read (buffer);
            s = new String (buffer);
            return s;
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
        return null;
    }
}
