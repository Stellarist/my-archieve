package dao

import (
	"spitts/src/entity"
	"strconv"
)

func IsRegistered(username string) bool {
	var count int
	query := "select count(*) from users where username = $1"
	entity.Database.DB.QueryRow(query, username).Scan(&count)

	return count != 0
}

func CheckPassword(username string, password string) bool {
	var count int
	query := "select count(*) from users where username = $1 and password = $2"
	entity.Database.DB.QueryRow(query, username, password).Scan(&count)

	return count != 0
}

func SelectUser(username string) entity.User {
	var user entity.User
	query := "select * from users where username = $1"
	entity.Database.DB.QueryRow(query, username).Scan(&user.Username, &user.Password, &user.Type)

	return user
}

func InsertUser(username string, password string, user_type int) error {
	user_t := strconv.Itoa(user_type)
	query := "insert into users(username, password, type) values($1, $2, $3)"
	_, err := entity.Database.DB.Exec(query, username, password, user_t)

	return err
}

func UpdateUsername(old_username string, new_username string) error {
	query := "update users set username = $1 where username = $2"
	_, err := entity.Database.DB.Exec(query, new_username, old_username)

	return err
}

func UpdatePassword(username string, password string) error {
	query := "update users set password = $1 where username = $2"
	_, err := entity.Database.DB.Exec(query, password, username)

	return err
}
