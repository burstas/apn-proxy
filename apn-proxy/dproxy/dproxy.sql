create table dproxy_app(
app_id       int unsigned   not null AUTO_INCREMENT PRIMARY key,
app_name     varchar(128) not null,
partition    int not null,
release_key  int not null,
app_cert     varchar(16384) not null,
app_key      varchar(16384) not null,
comment      varchar(512) not null,
create_time  datetime   not null,
update_time  datetime   not null,
UNIQUE INDEX (app_name)
)Engine=InnoDB default charset=UTF8;

create table dproxy_partition
(
partition            int unsigned   not null AUTO_INCREMENT PRIMARY key,
total_thread_num     int not null,
comment              varchar(512) not null,
create_time  datetime   not null,
update_time  datetime   not null,
)Engine=InnoDB default charset=UTF8;


create table dproxy_channel(
channel_id   int unsigned   not null AUTO_INCREMENT PRIMARY key,
channel_name varchar(128) not null,
comment      varchar(512) not null,
create_time  datetime   not null,
update_time  datetime   not null,
UNIQUE INDEX (channel_name)
)Engine=InnoDB default charset=UTF8;


create table dproxy_channel_partition
(
id             int unsigned   not null AUTO_INCREMENT PRIMARY key,
channel_id     int  not null,
partition      int  not null,
thread_num     int  not null,
comment      varchar(512) not null,
create_time  datetime   not null,
update_time  datetime   not null,
UNIQUE INDEX (channel_id, partition)
)Engine=InnoDB default charset=UTF8;

create table dproxy_channel_app
(
id             int unsigned   not null AUTO_INCREMENT PRIMARY key,
channel_id     int  not null,
app_id         int  not null,
comment   varchar(512)   not null,
create_time datetime   not null,
update_time datetime   not null,
UNIQUE INDEX (channel_id, app_id)
)Engine=InnoDB default charset=UTF8;

create table dproxy_change_notice
(
id             int unsigned   not null AUTO_INCREMENT PRIMARY key,
comment   varchar(512)   not null,
create_time datetime   not null,
UNIQUE INDEX (id)
)Engine=InnoDB default charset=UTF8;





