/*
 * Created on 9 nov. 2004
 *
 */
package tableur;

//import java.awt.GraphicsConfiguration;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableColumn;

/**
 * @author nicolas
 */
public class SheetFrame extends JFrame {
    public final SheetTableModel model;
    public final JButton okButton = new JButton("ok");
    public final JButton koButton = new JButton("annuler");
    public final JLabel label = new JLabel("");
    public final JTable table;
    public final JTextField editor = new JTextField();

    private final JMenuBar menuBar = new JMenuBar();
    private final JMenu menuFichier = new JMenu("Fichier");
    private final JMenu menuAPropos = new JMenu ("A Propos");
    
    private boolean changement;
    private final JFileChooser fc = new JFileChooser();
    
    private String fileName = "";
    
    /**
     * 
     */
    public SheetFrame (SheetTableModel m) {
        model = m;
        changement = false;
        
        table = new JTable (model);
        table.setRowSelectionAllowed(true);
        table.setColumnSelectionAllowed(true);
        table.setRowHeight(20);
        table.setAutoResizeMode(0);
        modelInit ();
  
        table.getSelectionModel().addListSelectionListener(new ListSelectionListener () {
            public void valueChanged(ListSelectionEvent e) {
                int rowIndex = table.getSelectedRow();
                int colIndex = table.getSelectedColumn();
                label.setText(SheetTableModel.getCellName(rowIndex, colIndex));
                editor.setText(model.getLiteralValueAt(rowIndex, colIndex));
                editor.requestFocusInWindow();
            }
        });
        
        okButton.addActionListener (new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                int rowIndex = table.getSelectedRow();
                int colIndex = table.getSelectedColumn();
                model.setValueAt (editor.getText(), rowIndex, colIndex);
                changement = true;
            }
        });
        
        JPanel panelHaut = new JPanel();
        panelHaut.setLayout(new GridLayout(1, 3));
        panelHaut.add(label, "West");
        panelHaut.add(editor, "Center");
        panelHaut.add(okButton, "East");
        add(panelHaut, "North");
        add(new JScrollPane(table));
        creerMenus ();

        Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
        setTitle ("Super tableur de la mort qui tue");
        setSize (800, 600);
        setLocation (d.width/4, d.height/4);
        setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
        setVisible (true);
    } 
    
    private void creerMenus () {
        JMenuItem menuItem;
        menuItem = new JMenuItem("Nouveau", KeyEvent.VK_N);
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.CTRL_MASK));
        menuItem.setToolTipText("Nouvelle feuille de calcul");
        menuItem.addActionListener (new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                nouvelleFeuille ();
            }
        });
        menuFichier.add(menuItem);
        
        menuFichier.addSeparator();
        
        menuItem = new JMenuItem("Ouvrir", KeyEvent.VK_O);
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
        menuItem.setToolTipText("Ouvrir une feuille de calcul");
        menuItem.addActionListener (new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                ouvrir ();
            }
        });
        menuFichier.add(menuItem);
        
        menuFichier.addSeparator();
        
        menuItem = new JMenuItem("Sauvegarder", KeyEvent.VK_S);
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
        menuItem.setToolTipText("Enregistre la feuille dans le fichier courant");
        menuItem.addActionListener (new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                enregistrer ();
            }
        });
        menuFichier.add(menuItem);
        
        menuItem = new JMenuItem("Sauvegarder sous", KeyEvent.VK_V);
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, ActionEvent.CTRL_MASK));
        menuItem.setToolTipText("Enregistre la feuille dans un nouveau fichier");
        menuItem.addActionListener (new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                enregistrerSous ();
            }
        });
        menuFichier.add(menuItem);
        
        menuFichier.addSeparator();
        
        menuItem = new JMenuItem("Quitter", KeyEvent.VK_Q);
        menuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, ActionEvent.CTRL_MASK));
        menuItem.setToolTipText("Quitte le programme");
        menuItem.addActionListener (new ActionListener () {
            public void actionPerformed (ActionEvent e) {
                quitter ();
            }
        });
        menuFichier.add(menuItem);
        
        menuBar.add(menuFichier);
        menuBar.add(menuAPropos);
        
        this.setJMenuBar(menuBar);
    }
    
    private void modelInit () {
        TableColumn column = table.getColumnModel().getColumn(0);
        column.setPreferredWidth(40);
        column.setCellRenderer(table.getTableHeader().getDefaultRenderer());
        for (int i = 1 ; i < table.getModel().getColumnCount() ; i++) {
            column = table.getColumnModel().getColumn(i);
            column.setPreferredWidth(60);
            column.setResizable(true);
        }
    }
    
    private boolean needToSav () {
        if (changement == true) {
            int sauv = JOptionPane.showConfirmDialog (null, 
                    "La feuille courante a été modifiée.\n Voulez vous sauvegarder ?", 
                    "Sauvegarder la feuille", JOptionPane.YES_NO_CANCEL_OPTION);
            switch (sauv) {
            // Oui
            case 0:
                enregistrer ();
                break;
            // Annuler
            case 2:
                return false;
            }
        }
        return true;
    }
    
    private void nouvelleFeuille (Sheet newSheet) {
        if (needToSav ()) {
            model.changeSheet (newSheet);
            modelInit ();
            changement = false;
        }
    }
    
    private void nouvelleFeuille () {
        nouvelleFeuille (new Sheet());
        fileName = "";
    }
    
    private void enregistrerAction () {
        changement = false;
        CSVFile csv = new CSVFile();
        csv.writeFile (model, fileName);
    }
    
    private void enregistrerSous () {
        int returnVal = fc.showSaveDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            fileName = file.getPath();
            enregistrerAction ();
        }
    }
    
    private void enregistrer () {
        if (fileName.equals("")) {
            enregistrerSous ();
        }
        else {
            enregistrerAction ();
        }
    }
    
    private void ouvrir () {
        int returnVal = fc.showOpenDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            CSVFile csv = new CSVFile();
            Sheet openSheet = csv.readFile(file.getPath());
            if (openSheet == null) {
                JOptionPane.showMessageDialog(null,"Impossible d'ouvrir ou de lire le fichier "+
                        file.getPath(), "Erreur à l'ouverture", JOptionPane.ERROR_MESSAGE);
            }
            else {
                changement = false;
                nouvelleFeuille (openSheet);
                fileName = file.getPath();
            }
        }
    }
    
    private void quitter () {
        if (needToSav ()) {
            System.exit(0);
        }
    }
    
    public static void main (String[] args) {
        Sheet s = new Sheet();
        new SheetFrame (new SheetTableModel(s));
    }
}
