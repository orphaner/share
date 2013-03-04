/*
 * Created on 13 janv. 2005
 * 
 */
package gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSlider;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.Timer;
import javax.swing.border.TitledBorder;

import reseau.ClientRomeAlesia;

/**
 * @author nicolas
 *  
 */
public class Jouer extends JPanel {

    // Plateau de jeu
    private List<JButton> coupFourreButton = new ArrayList<JButton> (3);
    private JButton aucunCoupFourre = new JButton ("Aucun");
    private JLabel nbArmeeRome = new JLabel ();
    private JLabel nbArmeeAlesia = new JLabel ();
    private JTextField armeesAttaque = new JTextField (5);
    private JTextArea historiqueCoups = new JTextArea ();
    private JButton jouerOK = new JButton ("OK");
    private JSlider bataille = new JSlider ();

    private JPanel historiqueCoupsPanel = new JPanel ();
    private JPanel champBataillePanel = new JPanel ();
    private JPanel jouerPanel = new JPanel ();

    private ClientRomeAlesia clientRA = new ClientRomeAlesia ();
    private Partie partieEnCours;
    private List<CoupFourre> coupFourreListe;
    protected int coupFourreSelectionne;
    private JLabel coupFourreLabel = new JLabel ("Aucun");

    private Timer recevoirScoreTimer;
    private int recevoirScoreDelay = 1000;
    private Timer canPlayTimer;
    private int canPlayDelay = 1000;

    private char camp;
    
    /**
     * @param coupFourreListe
     * @param partieEnCours
     * @param clientRA
     * @param camp
     */
    public Jouer(List<CoupFourre> coupFourreListe, Partie partieEnCours, ClientRomeAlesia clientRA, char camp) {
        this.coupFourreListe = coupFourreListe;
        this.partieEnCours = partieEnCours;
        this.clientRA = clientRA;
        this.camp = camp;
        construirePlateauDeJeu ();
    }

