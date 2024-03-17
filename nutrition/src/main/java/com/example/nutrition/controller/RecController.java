package com.example.nutrition.controller;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.ResponseBody;

import com.example.nutrition.entity.Result;
import com.example.nutrition.service.RecService;

@Controller
@ResponseBody
public class RecController {
    @Autowired
    private RecService recService;

    @PostMapping("/data-recommendation/foods")
    public Result getDataRecFood(@RequestBody Map<String, Object> map) {
        return recService.getDataRecFood(
            map.get("username").toString(), 
            map.get("record_time").toString(), 
            map.get("conditions").toString(), 
            map.get("symptoms").toString(), 
            map.get("treatments").toString(), 
            map.get("recommendation_time").toString()
        );
    }

    @PostMapping("/data-recommendation/history")
    public Result getDataRecHistory(@RequestBody Map<String, Object> map) {
        return recService.getDataRecHistory(map.get("username").toString());
    }

    @SuppressWarnings("unchecked")
    @PostMapping("/date-recommendation/foods")
    public Result getDateRecFood(@RequestBody Map<String, Object> map) {
        List<String> foods=(List<String>)map.get("foods");
        List<String> dates=(List<String>)map.get("dates");
        List<String> mealtimes=(List<String>)map.get("mealtimes");
        List<Integer> quantities=(List<Integer>)map.get("quantities");
        return recService.getDateRecFood(
            map.get("username").toString(),
            foods,
            dates,
            mealtimes,
            quantities,
            map.get("recommendation_time").toString()
        );
    }

    @PostMapping("/date-recommendation/history")
    public Result getDateRecHistory(@RequestBody Map<String, Object> map) {
        return recService.getDateRecHistory(map.get("username").toString());
    }

    @PostMapping("/habit-recommendation/foods")
    public Result getHabitRecFood(@RequestBody Map<String, Object> map) {
        return recService.getHabitRecFood(
            map.get("username").toString(), 
            map.get("record_time").toString(), 
            Integer.parseInt(map.get("sugar_content").toString()), 
            Integer.parseInt(map.get("salt_content").toString()), 
            Integer.parseInt(map.get("fat_content").toString())
        );
    }

    @PostMapping("/habit-recommendation/history")
    public Result getHabitRecHistory(@RequestBody Map<String, Object> map) {
        return recService.getHabitRecHistory(map.get("username").toString());
    }

    @PostMapping("/region-recommendation/rec")
    public Result getRegionRecFood(@RequestBody Map<String, Object> map) {
        return recService.getRegionRecFood(
            map.get("username").toString(), 
            map.get("recommendation_time").toString(), 
            map.get("region").toString()
        );
    }

    @PostMapping("/region-recommendation/history")
    public Result getRegionRecHistory(@RequestBody Map<String, Object> map) {
        return recService.getRegionRecHistory(map.get("username").toString());
    }

    @PostMapping("/job-recommendation/rec")
    public Result getJobRecFood(@RequestBody Map<String, Object> map) {
        return recService.getJobRecFood(
            map.get("username").toString(), 
            map.get("recommendation_time").toString(), 
            map.get("job").toString()
        );
    }

    @PostMapping("/job-recommendation/history")
    public Result getJobRecHistory(@RequestBody Map<String, Object> map) {
        return recService.getJobRecHistory(map.get("username").toString());
    }
}
