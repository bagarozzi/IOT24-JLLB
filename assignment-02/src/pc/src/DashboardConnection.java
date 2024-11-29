package src;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class DashboardConnection implements Connection{
    private static SerialPort serialPort;
    OperatorDashboardGui odg;

    public DashboardConnection(OperatorDashboardGui odg, String Port) {
        this.odg = odg;
        // Configurazione porta seriale
        serialPort = new SerialPort(Port); // Sostituire "COM3" con la porta corretta
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
        //javax.swing.SwingUtilities.invokeLater(() -> createAndShowGUI());
    }

    // Metodo per inviare comandi ad Arduino tramite seriale
    public void sendCommand(char command) {
        try {
            if (serialPort != null && serialPort.isOpened()) {
                serialPort.writeByte((byte) command);
            }
        } catch (SerialPortException e) {
            System.out.println("Error sending command: " + e.getMessage());
        }
    }

    // Configurazione del listener per eventi sulla seriale
    private void setupSerialListener() {
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
    private void updateGUI(String data) {
        odg.updateGUI(data);
    }
}