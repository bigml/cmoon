; var bmoon = bmoon || {};
bmoon.memberpass = {
    version: '1.0',
    
    init: function() {
        var o = bmoon.memberpass;
        if (o.inited) return o;

        o.e_base_save = $('#base-save');
        o.e_base_setting = $('#base-setting');
        o.e_base_setting.data('_postData', {});

        o.rlink = $('#rlink');
        o.omsn = $('#omsn');
        o.msn = $('#msn');
        o.msnr = $('#msn-r');
        o.submit = $('#submit');

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.memberpass.init();

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.memberpass.init();

        o.msnr.blur(o.msnCheck);
        o.submit.click(o.memberPass);

        $('input', o.e_base_setting).change(function() {
            o.e_base_setting.data('_postData')[$(this).attr('name')] = $(this).val();
            o.e_base_save.removeClass('disabled');
            o.e_base_save.unbind('click').click(o.baseSave);
        });
    },

    baseSave: function() {
        var o = bmoon.memberpass.init();

        var pdata = o.e_base_setting.data('_postData'),
        p = $(this).parent().parent();

        if (!$('.VAL_BASE').inputval()) return;
        
        pdata._op = 'mod';

        $('.vres', p).remove();
        p.removeClass('success').removeClass('error').addClass('loading');

        $.post('/json/member/edit', pdata, function(data) {
            p.removeClass('loading');
            if (data.success == '1') {
                p.addClass('success');
                o.e_base_save.addClass('disabled');
                o.e_base_save.unbind('click');
            } else {
                p.addClass('error');
                $('<span class="vres">' + data.errmsg + '</span>').appendTo(p);
            }
        }, 'json');
    },

    msnCheck: function() {
        var o = bmoon.memberpass.init();
        
        if (!$('.VAL_MEMBERPASS').inputval()) return false;
        
        var msn = o.msn.val(),
        msnr = o.msnr.val(),
        p = $(this).parent();

        $(".vres", p).remove();
        p.removeClass('success').removeClass('error');
        if (msn !== msnr) {
            $('<span class="vres">两次密码不匹配</span>').appendTo(p);
            p.addClass('error');
            return false;
        } else {
            p.addClass('success');
        }
        return true;
    },

    memberPass: function() {
        var o = bmoon.memberpass.init();

        if (!$('.VAL_MEMBERPASS').inputval()) return;
        if (!o.msnCheck()) return;

        var msn = $.md5($.md5(o.msn.val())),
        omsn = o.omsn.length > 0 ? $.md5($.md5(o.omsn.val())) : '',
        rlink = o.rlink.val(),
        p = $(this).parent();

        var pdata = {
            _op: 'mod',
            msn: msn
        };

        if (omsn.length > 1) pdata.omsn = omsn;
        if (rlink.length > 1) pdata.rlink = rlink;

        $(".vres", p).remove();
        p.removeClass('success').removeClass('error').addClass('loading');
        $.post('/json/member/edit', pdata, function(data) {
            p.removeClass('loading');
            if (data.success == 1) {
                p.addClass('success');
            } else {
                p.addClass('error');
                $('<span class="vres">'+ data.errmsg +'</span>').appendTo(p);
            }
        }, 'json');
    }
};

$(document).ready(bmoon.memberpass.onready);
