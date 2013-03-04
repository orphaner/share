/*
 * Created on 6 janv. 2005
 *
 */
package reseau;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

/**
 * @author nicolas
 *  
 */
public class Chat extends Thread {

    private final static int port1 = 7536;
    private final static int port2 = 6357;
    private final static int bufferMax = 1500;

    private DatagramSocket theSocket;
    private InetAddress adresse;
    private int portLocal;
    private int portDistant;

    private JButton envoyerChat = new JButton ("Envoyer");
    private JTextField saisieChat = new JTextField (20);
    private JTextArea messagesChat = new JTextArea ();
    private JPanel chatSaisiePanel = new JPanel ();
    private JPanel chatPanel = new JPanel ();
    private ActionListener envoyerChatListener;

    /**
     * @param host
     * @param serveur
     */
    public Chat() {
        construireChatPanel ();
    }

    /**
     * @param host
     * @param serveur
     */
    public void lancerLeChat (String host, boolean serveur) {
        affecteAdresse (host);
        affecterLesPorts (serveur);
        connection ();
    }

    /**
     *  
     */
    public void stopperLeChat () {
        deconnection ();
    }

    /**
     * @param serveur
     */
    private void affecterLesPorts (boolean serveur) {
        if (serveur == true) {
            portLocal = port1;
            portDistant = port2;
        }
        else {
            portLocal = port2;
            portDistant = port1;
        }
    }

    /**
     * @param host
     */
    private void affecteAdresse (String host) {
        try {
            adresse = InetAddress.getByName (host);
        }
        catch (UnknownHostException e) {
            e.printStackTrace ();
        }
    }

    /**
     *  
     */
    private void connection () {
        try {
            theSocket = new DatagramSocket (portLocal);
            envoyerChat.setEnabled (true);
            saisieChat.setEnabled (true);
            this.start ();
        }
        catch (SocketException e1) {
            e1.printStackTrace ();
        }
    }

    /**
     *  
     */
    public void deconnection () {
        envoyerChat.setEnabled (false);
        saisieChat.setEnabled (false);
        theSocket.close ();
    }

    /*
     * (non-Javadoc)
     * 
     * @see java.lang.Runnable#run()
     */
    public void run () {
        while (!theSocket.isClosed ()) {
            recevoirMessage ();
        }
    }

    /**
     *  
     */
    private void recevoirMessage () {
        byte buffer[] = new byte[bufferMax];
        DatagramPacket message = new DatagramPacket (buffer, bufferMax, adresse, portLocal);
        try {
            theSocket.receive (message);
            addMessage (new String (buffer).trim ());
        }
        catch (IOException e) {
            e.printStackTrace ();
        }
    }

    /**
     *  
     */
    protected void envoyerMessage () {
        String s = saisieChat.getText ();
        if (s.length () > 0) {
            DatagramPacket message = new DatagramPacket (s.getBytes (), s.length (), adresse, portDistant);
            try {
                theSocket.send (message);
                addMessage (s);
                saisieChat.setText ("");
            }
            catch (IOException e) {
                e.printStackTrace ();
            }
        }
    }

    /**
     * @param s
     */
    private synchronized void addMessage (String s) {
        s = s + "\n";
        messagesChat.append (s);
        messagesChat.setCaretPosition (messagesChat.getDocument ().getLength ());
    }

    /**
     *  
     */
    private void envoyerChatListenerCreer () {
        envoyerChatListener = new ActionListener () {
            public void actionPerformed (ActionEvent ae) {
                envoyerMessage ();
            }
        };
        envoyerChat.addActionListener (envoyerChatListener);
        saisieChat.addActionListener (envoyerChatListener);
    }

    /**
     * @return
     */
    public JPanel construireChatPanel () {
        TitledBorder chatBorder = BorderFactory.createTitledBorder ("Chat :");
        chatBorder.setTitleJustification (TitledBorder.LEFT);
        chatPanel.setLayout (new BorderLayout ());
        chatSaisiePanel.setLayout (new BorderLayout ());
        messagesChat.setRows (10);
        messagesChat.setAutoscrolls (true);
        messagesChat.setEditable (false);
        messagesChat.setLineWrap (true);
        chatSaisiePanel.add (saisieChat, BorderLayout.CENTER);
        chatSaisiePanel.add (envoyerChat, BorderLayout.EAST);
        chatPanel.add (new JScrollPane (messagesChat), BorderLayout.CENTER);
        chatPanel.add (chatSaisiePanel, BorderLayout.SOUTH);
        chatPanel.setBorder (chatBorder);
        envoyerChatListenerCreer ();
        envoyerChat.setEnabled (false);
        saisieChat.setEnabled (false);
        return chatPanel;
    }
}
