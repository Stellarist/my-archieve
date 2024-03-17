package dao

import (
	"spitts/src/entity"
)

func InsertTeam(team entity.Team) error {
	query := "insert into teams(name, location, division, win, lose) values($1, $2, $3, $4, $5)"
	_, err := entity.Database.DB.Exec(query, team.Name, team.Location, team.Division, team.Win, team.Lose)

	return err
}

func DeleteTeam(team_name string) error {
	query := "delete from teams where name = $1"
	_, err := entity.Database.DB.Exec(query, team_name)

	return err
}

func UpdateTeam(team entity.Team) error {
	query := "update teams set location = $1, division = $2, win = $3, lose = $4 where name = $5"
	_, err := entity.Database.DB.Exec(query, team.Location, team.Division, team.Win, team.Lose, team.Name)

	return err
}

func HasTeam(team_name string) bool {
	query := "select count(*) from teams where name = $1"
	var count int
	entity.Database.DB.QueryRow(query, team_name).Scan(&count)

	return count > 0
}

func GetTeamByName(team_name string) entity.Team {
	var team entity.Team
	query := "select * from teams where name = $1"
	entity.Database.DB.QueryRow(query, team_name).Scan(&team.Name, &team.Location, &team.Division, &team.Win, &team.Lose)

	return team
}

func ListTeamPlayers(team_name string) ([]entity.Player, error) {
	query := "select * from players where team = $1"
	rows, err := entity.Database.DB.Query(query, team_name)
	if err != nil {
		return nil, err
	}

	var players []entity.Player
	for rows.Next() {
		var player entity.Player
		err = rows.Scan(&player.Name, &player.Team, &player.Type, &player.Id)
		if err != nil {
			return nil, err
		}
		players = append(players, player)
	}

	return players, nil
}

func ListTeamHistory(team_name string) ([]entity.Game, error) {
	query := "select * from games where home_team = $1 or away_team = $1"
	rows, err := entity.Database.DB.Query(query, team_name)
	if err != nil {
		return nil, err
	}

	var games []entity.Game
	for rows.Next() {
		var game entity.Game
		err = rows.Scan(&game.Game_date, &game.Home_team, &game.Away_team, &game.Game_status, &game.Home_score, &game.Away_score)
		if err != nil {
			return nil, err
		}
		games = append(games, game)
	}

	return games, nil
}

func InsertGame(game entity.Game) error {
	tx, err := entity.Database.DB.Begin()
	if err != nil {
		return err
	}

	query := "insert into games(game_date, home_team, away_team, game_status, home_score, away_score) values($1, $2, $3, $4, $5, $6)"
	_, err = entity.Database.DB.Exec(query, game.Game_date, game.Home_team, game.Away_team, game.Game_status, game.Home_score, game.Away_score)
	if err != nil {
		tx.Rollback()
		return err
	}

	if game.Game_status == 0 {
		return tx.Commit()
	}

	UpdateWon(game)

	return tx.Commit()
}

func UpdateGame(game entity.Game) error {
	tx, err := entity.Database.DB.Begin()
	if err != nil {
		tx.Rollback()
		return err
	}

	query := "update games set game_status = $1, home_score = $2, away_score = $3 where game_date = $4 and home_team = $5 and away_team = $6"
	_, err = entity.Database.DB.Exec(query, game.Game_status, game.Home_score, game.Away_score, game.Game_date, game.Home_team, game.Away_team)
	if err != nil {
		tx.Rollback()
		return err
	}

	if game.Game_status == 0 {
		return tx.Commit()
	}

	UpdateWon(game)

	return tx.Commit()
}

func CheckGameCompleted(game entity.Game) bool {
	var game_status int
	query := "select game_status from games where game_date = $1 and home_team = $2 and away_team = $3"
	entity.Database.DB.QueryRow(query, game.Game_date, game.Home_team, game.Away_team).Scan(&game_status)

	return game_status == 1
}

func UpdateWon(game entity.Game) {
	var win_team, lose_team string
	if game.Home_score > game.Away_score {
		win_team = game.Home_team
		lose_team = game.Away_team
	} else {
		win_team = game.Away_team
		lose_team = game.Home_team
	}

	query := "update teams set win = win + 1 where name = $1"
	entity.Database.DB.QueryRow(query, win_team)

	query = "update teams set lose = lose + 1 where name = $1"
	entity.Database.DB.QueryRow(query, lose_team)
}

func ListTeamHonor(team_name string) ([]entity.TeamHonor, error) {
	query := "select * from team_honor where recipient = $1"
	rows, err := entity.Database.DB.Query(query, team_name)

	if err != nil {
		return nil, err
	}

	var honors []entity.TeamHonor
	for rows.Next() {
		var honor entity.TeamHonor
		err = rows.Scan(&honor.Honor_name, &honor.Award_date, &honor.Recipient)
		if err != nil {
			return nil, err
		}
		honors = append(honors, honor)
	}

	return honors, nil
}

func UpdateTeamHonor(honor entity.TeamHonor) error {
	query := "insert into team_honor (honor_name, award_date, recipient) values($1, $2, $3)"
	_, err := entity.Database.DB.Exec(query, honor.Honor_name, honor.Award_date, honor.Recipient)

	return err
}

func RankTeam(team_name string) (int, int) {
	var location_rank, division_rank int
	team := GetTeamByName(team_name)
	query1 := "select ranking from (select rank() over (partition by location order by case when win+lose=0 then -1 else win/(win+lose) end desc)as ranking, name, location from teams) where name=$1 and location=$2;"
	query2 := "select ranking from (select rank() over (partition by division order by case when win+lose=0 then -1 else win/(win+lose) end desc)as ranking, name, division from teams) where name=$1 and division=$2;"
	entity.Database.DB.QueryRow(query1, team.Name, team.Location).Scan(&location_rank)
	entity.Database.DB.QueryRow(query2, team_name, team.Division).Scan(&division_rank)

	return location_rank, division_rank
}
