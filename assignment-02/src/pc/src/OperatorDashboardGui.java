package src;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class OperatorDashboardGui extends JFrame{
    private JLabel statusLabel = new JLabel("Status: Waiting");
    private  JLabel tempLabel = new JLabel("Temperature: ");
    private JLabel fillLabel = new JLabel("Fill level: ");

    public static void main(String[] args) {
        JFrame gui = new OperatorDashboardGui(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                // TODO Auto-generated method stub
                throw new UnsupportedOperationException("Unimplemented method 'actionPerformed'");
            }
        });
    }

    public OperatorDashboardGui(ActionListener e) {
        JFrame frame = new JFrame("Smart Waste Dashboard");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(500, 500);

        JPanel panel = new JPanel();
        frame.add(panel);
        panel.setLayout(new GridLayout(5, 1));

        statusLabel = new JLabel("Status: Waiting");
        tempLabel = new JLabel("Temperature: ");
        fillLabel = new JLabel("Fill level: ");

        JButton emptyButton = new JButton("Empty");
        JButton restoreButton = new JButton("Restore");

        // Azioni dei pulsanti per inviare comandi ad Arduino
        emptyButton.addActionListener(e);
        restoreButton.addActionListener(e);

        panel.add(statusLabel);
        panel.add(tempLabel);
        panel.add(fillLabel);
        panel.add(emptyButton);
        panel.add(restoreButton);

        frame.setVisible(true);
    }

    public void updateGUI(String data) {
        String[] parts = data.split(",");
        if (parts.length == 2) {
            this.tempLabel.setText("Temperature: " + parts[0]);
            this.fillLabel.setText("Fill level: " + parts[1]);
        }
    }
}
