package handler

import (
	"spitts/src/entity"
	"spitts/src/service"

	"github.com/gin-gonic/gin"
)

func ListTeam(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListTeam(json["team_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func InsertTeam(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var team entity.Team = entity.Team{
		Name:     json["name"].(string),
		Location: json["location"].(string),
		Division: json["division"].(string),
		Win:      int(json["win"].(float64)),
		Lose:     int(json["lose"].(float64)),
	}

	res := service.InsertTeam(team)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func DeleteTeam(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.DeleteTeam(json["team_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdateTeam(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var team entity.Team = entity.Team{
		Name:     json["name"].(string),
		Location: json["location"].(string),
		Division: json["division"].(string),
		Win:      int(json["win"].(float64)),
		Lose:     int(json["lose"].(float64)),
	}

	res := service.UpdateTeam(team)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func ListTeamHistory(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListTeamHistory(json["team_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func ListTeamPlayers(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListTeamPlayers(json["team_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func ListTeamHonor(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.ListTeamHonor(json["team_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdateTeamHonor(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var honor entity.TeamHonor = entity.TeamHonor{
		Honor_name: json["honor_name"].(string),
		Award_date: json["award_date"].(string),
		Recipient:  json["recipient"].(string),
	}

	res := service.UpdateTeamHonor(honor)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func RankTeam(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.RankTeam(json["team_name"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func InsertGame(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var game entity.Game = entity.Game{
		Game_date:   json["game_date"].(string),
		Home_team:   json["home_team"].(string),
		Away_team:   json["away_team"].(string),
		Game_status: int(json["game_status"].(float64)),
		Home_score:  int(json["home_score"].(float64)),
		Away_score:  int(json["away_score"].(float64)),
	}

	res := service.InsertGame(game)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdateGame(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var game entity.Game = entity.Game{
		Game_date:   json["game_date"].(string),
		Home_team:   json["home_team"].(string),
		Away_team:   json["away_team"].(string),
		Game_status: int(json["game_status"].(float64)),
		Home_score:  int(json["home_score"].(float64)),
		Away_score:  int(json["away_score"].(float64)),
	}

	res := service.UpdateGame(game)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}
