package dao

import (
	"spitts/src/entity"
	"strconv"
)

func InsertPlayer(player entity.Player) error {
	query := "insert into players(name, team, type) values($1, $2, $3)"
	_, err := entity.Database.DB.Exec(query, player.Name, player.Team, strconv.Itoa(player.Type))

	return err
}

func DeletePlayer(player_id string) error {
	query := "delete from players where id = $1"
	_, err := entity.Database.DB.Exec(query, player_id)

	return err
}

func UpdatePlayer(player entity.Player) error {
	query := "update players set name = $1, team = $2, type = $3 where id = $4"
	_, err := entity.Database.DB.Exec(query, player.Name, player.Team, player.Type, player.Id)

	return err
}

func HasPlayer(player_id string) bool {
	query := "select count(*) from players where id = $1"
	var count int
	entity.Database.DB.QueryRow(query, player_id).Scan(&count)

	return count > 0
}

func GetPlayerById(player_id string) entity.Player {
	var player entity.Player
	query := "select * from players where id = $1"
	entity.Database.DB.QueryRow(query, player_id).Scan(&player.Name, &player.Team, &player.Type, &player.Id)

	return player
}

func GetLikelyPlayersByName(player_name string) ([]entity.Player, error) {
	query := "select * from players where name like '%' || $1 || '%'"
	rows, err := entity.Database.DB.Query(query, player_name)
	if err != nil {
		return nil, err
	}

	var players []entity.Player
	for rows.Next() {
		var player entity.Player
		err := rows.Scan(&player.Name, &player.Team, &player.Type, &player.Id)
		if err != nil {
			return nil, err
		}
		players = append(players, player)
	}

	return players, nil
}

func ListPlayerHistory(player_id string) ([]entity.Game, error) {
	team_name := GetPlayerById(player_id).Team
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

func ListPlayerHonor(player_id string) ([]entity.PlayerHonor, error) {
	query := "select * from player_honor where recipient_id = $1"
	rows, err := entity.Database.DB.Query(query, player_id)
	if err != nil {
		return nil, err
	}

	var honors []entity.PlayerHonor
	for rows.Next() {
		var honor entity.PlayerHonor
		err := rows.Scan(&honor.Honor_name, &honor.Award_date, &honor.Recipient, &honor.Recipient_id)
		if err != nil {
			return nil, err
		}
		honors = append(honors, honor)
	}

	return honors, nil
}

func UpdatePlayerHonor(honor entity.PlayerHonor) error {
	query := "insert into player_honor (honor_name, award_date, recipient, recipient_id) values($1, $2, $3, $4)"
	_, err := entity.Database.DB.Exec(query, honor.Honor_name, honor.Award_date, honor.Recipient, honor.Recipient_id)

	return err
}
