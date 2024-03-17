package com.example.nutrition.entity;

public class DataRecommendation {
    private Integer user_id;
    private String record_time;
    private String recommendation_time;
    private Integer recommendation_id;
    private Integer food_id;
    private String food_name;

    public DataRecommendation() {
    }

    public DataRecommendation(Integer user_id, String record_time, String recommendation_time,
            Integer recommendation_id, Integer food_id, String food_name) {
        this.user_id = user_id;
        this.record_time = record_time;
        this.recommendation_time = recommendation_time;
        this.recommendation_id = recommendation_id;
        this.food_id = food_id;
        this.food_name = food_name;
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

    public String getRecommendation_time() {
        return recommendation_time;
    }

    public void setRecommendation_time(String recommendation_time) {
        this.recommendation_time = recommendation_time;
    }

    public Integer getRecommendation_id() {
        return recommendation_id;
    }

    public void setRecommendation_id(Integer recommendation_id) {
        this.recommendation_id = recommendation_id;
    }

    public Integer getFood_id() {
        return food_id;
    }

    public void setFood_id(Integer food_id) {
        this.food_id = food_id;
    }

    public void setFood_name(String food_name) {
        this.food_name = food_name;
    }

    public String getFood_name() {
        return food_name;
    }
}