    /**
     * @param i
     * @return
     */
    private ActionListener coupFourreListenerCreer (final int i) {
        ActionListener action = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                coupFourreSelectionne = i;
                coupFourreLabel.setText (coupFourreListe.get (i).getNom ());
            }
        };
        return action;
    }

    /**
     *  
     */
    private void coupFourreListenerCreer () {
        ActionListener action = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                resetCoupFourreSelectionne ();
            }
        };
        aucunCoupFourre.addActionListener (action);
    }

    /**
     *  
     */
    private void jouerOKListenerCreer () {
        ActionListener action = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {

                if (isNumeric (armeesAttaque.getText ()) == false) {
                    JOptionPane.showMessageDialog (null, "Le nombre d'armées doit être numérique",
                            "Champ non numérique", JOptionPane.ERROR_MESSAGE);
                }
                else {
                    int armees = Integer.parseInt (armeesAttaque.getText ());
                    int libre;
                    if (camp == 'r') {
                        libre = Integer.parseInt (nbArmeeRome.getText());
                    }
                    else {
                        libre = Integer.parseInt (nbArmeeAlesia.getText());
                    }
                    if (Integer.parseInt (armeesAttaque.getText ()) < 1) {
                        JOptionPane.showMessageDialog (null, "Vous devez envoyer au moins une armées",
                                "Pas assez d'armées", JOptionPane.ERROR_MESSAGE);
                    }
                    else if (armees > libre) {
                          JOptionPane.showMessageDialog (null, "Vous essayez d'envoyer trop d'armées",
                                  "Pas assez d'armées", JOptionPane.ERROR_MESSAGE);
                    }
                    else {
                        int carte = 0;
                        if (coupFourreSelectionne > -1) {
                            carte = coupFourreListe.get (coupFourreSelectionne).getId ();
                            coupFourreButton.get (coupFourreSelectionne).setEnabled (false);
                        }
                        clientRA.commandePlay (carte, armees);
                        System.out.println("J'ai joué !!");
                        jouerOK.setEnabled (false);
                        resetCoupFourreSelectionne ();
                        recevoirScoreTimer.start ();
                        System.out.println("fin de J'ai joué !!");
                    }
                }
            }
        };
        jouerOK.addActionListener (action);
    }

    /**
     * @return
     */
    private void recevoirScoreTimerCreer () {
        ActionListener taskPerformer = new ActionListener () {

            public void actionPerformed (ActionEvent evt) {
                System.out.println ("Timer des scores!!");
                Coup resultat;
                resultat = clientRA.commandeGetResult ();
                if (resultat != null) {
                    historiqueCoups.append (resultat.toString ());
                    nbArmeeRome.setText (""+resultat.getArmeesRomeTotal());
                    nbArmeeAlesia.setText (""+resultat.getArmeesAlesiaTotal());
                    bataille.setValue (resultat.getPositionConflit ());
                    recevoirScoreTimer.stop ();
                    System.out.println("go on :"+resultat.getResultat());
                    if (resultat.getResultat() == 3) {
                        canPlayTimer.start ();
                    }
                }
            }
        };
        recevoirScoreTimer = new Timer (recevoirScoreDelay, taskPerformer);
    }

    /**
     * @return
     */
    private void canPlayTimerCreer () {
        ActionListener taskPerformer = new ActionListener () {
            public void actionPerformed (ActionEvent evt) {
                System.out.println ("Jpeu jouer ???");
                boolean resultat = clientRA.commandeCanPlay ();
                if (resultat == true) {
                    canPlayTimer.stop ();
                    armeesAttaque.setText ("");
                    jouerOK.setEnabled (true);
                }
            }
        };
        canPlayTimer = new Timer (canPlayDelay, taskPerformer);
    }

    /**
     *  
     */
    private void construirePlateauDeJeu () {
        construireChampBataillePanel ();
        construireHistoriquePanel ();
        construireJouerPanel ();

        this.setLayout (new BorderLayout ());
        this.add (champBataillePanel, BorderLayout.NORTH);
        this.add (historiqueCoupsPanel, BorderLayout.EAST);
        this.add (jouerPanel, BorderLayout.CENTER);
    }

    /**
     *  
     */
    private void construireJouerPanel () {
        initLesComposants ();
        TitledBorder jouerBorder = BorderFactory.createTitledBorder ("Envoyer les armées :");
        jouerBorder.setTitleJustification (TitledBorder.LEFT);
        jouerPanel.setBorder (jouerBorder);
        JPanel lancerArmees = new JPanel ();
        lancerArmees.setLayout (new GridLayout (0, 3));
        lancerArmees.add (new JLabel ("Armées à envoyer : "));
        lancerArmees.add (armeesAttaque);
        lancerArmees.add (jouerOK);
        lancerArmees.add (new JLabel ("Coup fourré :"));
        lancerArmees.add (coupFourreLabel);
        JPanel coupFourrePanel = new JPanel ();
        for (int i = 0; i < 3; i++) {
            coupFourrePanel.add (coupFourreButton.get (i));
        }
        coupFourrePanel.add (aucunCoupFourre);
        jouerPanel.setLayout (new BorderLayout ());
        jouerPanel.add (lancerArmees, BorderLayout.NORTH);
        jouerPanel.add (coupFourrePanel, BorderLayout.CENTER);
    }

    /**
     *  
     */
    private void construireHistoriquePanel () {
        TitledBorder historiqueCoupsBorder = BorderFactory.createTitledBorder ("Historique :");
        historiqueCoupsPanel.setBorder (historiqueCoupsBorder);
        historiqueCoups.setColumns (25);
        historiqueCoups.setEditable(false);
        historiqueCoupsPanel.setLayout (new BorderLayout ());
        JScrollPane scroll = new JScrollPane (historiqueCoups);
        scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        historiqueCoupsPanel.add (scroll, BorderLayout.CENTER);
    }

    /**
     *  
     */
    private void construireChampBataillePanel () {
        TitledBorder champBatailleBorder = BorderFactory.createTitledBorder ("Champ de bataille :");
        champBatailleBorder.setTitleJustification (TitledBorder.CENTER);
        champBataillePanel.setBorder (champBatailleBorder);
        bataille.setMajorTickSpacing (1);
        bataille.setPaintTicks (true);
        JPanel toto = new JPanel ();
        toto.setLayout (new BorderLayout ());
        toto.add (bataille, BorderLayout.SOUTH);
        toto.add (nbArmeeRome, BorderLayout.WEST);
        toto.add (nbArmeeAlesia, BorderLayout.EAST);
        champBataillePanel.setLayout (new GridLayout (0, 3));
        champBataillePanel.setPreferredSize (new Dimension (0, 190));
        champBataillePanel.setMaximumSize (champBataillePanel.getPreferredSize ());
        champBataillePanel.add (new AfficheImage ("rome.jpg"));
        champBataillePanel.add (toto);
        champBataillePanel.add (new AfficheImage ("alesia.jpg"));
    }

    /**
     * @param s
     */
    public void initLesComposants () {
        nbArmeeRome.setText (partieEnCours.getNbArmees ());
        nbArmeeAlesia.setText (partieEnCours.getNbArmees ());
        bataille.setMinimum (1);
        int nbVilles = Integer.parseInt (partieEnCours.getNbVilles ());
        bataille.setMaximum (nbVilles);
        bataille.setValue ((nbVilles / 2) + 1);
        bataille.setEnabled (false);
        for (int i = 0; i < 3; i++) {
            JButton cp = new JButton (coupFourreListe.get (i).toString ());
            cp.setToolTipText (coupFourreListe.get (i).getDescription ());
            cp.addActionListener (coupFourreListenerCreer (i));
            coupFourreButton.add (cp);
        }
        coupFourreSelectionne = -1;
        coupFourreListenerCreer ();
        jouerOKListenerCreer ();
        recevoirScoreTimerCreer ();
        canPlayTimerCreer ();
        if (camp == 'r') {
            nbArmeeRome.setFont (new Font ("",Font.BOLD,36));
        }
        else {
            nbArmeeAlesia.setFont (new Font ("",Font.BOLD,36));
        }
    }

    /**
     *  
     */
    private void resetCoupFourreSelectionne () {
        coupFourreSelectionne = -1;
        coupFourreLabel.setText ("Aucun");
    }

    /**
     * @author nicolas Affiche une image dans un panel
     */
    private class AfficheImage extends JPanel {
        private static final long serialVersionUID = 1L;

        private Image image;

        private int x, y;

        /**
         * @param s
         */
        public AfficheImage(String s) {
            image = getToolkit ().getImage (s);
            setOpaque (false);
        }

        /*
         * (non-Javadoc)
         * 
         * @see javax.swing.JComponent#paintComponent(java.awt.Graphics)
         */
        public void paintComponent (Graphics g) {
            x = (this.getWidth () - image.getWidth (this)) / 2;
            y = (this.getHeight () - image.getHeight (this)) / 2;

            super.paintComponent (g);
            g.drawImage (image, x, y, this);
        }
    }

    /**
     * @param theValue
     * @return
     */
    protected static boolean isNumeric (String theValue) {
        try {
            Integer.parseInt (theValue.trim ());
            return true;
        }
        catch (NumberFormatException e) {
            return false;
        }
    }
}
