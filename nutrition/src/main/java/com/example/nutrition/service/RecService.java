package com.example.nutrition.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.example.nutrition.dao.RecDao;
import com.example.nutrition.dao.UserDao;
import com.example.nutrition.entity.Code;
import com.example.nutrition.entity.DataRecommendation;
import com.example.nutrition.entity.DataRecord;
import com.example.nutrition.entity.DateRecommendation;
import com.example.nutrition.entity.DateRecord;
import com.example.nutrition.entity.FoodList;
import com.example.nutrition.entity.HabitRecommendation;
import com.example.nutrition.entity.HabitRecord;
import com.example.nutrition.entity.JobRecommendation;
import com.example.nutrition.entity.PyScript;
import com.example.nutrition.entity.RegionRecommendation;
import com.example.nutrition.entity.Result;
import com.example.nutrition.global.global;
import com.example.nutrition.service.RecService;
import com.fasterxml.jackson.databind.ObjectMapper;

@Service
public class RecService {
    @Autowired
    private UserDao userDao;
    @Autowired
    private RecDao recDao;

    public Result getDataRecFood(String username, String record_time, String conditions, String symptoms, String treatments, String recommendation_time) {
        try{
            int user_id=userDao.getByUsername(username).getId();
            DataRecord dataRecord=new DataRecord(user_id, record_time, conditions, symptoms, treatments);
            recDao.insertDataRecord(dataRecord);

            Map<String, Object> map=new HashMap<String, Object>();
            map.put("conditions", conditions);
            map.put("symptoms", symptoms);
            map.put("treatments", treatments);

            String str=PyScript.quote(global.data_rec_url, map);
            if(str==null)
                return new Result(Code.SCIPTS_ERROR);

            List<Integer> food_id_list=new ObjectMapper().readValue(str, FoodList.class).getList();

            List<String> food_name_list = new ArrayList<String>();
            int recommendation_id=0;

            for(Integer food_id: food_id_list){
                String food_name=userDao.getFoodNameById(food_id);
                food_name_list.add(food_name);
                DataRecommendation dataRecommendation=new DataRecommendation(user_id, record_time, recommendation_time, ++recommendation_id, food_id, food_name);
                recDao.insertDataRecommendation(dataRecommendation);
            }

            return new Result(Code.UPDATE_OK, food_name_list);
            
        } catch(Exception e) {
            return new Result(Code.UPDATE_ERROR, e.getMessage());
        }
    } 

    public Result getDateRecFood(String username, List<String> foods, List<String> dates, List<String> mealtimes, List<Integer> quantities, String recommendation_time) {
        try {
            int user_id=userDao.getByUsername(username).getId();

            List<Integer> food_id_list = new ArrayList<Integer>();
            for(String food: foods)
                food_id_list.add(userDao.getLikelyFoodIdByName("%"+food+"%"));
            
            for(int i=0; i<foods.size(); i++){
                DateRecord dateRecord=new DateRecord(user_id, food_id_list.get(i), mealtimes.get(i), dates.get(i), quantities.get(i));
                recDao.insertDateRecord(dateRecord);
            }

            Map<String, Object> map=new HashMap<String, Object>();
            map.put("foods", food_id_list);
            map.put("dates", dates);
            map.put("mealtimes", mealtimes);
            map.put("quantities", quantities);

            String str=PyScript.quote(global.date_rec_url, map);
            if(str==null)
                return new Result(Code.SCIPTS_ERROR);

            food_id_list=new ObjectMapper().readValue(str, FoodList.class).getList();

            List<String> food_name_list = new ArrayList<String>();
            int recommendation_id=0;

            for(Integer food_id: food_id_list){
                String food_name=userDao.getFoodNameById(food_id);
                food_name_list.add(food_name);
                DateRecommendation dateRecommendation=new DateRecommendation(user_id, recommendation_time, ++recommendation_id, food_id, food_name);
                recDao.insertDateRecommendation(dateRecommendation);
            }
            return new Result(Code.UPDATE_OK, food_name_list);
        
        } catch (Exception e) {
            return new Result(Code.UPDATE_ERROR, e.getMessage());
        }
    }

    public Result getHabitRecFood(String username, String record_time, int sugar_content, int salt_content, int fat_content) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            HabitRecord habitRecord=new HabitRecord(user_id, record_time, sugar_content, salt_content, fat_content);
            recDao.insertHabitRecord(habitRecord);

