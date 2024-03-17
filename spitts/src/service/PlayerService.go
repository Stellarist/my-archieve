package service

import (
	"spitts/src/dao"
	"spitts/src/entity"
	"spitts/src/global"
	"strconv"
)

func ListPlayer(player_name string) entity.Result {
	players, err := dao.GetLikelyPlayersByName(player_name)
	if err != nil {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: players}
}

func InsertPlayer(player entity.Player) entity.Result {
	err := dao.InsertPlayer(player)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func DeletePlayer(player_id string) entity.Result {
	if !dao.HasPlayer(player_id) {
		return entity.Result{Code: global.PLAYER_NOTFOUND}
	}

	err := dao.DeletePlayer(player_id)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func UpdatePlayer(player entity.Player) entity.Result {
	if !dao.HasPlayer(strconv.Itoa(player.Id)) {
		return entity.Result{Code: global.PLAYER_NOTFOUND}
	}

	err := dao.UpdatePlayer(player)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func ListPlayerHistory(player_id string) entity.Result {
	if !dao.HasPlayer(player_id) {
		return entity.Result{Code: global.PLAYER_NOTFOUND}
	}

	games, err := dao.ListPlayerHistory(player_id)
	if err != nil {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: games}
}

func ListPlayerHonor(player_id string) entity.Result {
	if !dao.HasPlayer(player_id) {
		return entity.Result{Code: global.PLAYER_NOTFOUND}
	}

	honors, err := dao.ListPlayerHonor(player_id)
	if err != nil {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: honors}
}

func UpdatePlayerHonor(honor entity.PlayerHonor) entity.Result {
	if !dao.HasPlayer(strconv.Itoa(honor.Recipient_id)) {
		return entity.Result{Code: global.PLAYER_NOTFOUND}
	}

	honor.Recipient = dao.GetPlayerById(strconv.Itoa((honor.Recipient_id))).Name

	err := dao.UpdatePlayerHonor(honor)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}
