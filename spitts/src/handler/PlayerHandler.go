package handler

import (
	"spitts/src/entity"
	"spitts/src/service"

	"github.com/gin-gonic/gin"
)

func ListPlayer(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListPlayer(json["player_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func InsertPlayer(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var player entity.Player = entity.Player{
		Name: json["name"].(string),
		Team: json["team"].(string),
		Type: int(json["type"].(float64)),
	}

	res := service.InsertPlayer(player)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func DeletePlayer(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.DeletePlayer(json["player_id"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdatePlayer(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var player entity.Player = entity.Player{
		Name: json["name"].(string),
		Team: json["team"].(string),
		Type: int(json["type"].(float64)),
		Id:   int(json["id"].(float64)),
	}

	res := service.UpdatePlayer(player)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func ListPlayerHistory(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListPlayerHistory(json["player_id"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func ListPlayerHonor(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListPlayerHonor(json["player_id"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdatePlayerHonor(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var player_honor entity.PlayerHonor = entity.PlayerHonor{
		Honor_name:   json["honor_name"].(string),
		Award_date:   json["award_date"].(string),
		Recipient_id: int(json["recipient_id"].(float64)),
	}

	res := service.UpdatePlayerHonor(player_honor)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}
