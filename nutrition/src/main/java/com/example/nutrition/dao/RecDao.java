package com.example.nutrition.dao;

import java.util.List;

import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import com.example.nutrition.entity.DataRecommendation;
import com.example.nutrition.entity.DataRecord;
import com.example.nutrition.entity.DateRecommendation;
import com.example.nutrition.entity.DateRecord;
import com.example.nutrition.entity.HabitRecommendation;
import com.example.nutrition.entity.HabitRecord;
import com.example.nutrition.entity.JobRecommendation;
import com.example.nutrition.entity.RegionRecommendation;

@Mapper
public interface RecDao {
    @Insert("insert into data_recommendation (user_id, record_time, recommendation_time, recommendation_id, food_id) values (#{user_id}, #{record_time}, #{recommendation_time}, #{recommendation_id}, #{food_id})")
    public Integer insertDataRecommendation(DataRecommendation dataRecommendation);

    @Insert("insert into data_record (user_id, record_time, conditions, symptoms, treatments) values (#{user_id}, #{record_time}, #{conditions}, #{symptoms}, #{treatments})")
    public Integer insertDataRecord(DataRecord dataRecord);

    @Insert("insert into date_recommendation (user_id, recommendation_time, recommendation_id, food_id) values (#{user_id}, #{recommendation_time}, #{recommendation_id}, #{food_id})")
    public Integer insertDateRecommendation(DateRecommendation dateRecommendation);

    @Insert("insert into date_record (user_id, food_id, mealtime, datetime, quantity) values (#{user_id}, #{food_id}, #{mealtime}, #{datetime}, #{quantity})")
    public Integer insertDateRecord(DateRecord dateRecord);

    @Insert("insert into habit_recommendation (user_id, record_time, recommendation_id, food_id) values (#{user_id}, #{record_time}, #{recommendation_id}, #{food_id})")
    public Integer insertHabitRecommendation(HabitRecommendation habitRecommendation);

    @Insert("insert into habit_record (user_id, record_time, sugar_content, salt_content, fat_content) values (#{user_id}, #{record_time}, #{sugar_content}, #{salt_content}, #{fat_content})")
    public Integer insertHabitRecord(HabitRecord habitRecord);

    @Insert("insert into region_recommendation (user_id, recommendation_time, recommendation) values (#{user_id}, #{recommendation_time}, #{recommendation})")
    public Integer insertRegionRecommendation(RegionRecommendation regionRecommendation);

    @Insert("insert into job_recommendation (user_id, recommendation_time, recommendation) values (#{user_id}, #{recommendation_time}, #{recommendation})")
    public Integer insertJobRecommendation(JobRecommendation jobRecommendation);

    @Select("select * from data_recommendation where user_id=#{user_id} ORDER BY record_time DESC, recommendation_time DESC LIMIT 20")
    public List<DataRecommendation> getDataRecommendationHistory(Integer user_id);

    @Select("select * from date_recommendation where user_id=#{user_id} ORDER BY recommendation_time DESC LIMIT 20")
    public List<DateRecommendation> getDateRecommendationHistory(Integer user_id);

    @Select("select * from habit_recommendation where user_id=#{user_id} ORDER BY record_time DESC LIMIT 20")
    public List<HabitRecommendation> getHabitRecommendationHistory(Integer user_id);

    @Select("select * from region_recommendation where user_id=#{user_id} ORDER BY recommendation_time DESC LIMIT 5")
    public List<RegionRecommendation> getRegionRecommendationHistory(Integer user_id);

    @Select("select * from job_recommendation where user_id=#{user_id} ORDER BY recommendation_time DESC LIMIT 5")
    public List<JobRecommendation> getJobRecommendationHistory(Integer user_id);
}
