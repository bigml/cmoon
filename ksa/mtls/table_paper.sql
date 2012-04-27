CREATE TABLE paper (
    id SERIAL,
    statu int NOT NULL DEFAULT 0,

    title varchar(256) NOT NULL DEFAULT '',
    hint varchar(256) NOT NULL DEFAULT '',
    content text NOT NULL DEFAULT '',

    previd int NOT NULL DEFAULT 0,
    nextid int NOT NULL DEFAULT 0,
    
    intime timestamp DEFAULT now(),
    uptime timestamp DEFAULT now(),
    PRIMARY KEY (id)
);
CREATE TRIGGER tg_uptime_paper BEFORE UPDATE ON paper FOR EACH ROW EXECUTE PROCEDURE update_time();
