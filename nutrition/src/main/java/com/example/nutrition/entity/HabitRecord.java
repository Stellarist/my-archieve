package com.example.nutrition.entity;

public class HabitRecord {
    private Integer user_id;
    private String record_time;
    private Integer sugar_content;
    private Integer salt_content;
    private Integer fat_content;

    public HabitRecord() {
    }

    public HabitRecord(Integer user_id, String record_time, Integer sugar_content, Integer salt_content,
            Integer fat_content) {
        this.user_id = user_id;
        this.record_time = record_time;
        this.sugar_content = sugar_content;
        this.salt_content = salt_content;
        this.fat_content = fat_content;
    }

    public Integer getUser_id() {
        return user_id;
    }

    public void setUser_id(Integer user_id) {
        this.user_id = user_id;
    }

    public String getRecord_time() {
        return record_time;
    }

    public void setRecord_time(String record_time) {
        this.record_time = record_time;
    }

    public Integer getSugar_content() {
        return sugar_content;
    }

    public void setSugar_content(Integer sugar_content) {
        this.sugar_content = sugar_content;
    }

    public Integer getSalt_content() {
        return salt_content;
    }

    public void setSalt_content(Integer salt_content) {
        this.salt_content = salt_content;
    }

    public Integer getFat_content() {
        return fat_content;
    }

    public void setFat_content(Integer fat_content) {
        this.fat_content = fat_content;
    }
}
