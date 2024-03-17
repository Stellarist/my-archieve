package com.example.nutrition.entity;

public class DateRecord {
    private Integer user_id;
    private Integer food_id;
    private String mealtime;
    private String datetime;
    private Integer quantity;

    public DateRecord() {
    }

    public DateRecord(Integer user_id, Integer food_id, String mealtime, String datetime, Integer quantity) {
        this.user_id = user_id;
        this.food_id = food_id;
        this.mealtime = mealtime;
        this.datetime = datetime;
        this.quantity = quantity;
    }

    public Integer getUser_id() {
        return user_id;
    }

    public void setUser_id(Integer user_id) {
        this.user_id = user_id;
    }

    public Integer getFood_id() {
        return food_id;
    }

    public void setFood_id(Integer food_id) {
        this.food_id = food_id;
    }

    public String getMealtime() {
        return mealtime;
    }

    public void setMealtime(String mealtime) {
        this.mealtime = mealtime;
    }

    public String getDatetime() {
        return datetime;
    }

    public void setDatetime(String datetime) {
        this.datetime = datetime;
    }

    public Integer getQuantity() {
        return quantity;
    }

    public void setQuantity(Integer quantity) {
        this.quantity = quantity;
    }
}
