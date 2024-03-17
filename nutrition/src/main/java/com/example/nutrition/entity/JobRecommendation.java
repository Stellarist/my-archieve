package com.example.nutrition.entity;

public class JobRecommendation {
    private Integer user_id;
    private String recommendation_time;
    private String recommendation;

    public JobRecommendation() {
    }

    public JobRecommendation(Integer user_id, String recommendation_time, String recommendation) {
        this.user_id = user_id;
        this.recommendation_time = recommendation_time;
        this.recommendation = recommendation;
    }

    public Integer getUser_id() {
        return user_id;
    }

    public void setUser_id(Integer user_id) {
        this.user_id = user_id;
    }

    public String getRecommendation_time() {
        return recommendation_time;
    }

    public void setRecommendation_time(String recommendation_time) {
        this.recommendation_time = recommendation_time;
    }

    public String getRecommendation() {
        return recommendation;
    }

    public void setRecommendation(String recommendation) {
        this.recommendation = recommendation;
    }
}
