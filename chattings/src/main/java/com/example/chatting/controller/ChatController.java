package com.example.chatting.controller;

import java.awt.*;
import java.io.File;

import com.example.chatting.domain.Code;
import com.example.chatting.domain.Message;
import com.example.chatting.domain.User;
import com.example.chatting.frame.ClientFrame;
import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.control.Hyperlink;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextArea;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

public class ChatController {
    @FXML
    private Label username_label;
    @FXML
    private Label room_label;
    @FXML
    private TextArea send_text;
    @FXML
    private ScrollPane scroll_pane;
    @FXML
    private VBox message_container;

    public void setLabel(User user) {
        username_label.setText(user.getUsername());
        room_label.setText(((Integer) user.getRoom()).toString());
    }

    public void onSendTextButtonClicked() {
        String message_text = send_text.getText();
        Message message = new Message(Code.TYPE_TEXT, message_text, ClientFrame.user);
        ClientFrame.client_service.sendMessage(message);
        addMessage(true, message);
        send_text.setText("");
    }

    public void onSendFileButtonClicked() {
        FileChooser file_chooser = new FileChooser();
        file_chooser.setTitle("Choose a File");
        Stage stage = (Stage) send_text.getScene().getWindow();
        File selected_file = file_chooser.showOpenDialog(stage);

        if (selected_file != null) {
            Message message = new Message(Code.TYPE_FILE, selected_file, ClientFrame.user);
            ClientFrame.client_service.sendMessage(message);
            addMessage(true, message);
        }
    }

    public void addMessage(boolean is_self, Message message) {
        Label message_label = new Label();
        FlowPane message_flow = new FlowPane();
        message_label.setWrapText(true);

        if (is_self) {
            message_flow.setAlignment(Pos.CENTER_RIGHT);
            message_label.setStyle(
                "-fx-font-size: 16px; " +
                "-fx-background-color: #0078d4; " +
                "-fx-text-fill: #FFFFFF; " +
                "-fx-padding: 5px 10px; " +
                "-fx-margin: 10px 20px 10px 20px; " +
                "-fx-border-radius: 10px 10px 0 10px; " +
                "-fx-background-radius: 10px 10px 0 10px; " +
                "-fx-border-color: #0078d4; " +
                "-fx-border-width: 2px;"
            );
        } else {
            message_flow.setAlignment(Pos.CENTER_LEFT);
            message_label..setStyle(
                "-fx-font-size: 16px; " +
                "-fx-background-color: #a8a8a8; " +
                "-fx-text-fill: #000000; " +
                "-fx-padding: 5px 10px; " +
                "-fx-margin: 10px 20px 10px 20px; " +
                "-fx-border-radius: 10px 10px 10px 0; " +
                "-fx-background-radius: 10px 10px 10px 0; " +
                "-fx-border-color: #a8a8a8; " +
                "-fx-border-width: 2px;"
            );
        }

        Text first_line = new Text(message.user_info.getUsername());
        first_line.setFont(Font.font("Arial", 12));
        first_line.setFill(Color.WHITE);

        if (message.type == Code.TYPE_TEXT) {
            String text = message.object.toString();
            text = insertLineBreaks(text, 30);
            Text second_line = new Text(text);
            second_line.setFont(Font.font("Arial", 18));
            second_line.setFill(Color.BLACK);

            message_label.setGraphic(new VBox(first_line, second_line));

        } else if (message.type == Code.TYPE_FILE) {
            File file = (File) message.object;
            Hyperlink file_link = new Hyperlink(file.getName());
            file_link.setUserData(file.getAbsolutePath());
            file_link.setTextFill(Color.BLACK);
            file_link.setOnAction(event -> {
                try {
                    String path = (String) file_link.getUserData();
                    Desktop.getDesktop().open(new File(path));
                } catch (Exception e) {
                    Code.note(Code.INVALID_FILE);
                    e.printStackTrace();
                }
            });

            message_label.setGraphic(new VBox(first_line, file_link));
        }

        message_flow.getChildren().add(message_label);
        message_container.getChildren().add(message_flow);
        scroll_pane.layout();
        scroll_pane.setVvalue(1.0);
    }

    public static String insertLineBreaks(String input, int lineLength) {
        StringBuilder output = new StringBuilder();
        int length = input.length();

        for (int i = 0; i < length; i += lineLength) {
            int endIndex = Math.min(i + lineLength, length);
            String chunk = input.substring(i, endIndex);
            output.append(chunk);

            if (endIndex < length) {
                output.append("\n");
            }
        }

        return output.toString();
    }
}
