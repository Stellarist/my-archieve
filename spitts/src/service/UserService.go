package service

import (
	"spitts/src/dao"
	"spitts/src/entity"
	"spitts/src/global"
)

func SignUp(user entity.User) entity.Result {
	if dao.IsRegistered(user.Username) {
		return entity.Result{Code: global.USED_USERNAME}
	}

	err := dao.InsertUser(user.Username, user.Password, 0)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func SignIn(user entity.User) entity.Result {
	if !dao.IsRegistered(user.Username) {
		return entity.Result{Code: global.USER_NOTFOUND}
	}

	if !dao.CheckPassword(user.Username, user.Password) {
		return entity.Result{Code: global.WRONG_PASSWORD}
	}

	return_user := dao.SelectUser(user.Username)
	return_user.Password = ""
	if return_user.Username == "" {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: return_user}
}

func UpdateUsername(old_username string, new_username string) entity.Result {
	if !dao.IsRegistered(old_username) {
		return entity.Result{Code: global.USER_NOTFOUND}
	} else if dao.IsRegistered(new_username) {
		return entity.Result{Code: global.USED_USERNAME}
	}

	err := dao.UpdateUsername(old_username, new_username)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func UpdatePassword(username string, old_password string, new_password string) entity.Result {
	if !dao.CheckPassword(username, old_password) {
		return entity.Result{Code: global.WRONG_PASSWORD}
	}

	err := dao.UpdatePassword(username, new_password)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}
