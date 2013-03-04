/*
 * Créé le 6 janv. 2005
 *
 */

package gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.Timer;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import reseau.Chat;
import reseau.ClientRomeAlesia;
import reseau.RomeAlesia;

/**
 * @author lassalle
 */
public class GuiRomeAlesia extends JFrame {

    private static final long serialVersionUID = 1L;

    private JButton creerPartie = new JButton ("Créer partie");
    private JButton rafraichirListe = new JButton ("Rafraichir");
    private JButton rejoindrePartie = new JButton ("Rejoindre la partie");
    private JButton serveurOK = new JButton ("Connection");

    private JTextField serveurIP = new JTextField ("localhost");
    private JTextField serveurPort = new JTextField ("3000");
    private JTextField serveurPseudo = new JTextField ("Nico");
    private JTextField nbArmeesSaisie = new JTextField (3);
    private JTextField nbVillesSaisie = new JTextField (3);

    private ListePartieTableModel listePartiesModel = new ListePartieTableModel ();
    private TableSorter sorter = new TableSorter (listePartiesModel);
    private JTable listeParties = new JTable (sorter);

    private JLabel serveurIPLabel = new JLabel ("Serveur : ");
    private JLabel serveurPortLabel = new JLabel ("Port : ");
    private JLabel serveurPseudoLabel = new JLabel ("Pseudo : ");
    private JLabel nbArmeesLabel = new JLabel ("Armées :");
    private JLabel nbVillesLabel = new JLabel ("Villes :");
    private JLabel campLabel = new JLabel ("Camp :");

    private JPanel serveurPanel = new JPanel ();
    private JPanel serveurSaisiePanel = new JPanel ();
    private JPanel boutonsServeurPanel = new JPanel ();
    private JPanel listeServeurPanel = new JPanel ();
    private JPanel jeuPanel;
    private Chat theChat;
    private JPanel chatPanel;
    private JPanel jeuChatPanel = new JPanel ();
    private JPanel creerPartiePanel = new JPanel ();
    private JPanel boutonServeurCreerPartiePanel = new JPanel ();

    private final static String[] dataCamp = { "Alesia", "Rome" };
    private JComboBox choixCampList = new JComboBox (dataCamp);

    private JTabbedPane tabbedPane = new JTabbedPane ();

    private ActionListener serveurOKListener;
    private ActionListener creerPartieListener;
    private ActionListener rafraichirListeListener;
    private ActionListener rejoindrePartieListener;

    private Timer attentePartenaireTimer;
    private final static int attentePartenaireTimerDelay = 5000;

    private ClientRomeAlesia clientRA = new ClientRomeAlesia ();
    private Partie partieEnCours;
    private List<CoupFourre> coupFourreListe;

    private char camp;

    /**
     *  
     */
    public GuiRomeAlesia() {
        attentePartenaireTimer = attentePartenaireTimerCreer ();

        sorter.setTableHeader (listeParties.getTableHeader ());
        serveurIP.setColumns (10);

        serveurOKListenerCreer ();
        creerPartieListenerCreer ();
        rafraichirListeListenerCreer ();
        rejoindrePartieListenerCreer ();

        construirePanelServeur ();

        jeuChatPanel.setLayout (new BorderLayout ());

        tabbedPane.addTab ("Serveur", null, serveurPanel, "Connection serveur");
        tabbedPane.addTab ("Jeu", null, jeuChatPanel, "Le jeu en cours");
        
        add (tabbedPane);

        proprietePanelPrincipal ();

        selectionDunePartie ();
        creerPartieDesactiverBoutons ();
    }

    /**
     *  
     */
    private void demarrerPartie () {
        tabbedPane.setSelectedIndex(1);
        jeuPanel = new Jouer (coupFourreListe, partieEnCours, clientRA, camp);
        theChat = new Chat ();
        chatPanel = theChat.construireChatPanel ();
        jeuChatPanel.add (jeuPanel, BorderLayout.CENTER);
        jeuChatPanel.add (chatPanel, BorderLayout.SOUTH);
    }

    /**
     *  
     */
    private void stopperPartie () {
        jeuChatPanel.remove (jeuPanel);
        theChat.stopperLeChat ();
        jeuChatPanel.remove (chatPanel);
    }

    /**
     *  
     */
    private void selectionDunePartie () {
        listeParties.getSelectionModel ().addListSelectionListener (new ListSelectionListener () {
            public void valueChanged (ListSelectionEvent e) {
                if (clientRA.getPartieEnCours () == false) {
                    int rowIndex = listeParties.getSelectedRow ();
                    if (rowIndex == -1) {
                        rejoindrePartie.setEnabled (false);
                    }
                    else {
                        rejoindrePartie.setEnabled (true);
                    }
                }
            }
        });
    }

