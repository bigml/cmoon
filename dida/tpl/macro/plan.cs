<?cs def:plan_sdate(repeat, sdate) ?>
<?cs if:repeat == #0 ?>
<?cs var:sdate ?>
<?cs elif:repeat == #1 ?>
每日
<?cs elif:repeat == #1 ?>
每周 <?cs var:sdate ?>
<?cs /if ?>
<?cs /def ?>

<?cs def:plan_subscribe(val, which) ?><?cs if:bitop.and(val, which) == #which ?>checked="checked"<?cs /if ?><?cs /def ?>
