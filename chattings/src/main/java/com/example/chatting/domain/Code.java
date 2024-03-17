package com.example.chatting.domain;

import javafx.scene.control.Alert;

public class Code {
    public static final int TYPE_TEXT = 1;
    public static final int TYPE_FILE = 2;
    public static final int TYPE_SIGNIN = 3;
    public static final int TYPE_SIGNUP = 4;
    public static final int SIGIN_SUCCESS = 100;
    public static final int UNFOUND_USER = 101;
    public static final int WRONG_PASSWORD = 102;
    public static final int SIGNUP_SUCCESS = 200;
    public static final int USED_USERNAME = 201;
    public static final int INCONSISTENT_PASSWORD = 202;
    public static final int CONNECTION_TIMEOUT = 300;
    public static final int INVALID_INPUT = 301;
    public static final int INVALID_FILE = 400;
    public static Alert info;
    public static Alert alert;

    public static void note(int error_code) {
        info = new Alert(Alert.AlertType.INFORMATION);
        alert = new Alert(Alert.AlertType.ERROR);
        switch (error_code) {
            case Code.SIGIN_SUCCESS:
                info.setContentText("Sign in succeeded.");
                break;
            case Code.SIGNUP_SUCCESS:
                info.setContentText("Sign up succeeded.");
                break;
            case Code.UNFOUND_USER:
                alert.setContentText("User not found.");
                break;
            case Code.WRONG_PASSWORD:
                alert.setContentText("Wrong password.");
                break;
            case Code.USED_USERNAME:
                alert.setContentText("Used username.");
                break;
            case Code.INCONSISTENT_PASSWORD:
                alert.setContentText("Inconsistent password.");
                break;
            case Code.CONNECTION_TIMEOUT:
                alert.setContentText("Connection timeout.");
                break;
            case Code.INVALID_INPUT:
                alert.setContentText("Invalid input.");
                break;
            case Code.INVALID_FILE:
                alert.setContentText("Invalid file.");
                break;
        }
        if (error_code == Code.SIGIN_SUCCESS || error_code == Code.SIGNUP_SUCCESS)
            info.showAndWait();
        else
            alert.showAndWait();
    }
}
