package handler

import (
	"spitts/src/entity"
	"spitts/src/service"

	"github.com/gin-gonic/gin"
)

func SignUp(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var user entity.User = entity.User{
		Username: json["username"].(string),
		Password: json["password"].(string),
	}

	res := service.SignUp(user)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func SignIn(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	var user entity.User = entity.User{
		Username: json["username"].(string),
		Password: json["password"].(string),
	}

	res := service.SignIn(user)

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdateUsername(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.UpdateUsername(json["old_username"].(string), json["new_username"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}

func UpdatePassword(c *gin.Context) {
	json := make(map[string]interface{})
	c.BindJSON(&json)

	res := service.UpdatePassword(json["username"].(string), json["old_password"].(string), json["new_password"].(string))

	c.JSON(200, gin.H{
		"code":   res.Code,
		"object": res.Object,
	})
}
