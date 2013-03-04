/*
 * Created on 10 janv. 2005
 *
 */
package gui;

/**
 * @author nicolas
 *
 */
public class CoupFourre {
    private int id;
    private String nom;
    private String description;

    /**
     * @param id
     * @param description
     */
    public CoupFourre(int id, String nom, String description) {
        super ();
        setInfos (id, nom, description);
    }

    /**
     * @param resultFromClient
     */
    public CoupFourre(String resultFromClient) {
        String result[] = resultFromClient.split (":", 3);
        setInfos (Integer.parseInt (result[0]), result[1], result[2]);
    }

    /**
     * @param id
     * @param description
     */
    private void setInfos (int id, String nom, String description) {
        this.id = id;
        this.nom = nom.trim ();
        this.description = description.trim ();
    }

    /**
     * @return
     */
    public String getNom () {
        return nom;
    }

    /**
     * @return
     */
    public String getDescription () {
        return description;
    }

    /**
     * @return
     */
    public int getId () {
        return id;
    }

    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    public String toString () {
        return getNom ();
    }
}
