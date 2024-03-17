package com.example.nutrition.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.example.nutrition.dao.UserDao;
import com.example.nutrition.entity.Code;
import com.example.nutrition.entity.User;
import com.example.nutrition.service.UserService;

@Service
public class UserService {
    @Autowired
    private UserDao userDao;

    public int signUp(User user) {
        if (userDao.isRegistered(user.getUsername()) != null)
            return Code.SAME_USERNAME;
        else if (userDao.insertUser(user.getUsername(), user.getPassword()) > 0)
            return Code.SAVE_OK;
        else
            return Code.SAVE_ERROR;
    }

    public int signIn(User user) {
        if (userDao.isRegistered(user.getUsername()) == null)
            return Code.UNFOUND_USER;
        if (user.getPassword().equals(userDao.getPassword(user.getUsername())) == false)
            return Code.WRONG_PASSWORD;
        else
            return Code.GET_OK;
    }

    public int updateUsername(String oldUsername, String newUsername) {
        if (userDao.isRegistered(oldUsername) == null)
            return Code.UNFOUND_USER;
        else if (userDao.isRegistered(newUsername) != null)
            return Code.SAME_USERNAME;
        else if (userDao.updateUsername(oldUsername, newUsername) > 0)
            return Code.UPDATE_OK;
        else
            return Code.UPDATE_ERROR;
    }

    public int updatePassword(String username, String oldPassword, String newPassword) {
        if (userDao.isRegistered(username) == null)
            return Code.UNFOUND_USER;
        else if (oldPassword.equals(userDao.getPassword(username)) == false)
            return Code.WRONG_PASSWORD;
        else if (userDao.updatePassword(username, newPassword) > 0)
            return Code.UPDATE_OK;
        else
            return Code.UPDATE_ERROR;
    }

    public int updateDetails(User user) {
        if (userDao.isRegistered(user.getUsername()) == null)
            return Code.UNFOUND_USER;
        else if (userDao.updateDetails(user) > 0)
            return Code.UPDATE_OK;
        else
            return Code.UPDATE_ERROR;
    }

    public User getUser(String username) {
        return userDao.getByUsername(username);
    }
}
