package it.unibo.impl;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.time.LocalDate;
import java.time.LocalTime;

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
    private String filterHistory = "\\[[A-Za-z]+\\]";
    private Path dataHistory = Paths.get("../../logs/data.txt");
    private Path logHistory = Paths.get("../../logs/logs.txt");

    public OperatorDashboardGui() {

        try {
            Files.write(dataHistory, "Data History\n".getBytes(), StandardOpenOption.CREATE);
            Files.write(logHistory, "Log History\n".getBytes(), StandardOpenOption.CREATE);
        } catch (IOException e) {
            System.out.println("can't write the file");
        }

        try {
            String startingLoggingMessage = "\n\n\n\n\nLogging started on " + LocalDate.now().toString() + " at " + LocalTime.now().toString() + "\n\n\n";
            Files.write(dataHistory, startingLoggingMessage.getBytes(), StandardOpenOption.APPEND);
            Files.write(logHistory, startingLoggingMessage.getBytes(), StandardOpenOption.APPEND);
        } catch (IOException e) {
            System.out.println("cant write in the file");
            e.printStackTrace();
        }

        this.setTitle("Smart Waste Dashboard");
        GridBagConstraints c = new GridBagConstraints();
        
        connection = new DashboardConnection(this, "/dev/cu.usbmodem1301");

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
        if(parts[0].matches(filterHistory)) {
            try {
                String log = LocalTime.now().toString() + " -> " + data;
                Files.write(logHistory, log.getBytes(), StandardOpenOption.APPEND);
            } catch (IOException e) {
                System.out.println("cant write in the file");
                e.printStackTrace();
            }
        }
        else {
            if (parts.length == 3) {
                switch (parts[0]) {
                    case "0":
                        this.statusLabel.setText("Status: Disposing");
                        break;
                    case "1":
                        this.statusLabel.setText("Status: Maintenance");
                        break;
                    case "2":
                        this.statusLabel.setText("Status: User detection");
                        break;
                    case "3":
                        this.statusLabel.setText("Status: Waiting");
                        break;
                    default:
                        this.statusLabel.setText("Status: Error code " + parts[0]);
                        break;
                }
                this.tempLabel.setText("Temperature: " + parts[2]);
                this.fillLabel.setText("Fill level: " + parts[1]);
                try {
                    String dataLog = LocalTime.now().toString() + " -> " + "Temperature: " + parts[2].substring(0,parts[2].length()-1) + "\tFill level: " + parts[1] + "%\n";
                    Files.write(dataHistory, dataLog.getBytes(), StandardOpenOption.APPEND);
                } catch (IOException e) {
                    System.out.println("can't write in the file");
                }
            }
        }
    }
}
