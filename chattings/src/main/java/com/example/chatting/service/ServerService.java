package com.example.chatting.service;

import com.example.chatting.domain.Code;
import com.example.chatting.domain.Message;
import com.example.chatting.domain.User;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;

public class ServerService {
    private static List<ClientHandler> clients = new ArrayList<>();
    private ServerSocket server_socket;
    private List<User> user_info;
    private File user_data;

    class ClientHandler extends Thread {
        private Socket client_socket;
        private Message message;
        private ObjectInputStream ois;
        private ObjectOutputStream oos;
        private int room=0;

        ClientHandler(Socket socket) {
            this.client_socket = socket;
            try {
                oos = new ObjectOutputStream(client_socket.getOutputStream());
                ois = new ObjectInputStream(client_socket.getInputStream());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        private void sendMessage(Message message) {
            try {
                oos.writeObject(message);
                oos.flush();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        private void dealRequest(Message message) {
            if (message.type == Code.TYPE_SIGNIN) {
                User user = (User) message.object;
                message.object = signIn(user.getUsername(), user.getPassword(), user.getRoom());
                room=user.getRoom();
                sendMessage(message);
            } else if (message.type == Code.TYPE_SIGNUP) {
                User user = (User) message.object;
                message.object = signUp(user.getUsername(), user.getPassword());
                sendMessage(message);
            } else {
                broadcast(message, this);
            }
        }


        @Override
        public void run() {
            try {
                while (true) {
                    if ((message = (Message) ois.readObject()) != null) {
                        System.out.println(client_socket.getInetAddress() + "'s request");
                        dealRequest(message);
                    }
                }
            } catch (Exception e) {
                System.out.println(client_socket.getInetAddress() + "'s disconnect");
            } finally {
                try {
                    client_socket.close();
                    clients.remove(this);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public ServerService() {
        initInfo();
        initSocket();
    }

    private static void broadcast(Message message, ClientHandler sender) {
        int target_room=message.user_info.getRoom();
        for (ClientHandler client : clients) {
            if (client != sender && target_room==client.room)
                client.sendMessage(message);
        }
    }

    private void initSocket() {
        try {
            server_socket = new ServerSocket(8888);
            System.out.println("login server running:");
            System.out.println(Inet4Address.getLocalHost() + " " + server_socket.getInetAddress() + " " + server_socket.getLocalPort());
            while (true) {
                Socket socket = server_socket.accept();
                socket.setReuseAddress(true);
                ClientHandler client_handler = new ClientHandler(socket);
                clients.add(client_handler);
                System.out.println(socket.getInetAddress() + "'s connect");
                new Thread(client_handler).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private int signIn(String username, String password, int room) {
        for (int i = 0; i < user_info.size(); i++) {
            User user = user_info.get(i);
            if (user.getUsername().equals(username))
                if (user.getPassword().equals(password)) {
                    user.setRoom(room);
                    return Code.SIGIN_SUCCESS;
                } else {
                    return Code.WRONG_PASSWORD;
                }
        }
        return Code.UNFOUND_USER;
    }

    private int signUp(String username, String password) {
        for (User user : user_info)
            if (user.getUsername().equals(username)) {
                return Code.USED_USERNAME;
            }
        addInfo(new User(username, password));
        return Code.SIGNUP_SUCCESS;
    }

    private void initInfo() {
        try {
            user_data = new File(System.getProperty("user.dir") + File.separator + "infos" + File.separator + "info.json");
            user_info = new ObjectMapper().readValue(user_data, new TypeReference<List<User>>() {
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void addInfo(User user) {
        try {
            if (user != null) {
                user_info.add(user);
                new ObjectMapper().writeValue(user_data, user_info);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
