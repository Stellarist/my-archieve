package service

import (
	"spitts/src/dao"
	"spitts/src/entity"
	"spitts/src/global"
)

func ListTeam(team_name string) entity.Result {
	if !dao.HasTeam(team_name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}
	team := dao.GetTeamByName(team_name)

	return entity.Result{Code: global.SUCCESS, Object: team}
}

func InsertTeam(team entity.Team) entity.Result {
	if dao.HasTeam(team.Name) {
		return entity.Result{Code: global.DUPLICATE_TEAM}
	}

	err := dao.InsertTeam(team)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func DeleteTeam(team_name string) entity.Result {
	if !dao.HasTeam(team_name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	err := dao.DeleteTeam(team_name)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func UpdateTeam(team entity.Team) entity.Result {
	if !dao.HasTeam(team.Name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	err := dao.UpdateTeam(team)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func ListTeamHistory(team_name string) entity.Result {
	if !dao.HasTeam(team_name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	games, err := dao.ListTeamHistory(team_name)
	if err != nil {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: games}
}

func ListTeamPlayers(team_name string) entity.Result {
	if !dao.HasTeam(team_name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	players, err := dao.ListTeamPlayers(team_name)
	if err != nil {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: players}
}

func ListTeamHonor(team_name string) entity.Result {
	if !dao.HasTeam(team_name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	honors, err := dao.ListTeamHonor(team_name)
	if err != nil {
		return entity.Result{Code: global.QUERY_ERROR}
	}

	return entity.Result{Code: global.SUCCESS, Object: honors}
}

func UpdateTeamHonor(honor entity.TeamHonor) entity.Result {
	if !dao.HasTeam(honor.Recipient) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	err := dao.UpdateTeamHonor(honor)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func InsertGame(game entity.Game) entity.Result {
	if !dao.HasTeam(game.Home_team) || !dao.HasTeam(game.Away_team) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	err := dao.InsertGame(game)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func UpdateGame(game entity.Game) entity.Result {
	if !dao.HasTeam(game.Home_team) || !dao.HasTeam(game.Away_team) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	if dao.CheckGameCompleted(game) {
		return entity.Result{Code: global.GAME_COMPLETED}
	}

	err := dao.UpdateGame(game)
	if err != nil {
		return entity.Result{Code: global.UPDATE_ERROR}
	}

	return entity.Result{Code: global.SUCCESS}
}

func RankTeam(team_name string) entity.Result {
	if !dao.HasTeam(team_name) {
		return entity.Result{Code: global.TEAM_NOTFOUND}
	}

	location_rank, division_rank := dao.RankTeam(team_name)
	ret := make(map[string]int, 2)
	ret["location_rank"] = location_rank
	ret["division_rank"] = division_rank

	return entity.Result{Code: global.SUCCESS, Object: ret}
}
