package src;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class OperatorDashboard {
    private static SerialPort serialPort;
    private static JLabel statusLabel, tempLabel, fillLabel;

    public static void main(String[] args) {
        // Configurazione porta seriale
        serialPort = new SerialPort("COM3"); // Sostituire "COM3" con la porta corretta

        try {
            serialPort.openPort();
            serialPort.setParams(SerialPort.BAUDRATE_9600,
                                 SerialPort.DATABITS_8,
                                 SerialPort.STOPBITS_1,
                                 SerialPort.PARITY_NONE);
            
            serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | 
                                          SerialPort.FLOWCONTROL_RTSCTS_OUT);

            setupSerialListener();
            System.out.println("Connected to Arduino");
        } catch (SerialPortException e) {
            System.out.println("Failed to connect to Arduino: " + e.getMessage());
            return;
        }

        // Creazione della GUI
        javax.swing.SwingUtilities.invokeLater(() -> createAndShowGUI());
    }

    // Metodo per creare l'interfaccia grafica
    private static void createAndShowGUI() {
        JFrame frame = new JFrame("Smart Waste Dashboard");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(300, 200);

        JPanel panel = new JPanel();
        frame.add(panel);
        panel.setLayout(new GridLayout(5, 1));

        statusLabel = new JLabel("Status: Waiting");
        tempLabel = new JLabel("Temperature: ");
        fillLabel = new JLabel("Fill level: ");

        JButton emptyButton = new JButton("Empty");
        JButton restoreButton = new JButton("Restore");

        // Azioni dei pulsanti per inviare comandi ad Arduino
        emptyButton.addActionListener(e -> sendCommand('E'));
        restoreButton.addActionListener(e -> sendCommand('R'));

        panel.add(statusLabel);
        panel.add(tempLabel);
        panel.add(fillLabel);
        panel.add(emptyButton);
        panel.add(restoreButton);

        frame.setVisible(true);
    }

    // Metodo per inviare comandi ad Arduino tramite seriale
    private static void sendCommand(char command) {
        try {
            if (serialPort != null && serialPort.isOpened()) {
                serialPort.writeByte((byte) command);
            }
        } catch (SerialPortException e) {
            System.out.println("Error sending command: " + e.getMessage());
        }
    }

    // Configurazione del listener per eventi sulla seriale
    private static void setupSerialListener() {
        try {
            serialPort.addEventListener(new SerialPortEventListener() {
                @Override
                public void serialEvent(SerialPortEvent event) {
                    if (event.isRXCHAR() && event.getEventValue() > 0) {
                        try {
                            String data = serialPort.readString(event.getEventValue());
                            updateGUI(data.trim());
                        } catch (SerialPortException e) {
                            System.out.println("Error reading serial data: " + e.getMessage());
                        }
                    }
                }
            });
        } catch (SerialPortException e) {
            System.out.println("Error setting up serial listener: " + e.getMessage());
        }
    }

    // Metodo per aggiornare la GUI con i dati ricevuti da Arduino
    private static void updateGUI(String data) {
        String[] parts = data.split(",");
        if (parts.length == 2) {
            tempLabel.setText("Temperature: " + parts[0]);
            fillLabel.setText("Fill level: " + parts[1]);
        }
    }
}
