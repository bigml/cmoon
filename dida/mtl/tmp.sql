ALTER TABLE plan ADD COLUMN subscribe smallint NOT NULL DEFAULT 0;
CREATE INDEX expect_index ON expect (mid, pid, addrtype);

CREATE FUNCTION merge_expect(cmid int, cmname character varying, cpid int, caddrtype int, cstatu int, cgotime int, cdad int, csgeo point, crect box, ckm numeric, crepeat int, csdate character varying, cstime time) RETURNS VOID AS
$$
BEGIN
    LOOP
        -- first try to update the key
        UPDATE expect SET statu=cstatu, gotime=cgotime, dad=cdad, sgeo=csgeo, rect=crect, km=ckm, repeat=crepeat, sdate=csdate, stime=cstime WHERE mid=cmid AND pid=cpid AND addrtype=caddrtype;
        IF found THEN
            RETURN;
        END IF;
        -- not there, so try to insert the key
        -- if someone else inserts the same key concurrently,
        -- we could get a unique-key failure
        BEGIN
            INSERT INTO expect(mid, mname, pid, addrtype, statu, gotime, dad, sgeo, rect, km, repeat, sdate, stime) VALUES (cmid, cmname, cpid, caddrtype, cstatu, cgotime, cdad, csgeo, crect, ckm, crepeat, csdate, cstime);
            RETURN;
        EXCEPTION WHEN unique_violation THEN
            -- do nothing, and loop to try the UPDATE again
        END;
    END LOOP;
END
$$
LANGUAGE plpgsql;
