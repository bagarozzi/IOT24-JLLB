package it.unibo;

import com.formdev.flatlaf.intellijthemes.FlatMonokaiProIJTheme;

import it.unibo.impl.OperatorDashboardGui;

public class Dashboard {
    
    public static void main(String[] args) {
        FlatMonokaiProIJTheme.setup();
        new OperatorDashboardGui();
    }
}
