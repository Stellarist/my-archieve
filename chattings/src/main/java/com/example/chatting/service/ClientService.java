package com.example.chatting.service;

import com.example.chatting.domain.Code;
import com.example.chatting.domain.Message;
import com.example.chatting.domain.User;
import com.example.chatting.frame.ClientFrame;

import java.io.*;
import java.net.Socket;

public class ClientService {
    Socket socket;
    public boolean status = false;
    private Message message = new Message();
    private ObjectInputStream ois;
    private ObjectOutputStream oos;

    class ServerHandler extends Thread {
        @Override
        public void run() {
            try {
                while (!status)
                    Thread.sleep(500);
                while (true) {
                    if ((message = (Message) ois.readObject()) != null)
                        dealResponse(message);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        private void dealResponse(Message message) {
            if (message.type == Code.TYPE_FILE) {
                File file = (File) message.object;
                String target_path = System.getProperty("user.dir") + File.separator + "temp" + File.separator + file.getName();
                File target_file = new File(target_path);
                try (InputStream inputStream = new FileInputStream(file);
                     OutputStream outputStream = new FileOutputStream(target_file);) {
                    byte[] buffer = new byte[4096];
                    int bytes_read;
                    while ((bytes_read = inputStream.read(buffer)) != -1) {
                        outputStream.write(buffer, 0, bytes_read);
                        outputStream.flush();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            ClientFrame.updateMessage(message);
        }
    }

    public ClientService() {
        try {
            socket = new Socket("localhost", 8888);
            ois = new ObjectInputStream(socket.getInputStream());
            oos = new ObjectOutputStream(socket.getOutputStream());
            new Thread(new ServerHandler()).start();
        } catch (Exception e1) {
            try {
                socket.close();
            } catch (Exception e2) {
                e2.printStackTrace();
            }
            e1.printStackTrace();
        }
    }

    public void sendMessage(Message message) {
        try {
            oos.writeObject(message);
            oos.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public int signIn(String username, String password, int room) {
        message.type = Code.TYPE_SIGNIN;
        message.object = new User(username, password, room);
        try {
            oos.writeObject(message);
            oos.flush();
            long start_time = System.currentTimeMillis();
            while (System.currentTimeMillis() - start_time < 5000) {
                if ((message = (Message) ois.readObject()) != null) {
                    if (message.type == Code.TYPE_SIGNIN) {
                        return (int) message.object;
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return Code.CONNECTION_TIMEOUT;
    }

    public int signUp(String username, String password) {
        message.type = Code.TYPE_SIGNUP;
        message.object = new User(username, password);
        try {
            oos.writeObject(message);
            oos.flush();
            long start_time = System.currentTimeMillis();
            while (System.currentTimeMillis() - start_time < 5000) {
                if ((message = (Message) ois.readObject()) != null) {
                    if (message.type == Code.TYPE_SIGNUP) {
                        return (int) message.object;
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return Code.CONNECTION_TIMEOUT;
    }
}
