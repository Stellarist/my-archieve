package com.example.chatting.domain;

import java.io.Serializable;

public class Message implements Serializable {
    public int type;
    public Object object;
    public User user_info;

    public Message() {
    }

    public Message(int type, Object object, User user_info) {
        this.type = type;
        this.object = object;
        this.user_info = user_info;
    }
}
