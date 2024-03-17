CREATE TABLE users (  
    username VARCHAR(64) PRIMARY KEY,
    password VARCHAR(64),
    type INT
);

CREATE TABLE teams (
    name VARCHAR(64) PRIMARY KEY,
    location VARCHAR(64),
    division VARCHAR(64),
    win INT,
    lose INT
);

CREATE TABLE players (
    name VARCHAR(64),
    team VARCHAR(64) REFERENCES teams(name),
    type INT,
    id SERIAL PRIMARY KEY  
);

CREATE TABLE games (
    game_date DATE,
    home_team VARCHAR(64) REFERENCES teams(name),
    away_team VARCHAR(64) REFERENCES teams(name),
    game_status INT,
    home_score INT,
    away_score INT,
    PRIMARY KEY(game_date, home_team)
);

CREATE TABLE player_honor (
    honor_name VARCHAR(64),
    award_date DATE,
    recipient VARCHAR(64),
    recipient_id INT REFERENCES players(id),
    PRIMARY KEY (honor_name, award_date)
);

CREATE TABLE team_honor (  
    honor_name VARCHAR(64),
    award_date DATE,
    recipient VARCHAR(64) REFERENCES teams(name),
    PRIMARY KEY (honor_name, award_date)
);
