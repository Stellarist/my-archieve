package com.example.nutrition.controller;

import java.util.Iterator;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.ResponseBody;

import com.example.nutrition.entity.Result;
import com.example.nutrition.entity.User;
import com.example.nutrition.service.UserService;

@Controller
@ResponseBody
public class UserController {
    @Autowired
    private UserService userService;

    @PostMapping("/login/signup")
    public Result signup(@RequestBody User user) {
        int code = userService.signUp(user);
        return new Result(code);
    }

    @PostMapping("/login/signin")
    public Result signin(@RequestBody User user) {
        int code = userService.signIn(user);
        user = userService.getUser(user.getUsername());
        user.setPassword(null);
        return new Result(code, user);
    }

    @PostMapping("/settings/update-username")
    public Result updateUsername(@RequestBody List<String> list) {
        Iterator<String> it = list.iterator();
        int code = userService.updateUsername(it.next(), it.next());
        return new Result(code);
    }

    @PostMapping("/settings/update-password")
    public Result updatePassword(@RequestBody List<String> list) {
        Iterator<String> it = list.iterator();
        int code = userService.updatePassword(it.next(), it.next(), it.next());
        return new Result(code);
    }

    @PostMapping("/settings/update-details")
    public Result updateDetails(@RequestBody User user) {
        int code = userService.updateDetails(user);
        return new Result(code);
    }
}
