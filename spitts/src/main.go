package main

import (
	"spitts/src/entity"
	"spitts/src/handler"

	"github.com/gin-gonic/gin"
	_ "github.com/lib/pq"
)

func main() {
	entity.Database.Init()
	entity.Database.Open()

	r := gin.Default()
	r.Use(entity.Cors)

	g := r.Group("/spitts")
	g.POST("/login/signup", handler.SignUp)
	g.POST("/login/signin", handler.SignIn)
	g.OPTIONS("/login/signin", handler.SignIn)
	g.POST("/update/username", handler.UpdateUsername)
	g.POST("/update/password", handler.UpdatePassword)
	g.POST("player/info", handler.ListPlayer)
	g.POST("player/insert", handler.InsertPlayer)
	g.POST("player/delete", handler.DeletePlayer)
	g.POST("player/update", handler.UpdatePlayer)
	g.POST("player/history", handler.ListPlayerHistory)
	g.POST("player/list-honor", handler.ListPlayerHonor)
	g.POST("player/update-honor", handler.UpdatePlayerHonor)
	g.POST("team/info", handler.ListTeam)
	g.POST("team/insert", handler.InsertTeam)
	g.POST("team/delete", handler.DeleteTeam)
	g.POST("team/update", handler.UpdateTeam)
	g.POST("team/history", handler.ListTeamHistory)
	g.POST("team/players", handler.ListTeamPlayers)
	g.POST("team/rank", handler.RankTeam)
	g.POST("team/insert-game", handler.InsertGame)
	g.POST("team/update-game", handler.UpdateGame)
	g.POST("team/list-honor", handler.ListTeamHonor)
	g.POST("team/update-honor", handler.UpdateTeamHonor)

	r.Run()

	entity.Database.Close()
}
