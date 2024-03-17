package global

const (
	Username = "root"
	Password = "Hell0-w0rld"
	Host     = "47.115.204.42"
	Port     = "5432"
	Dbname   = "spitts"
)

const (
	SUCCESS = 20001

	WRONG_PASSWORD  = 20010
	USED_USERNAME   = 20020
	USER_NOTFOUND   = 20030
	UPDATE_ERROR    = 20040
	QUERY_ERROR     = 20050
	PLAYER_NOTFOUND = 20060
	TEAM_NOTFOUND   = 20070
	DUPLICATE_TEAM  = 20080
	GAME_COMPLETED  = 20090
)