            Map<String, Object> map=new HashMap<String, Object>();
            map.put("sugar_content", sugar_content);
            map.put("salt_content", salt_content);
            map.put("fat_content", fat_content);

            String str=PyScript.quote(global.habit_rec_url, map);
            if(str==null)
                return new Result(Code.SCIPTS_ERROR);
            
            List<Integer> food_id_list=new ObjectMapper().readValue(str, FoodList.class).getList();

            List<String> food_name_list = new ArrayList<String>();
            int recommendation_id=0;

            for(Integer food_id: food_id_list){
                String food_name=userDao.getFoodNameById(food_id);
                food_name_list.add(food_name);
                HabitRecommendation habitRecommendation=new HabitRecommendation(user_id, record_time, ++recommendation_id, food_id, food_name);
                recDao.insertHabitRecommendation(habitRecommendation);
            }

            return new Result(Code.UPDATE_OK, food_name_list);

        } catch (Exception e) {
            return new Result(Code.UPDATE_ERROR, e.getMessage());
        }
    }

    public Result getRegionRecFood(String username, String recommendation_time, String region) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            userDao.updateRegion(user_id, region);

            String recommendation=PyScript.quote(global.region_rec_url, region);
            if(recommendation==null)
                return new Result(Code.SCIPTS_ERROR);

            RegionRecommendation regionRecommendation=new RegionRecommendation(user_id, recommendation_time, recommendation);
            recDao.insertRegionRecommendation(regionRecommendation);

            return new Result(Code.UPDATE_OK, regionRecommendation);
    
        } catch (Exception e) {
            return new Result(Code.UPDATE_ERROR, e.getMessage());
        }
    }

    public Result getJobRecFood(String username, String recommendation_time, String job) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            userDao.updateJob(user_id, job);

            String recommendation=PyScript.quote(global.job_rec_url, job);
            if(recommendation==null)
                return new Result(Code.SCIPTS_ERROR);

            JobRecommendation jobRecommendation=new JobRecommendation(user_id, recommendation_time, recommendation);
            recDao.insertJobRecommendation(jobRecommendation);

            return new Result(Code.UPDATE_OK, jobRecommendation);

        } catch (Exception e) {
            return new Result(Code.UPDATE_ERROR, e.getMessage());
        }

    }

    public Result getDataRecHistory(String username) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            List<DataRecommendation> dataRecommendationList=recDao.getDataRecommendationHistory(user_id);
            for(DataRecommendation dataRecommendation: dataRecommendationList)
                dataRecommendation.setFood_name(userDao.getFoodNameById(dataRecommendation.getFood_id()));
            return new Result(Code.QUERY_OK, dataRecommendationList);

        } catch (Exception e) {
            return new Result(Code.QUERY_ERROR, e.getMessage());
        }
    }

    public Result getDateRecHistory(String username) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            List<DateRecommendation> dateRecommendationList=recDao.getDateRecommendationHistory(user_id);
            for(DateRecommendation dateRecommendation: dateRecommendationList)
                dateRecommendation.setFood_name(userDao.getFoodNameById(dateRecommendation.getFood_id()));
            return new Result(Code.QUERY_OK, dateRecommendationList);

        } catch (Exception e) {
            return new Result(Code.QUERY_ERROR, e.getMessage());
        }
    }

    public Result getHabitRecHistory(String username) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            List<HabitRecommendation> habitRecommendationList=recDao.getHabitRecommendationHistory(user_id);
            for(HabitRecommendation habitRecommendation: habitRecommendationList)
                habitRecommendation.setFood_name(userDao.getFoodNameById(habitRecommendation.getFood_id()));
            return new Result(Code.QUERY_OK, habitRecommendationList);

        }catch (Exception e) {
            return new Result(Code.QUERY_ERROR, e.getMessage());
        }
    }

    public Result getRegionRecHistory(String username) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            List<RegionRecommendation> regionRecommendationList=recDao.getRegionRecommendationHistory(user_id);
            return new Result(Code.QUERY_OK, regionRecommendationList);

        } catch (Exception e) {
            return new Result(Code.QUERY_ERROR, e.getMessage());
        }
    }

    public Result getJobRecHistory(String username) {
        try {
            int user_id=userDao.getByUsername(username).getId();
            List<JobRecommendation> jobRecommendationList=recDao.getJobRecommendationHistory(user_id);
            return new Result(Code.QUERY_OK, jobRecommendationList);

        } catch (Exception e) {
            return new Result(Code.QUERY_ERROR, e.getMessage());
        }
    }
}
