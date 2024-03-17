package entity

import (
	"database/sql"
	"log"
	"net/http"
	"spitts/src/global"

	"github.com/gin-gonic/gin"
)

var Database Sql

type Result struct {
	Code   int
	Object any
}

type User struct {
	Username string
	Password string
	Type     int
}

type Player struct {
	Name string
	Team string
	Type int
	Id   int
}

type Team struct {
	Name     string
	Location string
	Division string
	Win      int
	Lose     int
}

type Game struct {
	Game_date   string
	Home_team   string
	Away_team   string
	Game_status int
	Home_score  int
	Away_score  int
}

type PlayerHonor struct {
	Honor_name   string
	Award_date   string
	Recipient    string
	Recipient_id int
}

type TeamHonor struct {
	Honor_name string
	Award_date string
	Recipient  string
}

type Sql struct {
	DB       *sql.DB
	Username string
	Password string
	Host     string
	Port     string
	Dbname   string
}

func Cors(c *gin.Context) {
	method := c.Request.Method
	c.Header("Access-Control-Allow-Origin", "*")
	c.Header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
	c.Header("Access-Control-Allow-Headers", "Content-Type, Authorization")
	c.Header("Access-Control-Allow-Credentials", "true")
	if method == "OPTIONS" {
		c.AbortWithStatus(http.StatusNoContent)
		return
	}
	c.Next()
}

func (s *Sql) Init() {
	s.Username = global.Username
	s.Password = global.Password
	s.Host = global.Host
	s.Port = global.Port
	s.Dbname = global.Dbname
}

func (s *Sql) Open() {
	var err error
	s.DB, err = sql.Open("postgres", "user="+s.Username+" password="+s.Password+" host="+s.Host+" port="+s.Port+" dbname="+s.Dbname+" sslmode=disable")
	if err != nil {
		log.Fatal(err)
	}
}

func (s *Sql) Close() {
	err := s.DB.Close()
	if err != nil {
		log.Fatal(err)
	}
}
