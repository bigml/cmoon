LOCALBASE = ../../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

FILES = b/base.css \
        b/index.css \
        b/readme.css \
        b/admin/spd/post.css b/admin/spd/robot.css \
        b/bore/bore.css b/bore/op.css \
        b/plan/info.css \
        b/inbox/system.css

all: preload $(FILES)

preload:
	@mkdir -p b/
	@mkdir -p b/admin/spd/
	@mkdir -p b/member/
	@mkdir -p b/bore/
	@mkdir -p b/plan/
	@mkdir -p b/inbox/

b/base.css: reset.css base.css layout.css modules/overlay.css

b/index.css: index/dateinput.css index/index.css
b/readme.css: readme.css

b/admin/spd/post.css: admin/spd/post.css
b/admin/spd/robot.css: admin/spd/robot.css

b/bore/bore.css: bore/bore.css
b/bore/op.css: bore/mku_skin.css bore/mku_set.css bore/op.css

b/plan/info.css: plan/info.css

b/inbox/system.css: modules/accordion.css modules/box.css modules/pagenav.css \
                    inbox/inbox.css

%.css:
	$(CSSGLUE) $^ > $@


clean:
	@rm -f $(FILES)
