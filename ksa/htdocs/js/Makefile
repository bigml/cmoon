LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.js \
        b/paper/add.js

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/paper/

b/base.js: pub/jquery-1.7.1.js pub/jquery.cookie.js \
           pub/jquery.bmoon.js pub/jquery.hotkeys.js pub/jquery.mninpval.js

b/paper/add.js: pub/markitup/markitup/jquery.markitup.js paper/mku_set.js \
                  pub/jqueryUploader/src/jquery.uploader.js \
                  pub/noty/js/jquery.noty.js \
                  paper/add.js

%.js:
	@echo -n > $@
	$(JSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
