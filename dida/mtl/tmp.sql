alter table emap drop column bversion;
alter table emap add column bversion real not null default 0.0;
