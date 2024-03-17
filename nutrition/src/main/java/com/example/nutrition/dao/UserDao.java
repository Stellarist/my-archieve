package com.example.nutrition.dao;

import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

import com.example.nutrition.entity.User;

@Mapper
public interface UserDao {
    @Insert("insert into users (username, password) values (#{username}, #{password})")
    public Integer insertUser(String username, String password);

    @Select("select password from users where username=#{username}")
    public String getPassword(String username);

    @Select("select * from users where username=#{username}")
    public Integer isRegistered(String username);

    @Update("update users set username=#{newUsername} where username=#{oldUsername}")
    public Integer updateUsername(String oldUsername, String newUsername);

    @Update("update users set password=#{newPassword} where username=#{username}")
    public Integer updatePassword(String username, String newPassword);

    @Update("update users set sex=#{sex}, age=#{age}, job=#{job}, signature=#{signature}, region=#{region} where username=#{username}")
    public Integer updateDetails(User user);

    @Update("update users set region=#{region} where id=#{user_id}")
    public Integer updateRegion(int user_id, String region);

    @Update("update users set job=#{job} where id=#{user_id}")
    public Integer updateJob(int user_id, String job);

    @Select("select * from users where username=#{username}")
    public User getByUsername(String username);

    @Select("select name from food where id=#{id}")
    public String getFoodNameById(int id);

    @Select("select id from food where name like #{name} limit 1")
    public Integer getLikelyFoodIdByName(String name);
}
