create table users(
    id serial not null primary key,
    username varchar(64) not null,
    password varchar(32) not null,
    sex char(2) default null,
    age int default null,
    job varchar(64) default null,
    signature varchar(256) default null,
    region varchar(64) default null
);

CREATE TABLE food (
  id int unique NOT NULL,
  name varchar(255) NOT NULL,
  xuhao varchar(255) DEFAULT NULL,
  shibu int DEFAULT NULL,
  clcnengliang int DEFAULT NULL,
  nengliang int DEFAULT NULL,
  shuifen decimal(10,2) DEFAULT NULL,
  danbai decimal(10,2) DEFAULT NULL,
  zhifang decimal(10,2) DEFAULT NULL,
  tanshui decimal(10,2) DEFAULT NULL,
  xianwei decimal(10,2) DEFAULT NULL,
  huifen decimal(10,2) DEFAULT NULL,
  reliang int DEFAULT NULL,
  huluobosu int DEFAULT NULL,
  shihuangchun int DEFAULT NULL,
  liuansu int DEFAULT NULL,
  hehuangsu int DEFAULT NULL,
  yansuan int DEFAULT NULL,
  va decimal(10,2) DEFAULT NULL,
  b1 decimal(10,2) DEFAULT NULL,
  b2 decimal(10,2) DEFAULT NULL,
  b3 decimal(10,2) DEFAULT NULL,
  vc decimal(10,2) DEFAULT NULL,
  ve decimal(10,2) DEFAULT NULL,
  jia decimal(10,2) DEFAULT NULL,
  na decimal(10,2) DEFAULT NULL,
  gai decimal(10,2) DEFAULT NULL,
  mei decimal(10,2) DEFAULT NULL,
  tie decimal(10,2) DEFAULT NULL,
  meng decimal(10,2) DEFAULT NULL,
  xin decimal(10,2) DEFAULT NULL,
  tong decimal(10,2) DEFAULT NULL,
  lin decimal(10,2) DEFAULT NULL,
  xi decimal(10,2) DEFAULT NULL,
  leibie int DEFAULT NULL,
  lei int DEFAULT NULL,
  danguchun decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (id));

CREATE TABLE data_record(
    user_id int not null references users(id),
    record_time TIMESTAMP not null,
    conditions varchar(64) not null,
    symptoms varchar(64) not null,
    treatments varchar(64) not null,
    PRIMARY KEY (user_id,record_time)
);

create table data_recommendation(
    user_id int not null,
    record_time timestamp not null,
    recommendation_time timestamp not null,
    recommendation_id int not null ,
    food_id int not null,
    PRIMARY KEY (user_id,record_time,recommendation_time,recommendation_id),
    FOREIGN KEY (user_id, record_time) REFERENCES data_record(user_id, record_time),
    FOREIGN KEY (food_id) REFERENCES food(id)
);

CREATE TABLE date_record(
    user_id int not null references users(id),
    food_id int DEFAULT null references food(id),
    mealtime varchar(4) not null,
    datetime date not null,
    quantity int not null,
    PRIMARY KEY (user_id,food_id,mealtime,datetime)
);

CREATE TABLE date_recommendation(
    user_id int not null references users(id),
    recommendation_time TIMESTAMP not null,
    recommendation_id int not null,
    food_id int DEFAULT null references food(id),
    PRIMARY KEY (user_id,recommendation_time,recommendation_id)
);

CREATE TABLE habit_record(
    user_id int not null references users(id),
    record_time TIMESTAMP not null,
    sugar_content int not null,
    salt_content int not null,
    fat_content int not null,
    PRIMARY KEY (user_id,record_time)
);

CREATE TABLE habit_recommendation(
    user_id int not null,
    record_time TIMESTAMP not null, 
    recommendation_id int not null,
    food_id int DEFAULT null,
    PRIMARY KEY (user_id,record_time, recommendation_id),
    FOREIGN KEY (user_id, record_time) REFERENCES habit_record(user_id, record_time),
    FOREIGN KEY (food_id) REFERENCES food(id)
);

CREATE TABLE region_recommendation(
    user_id int not null references users(id),
    recommendation_time TIMESTAMP not null,
    recommendation varchar(256) DEFAULT null,
    PRIMARY KEY (user_id,recommendation_time)
);

CREATE TABLE job_recommendation(
    user_id int not null references users(id),
    recommendation_time TIMESTAMP not null,
    recommendation varchar(256) DEFAULT null,
    PRIMARY KEY (user_id,recommendation_time)
);