    /**
     *  
     */
    private void rafraichirListeListenerCreer () {
        rafraichirListeListener = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                rafraichirListeParties ();
            }
        };
        rafraichirListe.addActionListener (rafraichirListeListener);
    }

    /**
     *  
     */
    private void serveurOKListenerCreer () {
        serveurOKListener = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                if (clientRA.getEstConnecte () == false) {
                    connection ();
                }
                else {
                    deconnection ();
                }
            }

            /**
             *  
             */
            private void deconnection () {
                attentePartenaireTimer.stop ();
                clientRA.commandeQuitServer ();

                serveurIP.setEnabled (true);
                serveurPort.setEnabled (true);
                serveurPseudo.setEnabled (true);

                creerPartie.setEnabled (false);
                rafraichirListe.setEnabled (false);

                creerPartie.setText ("Créer partie");
                rejoindrePartie.setText ("Rejoindre la partie");
                creerPartieDesactiverBoutons ();
                creerPartie.setEnabled (false);

                serveurOK.setText ("Connection");
                listePartiesModel.reset ();

                stopperPartie ();
            }

            /**
             *  
             */
            private void connection () {
                if (serveurIP.getText ().equals ("")) {
                    JOptionPane.showMessageDialog (null, "Veuillez saisir le nom ou l'ip du serveur de jeu",
                            "Champ vide", JOptionPane.ERROR_MESSAGE);
                }
                else if (serveurPort.getText ().equals ("")) {
                    JOptionPane.showMessageDialog (null, "Veuillez saisir le port du serveur de jeu",
                            "Champ vide", JOptionPane.ERROR_MESSAGE);
                }
                else if (isNumeric (serveurPort.getText ()) == false) {
                    JOptionPane.showMessageDialog (null, "Le port du serveur de jeu doit être numérique",
                            "Champ non numérique", JOptionPane.ERROR_MESSAGE);
                }
                else if (serveurPseudo.getText ().equals ("")) {
                    JOptionPane.showMessageDialog (null, "Veuillez saisir un pseudo", "Champ vide",
                            JOptionPane.ERROR_MESSAGE);
                }
                else if (serveurPseudo.getText ().length () > 30) {
                    JOptionPane.showMessageDialog (null,
                            "Veuillez saisir un pseudo de moins de 30 caractères", "Pseudo trop long",
                            JOptionPane.ERROR_MESSAGE);
                }
                else {
                    clientRA.connection (serveurIP.getText (), Integer.parseInt (serveurPort.getText ()));
                    if (clientRA.getEstConnecte () == false) {
                        JOptionPane.showMessageDialog (null,
                                "Le nom ou l'ip du serveur de jeu n'est pas valide.\n"
                                        + "Vérifiez également si le serveur est bien démarré",
                                "Erreur de connection au serveur", JOptionPane.ERROR_MESSAGE);
                    }
                    else {
                        String pseudo = serveurPseudo.getText ();
                        if (clientRA.commandePseudo (pseudo) == RomeAlesia.RES_PSEUDO_USED) {
                            JOptionPane.showMessageDialog (null, "Le pseudo est déjà utilisé !",
                                    "Pseudo déjà utilisé", JOptionPane.ERROR_MESSAGE);
                            clientRA.commandeQuitServer ();
                        }
                        else {
                            serveurIP.setEnabled (false);
                            serveurPort.setEnabled (false);
                            serveurPseudo.setEnabled (false);

                            creerPartie.setEnabled (true);
                            creerPartieActiverBoutons ();
                            rafraichirListe.setEnabled (true);
                            serveurOK.setText ("Déconnection");
                            rafraichirListeParties ();
                        }
                    }
                }
            }
        };
        serveurOK.addActionListener (serveurOKListener);
    }

    /**
     *  
     */
    private void creerPartieListenerCreer () {
        creerPartieListener = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                if (clientRA.getPartieEnCours () == false) {
                    creerPartie ();
                }
                else {
                    quitterPartieEnCours ();
                }
            }

            /**
             *  
             */
            private void quitterPartieEnCours () {
                clientRA.commandeQuitGame ();
                creerPartieActiverBoutons ();
                rafraichirListeParties ();
                attentePartenaireTimer.stop ();
                creerPartie.setText ("Créer partie");
                stopperPartie ();
            }

            /**
             *  
             */
            private void creerPartie () {
                if (nbArmeesSaisie.getText ().equals ("")) {
                    JOptionPane.showMessageDialog (null, "Veuillez saisir le nombre d'armées", "Champ vide",
                            JOptionPane.ERROR_MESSAGE);
                }
                else if (isNumeric (nbArmeesSaisie.getText ()) == false) {
                    JOptionPane.showMessageDialog (null, "Le nombre d'armées doit être numérique",
                            "Champ non numérique", JOptionPane.ERROR_MESSAGE);
                }
                else if (nbVillesSaisie.getText ().equals ("")) {
                    JOptionPane.showMessageDialog (null, "Veuillez saisir le nombre de villes", "Champ vide",
                            JOptionPane.ERROR_MESSAGE);
                }
                else if (isNumeric (nbVillesSaisie.getText ()) == false) {
                    JOptionPane.showMessageDialog (null, "Le nombre de villes doit être numérique",
                            "Champ non numérique", JOptionPane.ERROR_MESSAGE);
                }
                else {
                    int nbArmees = Integer.parseInt (nbArmeesSaisie.getText ());
                    int nbVilles = Integer.parseInt (nbVillesSaisie.getText ());
                    camp = choixDuCamp ();
                    clientRA.commandeNewGame (nbArmees, nbVilles, camp);

                    creerPartieDesactiverBoutons ();

                    attentePartenaireTimer.start ();
                    creerPartie.setText ("Quitter sa partie");
                    rafraichirListeParties ();
                }
            }

            /**
             *  
             */
            private char choixDuCamp () {
                if (choixCampList.getSelectedIndex () == 0) {
                    return 'a';
                }
                return 'r';
            }
        };
        creerPartie.addActionListener (creerPartieListener);
    }

    /**
     * @return
     */
    private Timer attentePartenaireTimerCreer () {
        ActionListener taskPerformer = new ActionListener () {
            public void actionPerformed (ActionEvent evt) {
                regarderSiPartenaireConnecte ();
            }

            /**
             *  
             */
            private void regarderSiPartenaireConnecte () {
                if (clientRA.commandeGetPlayer () == true) {
                    System.out.println ("ya un gus quié là");
                    attentePartenaireTimer.stop ();
                    coupFourreListe = clientRA.commandeGetCards ();
                    partieEnCours = clientRA.commandeGetMyGame ();
                    demarrerPartie ();
                    String host = clientRA.getOtherIp ();
                    theChat.lancerLeChat (host, true);
                }
                else {
                    System.out.println ("toujours personne :'(");
                }
            }
        };

        return new Timer (attentePartenaireTimerDelay, taskPerformer);
    }

    /**
     *  
     */
    private void rejoindrePartieListenerCreer () {
        rejoindrePartieListener = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                if (clientRA.getPartieEnCours () == false) {
                    rejoindrePartie ();
                }
                else {
                    quitterPartieRejointe ();
                }
            }

            /**
             *  
             */
            private void quitterPartieRejointe () {
                clientRA.commandeQuitGame ();
                rejoindrePartie.setText ("Rejoindre la partie");
                creerPartieActiverBoutons ();
                creerPartie.setEnabled (true);
                rafraichirListeParties ();
                stopperPartie ();
            }

            /**
             *  
             */
            private void rejoindrePartie () {
                int rowIndex = listeParties.getSelectedRow ();
                Partie partie = listePartiesModel.getGame (rowIndex);
                System.out.println ("Rejoindre la partie :");
                System.out.println (" * row: " + rowIndex + " -> idPartie: " + partie.getId ());
                if (clientRA.commandeJoinGame (partie.getId ()) == false) {
                    JOptionPane
                            .showMessageDialog (
                                    null,
                                    "La partie sélectionnée est injoignable.\n"
                                            + "Soit le créateur a fermé la partie, soit un autre client a été plus rapide que vous =)",
                                    "Impossible de joindre la partie", JOptionPane.ERROR_MESSAGE);
                }
                else {
                    creerPartieDesactiverBoutons ();
                    creerPartie.setEnabled (false);
                    if (partie.getCampLibre ().equals ("Rome")) {
                        camp = 'r';
                    }
                    else {
                        camp = 'a';
                    }
                    rejoindrePartie.setText ("Quitter la partie rejointe");
                    coupFourreListe = clientRA.commandeGetCards ();
                    partieEnCours = partie;
                    demarrerPartie ();
                    String host = clientRA.getOtherIp ();
                    theChat.lancerLeChat (host, false);
                }
            }
        };
        rejoindrePartie.addActionListener (rejoindrePartieListener);
    }

    /**
     *  
     */
    private void proprietePanelPrincipal () {
        Dimension d = Toolkit.getDefaultToolkit ().getScreenSize ();
        setTitle ("Rome Alesia");
        setSize (800, 600);
        setLocation (d.width / 4, d.height / 4);
        setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
        setVisible (true);
    }

    /**
     *  
     */
    private void construirePanelServeur () {
        construireServeurSaisiePanel ();
        construireListeServeurPanel ();
        construireBoutonsServeurPanel ();
        construireCreerPartiePanel ();

        boutonServeurCreerPartiePanel.setLayout (new BoxLayout (boutonServeurCreerPartiePanel,
                BoxLayout.Y_AXIS));
        boutonServeurCreerPartiePanel.add (boutonsServeurPanel);
        boutonServeurCreerPartiePanel.add (creerPartiePanel);

        serveurPanel.setLayout (new BorderLayout ());
        serveurPanel.add (serveurSaisiePanel, BorderLayout.NORTH);
        serveurPanel.add (listeServeurPanel, BorderLayout.CENTER);
        serveurPanel.add (boutonServeurCreerPartiePanel, BorderLayout.SOUTH);
    }

    /**
     *  
     */
    private void construireBoutonsServeurPanel () {
        TitledBorder boutonsServeurBorder = BorderFactory.createTitledBorder ("Actions du serveur :");
        boutonsServeurBorder.setTitleJustification (TitledBorder.LEFT);
        boutonsServeurPanel.setBorder (boutonsServeurBorder);

        boutonsServeurPanel.setLayout (new GridLayout (1, 0));
        rafraichirListe.setEnabled (false);
        rejoindrePartie.setEnabled (false);
        boutonsServeurPanel.add (rafraichirListe);
        boutonsServeurPanel.add (rejoindrePartie);

    }

    /**
     *  
     */
    private void construireCreerPartiePanel () {
        TitledBorder creerPartieBorder = BorderFactory.createTitledBorder ("Créer une partie :");
        creerPartieBorder.setTitleJustification (TitledBorder.LEFT);
        creerPartiePanel.setBorder (creerPartieBorder);
        creerPartie.setEnabled (false);
        creerPartiePanel.add (nbArmeesLabel);
        creerPartiePanel.add (nbArmeesSaisie);
        creerPartiePanel.add (nbVillesLabel);
        creerPartiePanel.add (nbVillesSaisie);
        creerPartiePanel.add (campLabel);
        creerPartiePanel.add (choixCampList);
        creerPartiePanel.add (creerPartie);
    }

    /**
     *  
     */
    private void construireListeServeurPanel () {
        TitledBorder listeServeurBorder = BorderFactory.createTitledBorder ("Liste des parties :");
        listeServeurBorder.setTitleJustification (TitledBorder.LEFT);
        listeServeurPanel.setBorder (listeServeurBorder);

        listeServeurPanel.setLayout (new BorderLayout ());
        listeServeurPanel.add (new JScrollPane (listeParties), BorderLayout.CENTER);
    }

    /**
     *  
     */
    private void construireServeurSaisiePanel () {
        TitledBorder serveurSaisieBorder = BorderFactory.createTitledBorder ("Serveur :");
        serveurSaisieBorder.setTitleJustification (TitledBorder.LEFT);
        serveurSaisiePanel.setBorder (serveurSaisieBorder);

        serveurSaisiePanel.setLayout (new BoxLayout (serveurSaisiePanel, BoxLayout.X_AXIS));
        serveurSaisiePanel.add (serveurIPLabel);
        serveurSaisiePanel.add (serveurIP);
        serveurSaisiePanel.add (serveurPortLabel);
        serveurSaisiePanel.add (serveurPort);
        serveurSaisiePanel.add (serveurPseudoLabel);
        serveurPseudo.setColumns (10);
        serveurSaisiePanel.add (serveurPseudo);
        serveurSaisiePanel.add (serveurOK);
    }

    /**
     *  
     */

    /**
     *  
     */
    private void rafraichirListeParties () {
        List<Partie> l = clientRA.commandeGetList ();
        if (l != null) {
            listePartiesModel.setGameList (l);
        }
        rejoindrePartie.setEnabled (false);
    }

    /**
     * @param args
     */
    public static void main (String[] args) {
        new GuiRomeAlesia ();
    }

    /**
     *  
     */
    private void creerPartieActiverBoutons () {
        nbVillesSaisie.setEnabled (true);
        nbArmeesSaisie.setEnabled (true);
        choixCampList.setEnabled (true);
    }

    /**
     *  
     */
    private void creerPartieDesactiverBoutons () {
        nbVillesSaisie.setEnabled (false);
        nbArmeesSaisie.setEnabled (false);
        choixCampList.setEnabled (false);
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
