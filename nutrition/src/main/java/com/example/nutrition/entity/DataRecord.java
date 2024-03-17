package com.example.nutrition.entity;

public class DataRecord {
    private Integer user_id;
    private String record_time;
    private String conditions;
    private String symptoms;
    private String treatments;

    public DataRecord() {
    }

    public DataRecord(Integer user_id, String record_time, String conditions, String symptoms, String treatments) {
        this.user_id = user_id;
        this.record_time = record_time;
        this.conditions = conditions;
        this.symptoms = symptoms;
        this.treatments = treatments;
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

    public String getConditions() {
        return conditions;
    }

    public void setConditions(String conditions) {
        this.conditions = conditions;
    }

    public String getSymptoms() {
        return symptoms;
    }

    public void setSymptoms(String symptoms) {
        this.symptoms = symptoms;
    }

    public String getTreatments() {
        return treatments;
    }

    public void setTreatments(String treatments) {
        this.treatments = treatments;
    }
}
