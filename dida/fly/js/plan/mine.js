; var bmoon = bmoon || {};
bmoon.planmine = {
    version: '1.0',

    init: function() {
        var o = bmoon.planmine;
        if (o.inited) return o;

        o.e_subsets = $('#plan-list .op input.set-sub');
        
        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.planmine.init();

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.planmine.init();

        o.e_subsets.click(o.setPlanSub);
    },

    setPlanSub: function() {
        var o = bmoon.planmine.init();

        var id = $(this).attr('rel'),
        name = $(this).attr('name'),
        sub = bmoon.dida.plansub[name],
        checkop = $(this).attr('checked') == 'checked' ? 1: 0,
        p = $(this).parent().parent();

        $('.vres', p).remove();
        p.removeClass('success').removeClass('error').addClass('loading');

        $.getJSON('/json/plan/mine', {
            _op: 'mod',
            id: id,
            subscribe: sub,
            checkop: checkop
        }, function(data) {
            p.removeClass('loading');
            if (data.success == '1') {
                p.addClass('success');
            } else {
                p.addClass('error');
                $('<span class="vres">' + data.errmsg + '</span>').appendTo(p);
            }
        });
    }
};

$(document).ready(bmoon.planmine.onready);
