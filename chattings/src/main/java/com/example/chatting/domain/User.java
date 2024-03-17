package com.example.chatting.domain;

import java.io.Serializable;

public class User implements Serializable {
    private String username;
    private String password;
    private int room;

    public User() {
    }

    public User(String username, String password) {
        this.username = username;
        this.password = password;
    }

    public User(String username, int room) {
        this.username = username;
        this.room = room;
    }

    public User(String username, String password, int room) {
        this.username = username;
        this.password = password;
        this.room = room;
    }

    @Override
    public String toString() {
        return "User{" +
                "username='" + username + '\'' +
                ", password='" + password + '\'' +
                ", room=" + room +
                '}';
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public int getRoom() {
        return room;
    }

    public void setRoom(int room) {
        this.room = room;
    }
}
