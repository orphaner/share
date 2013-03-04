/*
 * Created on 10 janv. 2005
 *
 */
package gui;

/**
 * @author nicolas
 *
 */
public class Partie {
    private int id;
    private String nbArmees;
    private String nbVilles;
    private String createur;
    private String campLibre;

    /**
     * @param stringResultClient
     */
    public Partie(String stringResultClient) {
        String result[] = stringResultClient.split (":", 5);
        setInfos (Integer.parseInt (result[0]), result[1], result[2], result[3], result[4]);
    }

    /**
     * @param id
     * @param nbArmees
     * @param nbVilles
     * @param createur
     * @param campLibre
     */
    private void setInfos (int id, String nbArmees, String nbVilles, String campLibre, String createur) {
        this.id = id;
        this.nbArmees = nbArmees;
        this.nbVilles = nbVilles;
        this.createur = createur.trim ();
        if (campLibre.equals ("r")) {
            this.campLibre = new String ("Rome");
        }
        else if (campLibre.equals ("a")) {
            this.campLibre = new String ("Alesia");
        }
    }

    /**
     * @return Renvoie campLibre.
     */
    public String getCampLibre () {
        return campLibre;
    }

    /**
     * @return Renvoie createur.
     */
    public String getCreateur () {
        return createur;
    }

    /**
     * @return Renvoie id.
     */
    public int getId () {
        return id;
    }

    /**
     * @return Renvoie nbArmees.
     */
    public String getNbArmees () {
        return nbArmees;
    }

    /**
     * @return Renvoie nbVilles.
     */
    public String getNbVilles () {
        return nbVilles;
    }

    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    public String toString () {
        String s;
        s = "" + id + ":" + nbArmees + ":" + nbVilles + ":" + campLibre + ":" + createur;
        return s;
    }
}
