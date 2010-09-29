; var bmoon = bmoon || {};
bmoon.oms = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.oms;
		if (o.inited) return o;

		o.scroll = $(".scrollable").scrollable({api: true});
		o.navi = $(".scrollable").navigator({api: true});

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.oms.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.oms.init();

		$('#im-users li .remove').click(o.removeCamer);
	},

	removeCamer: function() {
		var o = bmoon.oms.init();

		var
		c = $(this),
		p = c.parent(),
		uname = p.attr('uname');

		if (!confirm('确认删除 ' + uname + ' ?')) {
			return;
		}

		$.post('/oms/camer', {op: 'del', uname: uname}, function(data) {
			if (data.success == '1') {
				p.remove();
			} else {
				alert(data.errmsg || '删除操作失败，请稍后再试');
			}
		}, 'json');
	}
};

$(document).ready(bmoon.oms.onready);
