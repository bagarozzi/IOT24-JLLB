package it.unibo;

import com.formdev.flatlaf.intellijthemes.FlatMonokaiProIJTheme;

public class Dashboard {
    
    public static void main(String[] args) {
        FlatMonokaiProIJTheme.setup();
        new OperatorDashboardGui();
    }
}
