package com.example.chatting.controller;

import com.example.chatting.domain.Code;
import com.example.chatting.domain.User;
import com.example.chatting.frame.ClientFrame;
import javafx.fxml.FXML;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;

public class IndexController {
    @FXML
    private GridPane index_pane;
    @FXML
    private AnchorPane sign_in_pane;
    @FXML
    private AnchorPane sign_up_pane;

    @FXML
    private TextField login_room;
    @FXML
    private TextField login_username;
    @FXML
    private PasswordField login_password;

    @FXML
    private TextField register_username;
    @FXML
    private PasswordField register_password;
    @FXML
    private PasswordField register_repassword;

    @FXML
    protected void onSignInButtonClick() {
        index_pane.setVisible(false);
        sign_in_pane.setVisible(true);
    }

    @FXML
    protected void onSignUpButtonClick() {
        index_pane.setVisible(false);
        sign_up_pane.setVisible(true);
    }

    @FXML
    protected void onLoginButtonClick() {
        int status, room = 0;
        String username = login_username.getText();
        String password = login_password.getText();
        String room_str = login_room.getText();
        if (room_str.matches("\\d+")) {
            room = Integer.parseInt(room_str);
            status = ClientFrame.client_service.signIn(username, password, room);
        } else
            status = Code.INVALID_INPUT;

        if (status == Code.SIGIN_SUCCESS) {
            Code.note(status);
            ClientFrame.client_service.status = true;
            ClientFrame.user = new User(username, room);
            ClientFrame.changeScene(2);
        } else
            Code.note(status);
    }

    @FXML
    protected void onRegisterButtonClick() {
        int status;
        String username = register_username.getText();
        String password = register_password.getText();
        String repassword = register_repassword.getText();
        if (password.equals(repassword) == false)
            status = Code.INCONSISTENT_PASSWORD;
        else
            status = ClientFrame.client_service.signUp(username, password);
        Code.note(status);
        if (status == Code.SIGNUP_SUCCESS) {
            sign_up_pane.setVisible(false);
            index_pane.setVisible(true);
        }
    }

    @FXML
    protected void onIndexKeyPressed(KeyEvent event) {
        if (event.getCode() == KeyCode.ESCAPE)
            System.exit(0);
    }

    @FXML
    protected void onSignInKeyPressed(KeyEvent event) {
        if (event.getCode() == KeyCode.ESCAPE) {
            sign_in_pane.setVisible(false);
            index_pane.setVisible(true);
        }
    }

    @FXML
    protected void onSignUpKeyPressed(KeyEvent event) {
        if (event.getCode() == KeyCode.ESCAPE) {
            sign_up_pane.setVisible(false);
            index_pane.setVisible(true);
        }
    }
}