package com.example.chatting.frame;

import com.example.chatting.controller.ChatController;
import com.example.chatting.domain.Message;
import com.example.chatting.domain.User;
import com.example.chatting.service.ClientService;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class ClientFrame extends javafx.application.Application {
    public static Stage stage;
    public static User user;
    private static FXMLLoader index_fxmlLoader;
    private static FXMLLoader chat_fxmlLoader;
    private static Scene index_scene;
    private static Scene chat_scene;
    public static ClientService client_service = new ClientService();

    @Override
    public void start(Stage new_stage) throws IOException {
        stage = new_stage;
        index_fxmlLoader = new FXMLLoader(ClientFrame.class.getResource("index.fxml"));
        chat_fxmlLoader = new FXMLLoader(ClientFrame.class.getResource("chat.fxml"));
        index_scene = new Scene(index_fxmlLoader.load());
        chat_scene = new Scene(chat_fxmlLoader.load());
        stage.setTitle("Chatting");
        stage.setResizable(false);
        stage.setOnCloseRequest(windowEvent -> {
            System.exit(0);
        });
        stage.setScene(index_scene);
        stage.show();
    }

    public void run() {
        launch();
    }

    public static void changeScene(int scene) {
        if (scene == 1) {
            stage.close();
            stage.setScene(index_scene);
            stage.show();
        } else if (scene == 2) {
            ((ChatController) chat_fxmlLoader.getController()).setLabel(user);
            stage.close();
            stage.setScene(chat_scene);
            stage.show();
        }
    }

    public static void updateMessage(Message message) {
        Platform.runLater(() -> {
            ((ChatController) chat_fxmlLoader.getController()).addMessage(false, message);
        });
    }
}