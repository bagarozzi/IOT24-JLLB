package it.unibo.impl;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import it.unibo.api.Connection;

public class OperatorDashboardGui extends JFrame{
    private JLabel statusLabel = new JLabel("Status: Waiting");
    private  JLabel tempLabel = new JLabel("Temperature: ");
    private JLabel fillLabel = new JLabel("Fill level: ");
    private Connection connection;
    private String MSGtoUpdate = "[0-9]+:([+-]?(?=\\.\\d|\\d)(?:\\d+)?(?:\\.?\\d*))(?:[Ee]([+-]?\\d+))?:([+-]?(?=\\.\\d|\\d)(?:\\d+)?(?:\\.?\\d*))(?:[Ee]([+-]?\\d+))?";
    private Path TemeratureHistory = Paths.get("output/temperature.txt");
    private Path OutputHistory = Paths.get("output/output.txt");

    public OperatorDashboardGui() {

        try {
            Files.write(TemeratureHistory, "Temerature History".getBytes(), StandardOpenOption.CREATE);
            Files.write(OutputHistory, "Output History".getBytes(), StandardOpenOption.CREATE);
        } catch (IOException e) {
            System.out.println("can't write the file");
        }

        //connection = new DashboardConnection(this, "COM3");
        this.setTitle("Smart Waste Dashboard");
        GridBagConstraints c = new GridBagConstraints();
        
        connection = new DashboardConnection(this, "COM3");

        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(500, 500);

        JPanel panel = new JPanel();
        this.add(panel);
        panel.setLayout(new GridBagLayout());

        statusLabel = new JLabel("Status: Waiting");
        tempLabel = new JLabel("Temperature: ");
        fillLabel = new JLabel("Fill level: ");

        JButton emptyButton = new JButton("Empty");
        emptyButton.addActionListener(e->connection.sendCommand("emptyBin"));
        JButton restoreButton = new JButton("Restore");
        restoreButton.addActionListener(e->connection.sendCommand("resetTemperature"));


        // Azioni dei pulsanti per inviare comandi ad Arduino
        //emptyButton.addActionListener(e->connection.sendCommand('0'));
        //restoreButton.addActionListener(e->connection.sendCommand('0'));

        c.gridx = 3;
        c.gridy = 0;
        c.insets = new Insets(100, 0, 0, 0);
        panel.add(statusLabel, c);
        c.insets = new Insets(20, 0, 0, 0);
        c.gridy = 1;
        panel.add(tempLabel, c);
        c.gridy = 2;
        c.insets = new Insets(20, 0, 100, 0);
        panel.add(fillLabel, c);
        c.insets = new Insets(0, 0, 0, 0);
        c.fill = GridBagConstraints.BOTH;
        c.gridwidth = 3;
        c.gridheight = 1;
        c.gridy = 3;
        c.gridx = 1;
        c.weightx = 1.0;
        c.weighty = 1.0;
        panel.add(emptyButton, c);
        c.gridy = 5;
        panel.add(restoreButton, c);

        this.setVisible(true);
    }

    public void updateGUI(String data) {
        String[] parts = data.split(":");
        if(data.matches(MSGtoUpdate)){
            if (parts.length == 2) {
                this.tempLabel.setText("Temperature: " + parts[2]);
                this.fillLabel.setText("Fill level: " + parts[1]);
            }
        } else if (parts[0].equals("[Temperature]")) {
            try {
                Files.write(TemeratureHistory, data.getBytes(), StandardOpenOption.APPEND);
            } catch (IOException e) {
                System.out.println("can't write in the file");
            }
        } else{
            try {
                Files.write(OutputHistory, data.getBytes(), StandardOpenOption.APPEND);
            } catch (IOException e) {
                System.out.println("cant write in the file");
                e.printStackTrace();
            }
        }
    }
}
