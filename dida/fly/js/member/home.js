; var bmoon = bmoon || {};
bmoon.memberhome = {
    version: '1.0',

    init: function() {
        var o = bmoon.memberhome;
        if (o.inited) return o;

        o.e_needtips = $('.tooltip');
        o.e_nav = $('#plan-nav');

        o.e_userpicinfo = $('#user-pic-info');
        o.e_upi_phone = $('#upi-phone');
        o.e_upi_contact = $('#upi-contact');

        o.mid = bmoon.utl.getQueryString('mid');
        
        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.memberhome.init();

        o.e_needtips.tooltip({
            tipClass: 'tipsy tipsy-south',
            layout: '<div><div class="tipsy-inner"/></div>'
        });
        o.bindClick();
        o.getPicInfo();
        bmoon.utl.after(o.rendNav, 'mgd.ntt != undefined', 10);
    },

    bindClick: function() {
        var o = bmoon.memberhome.init();

    },

    getPicInfo: function() {
        var o = bmoon.memberhome.init();

        o.e_upi_phone.attr('src', '/image/member/pic?defs=0&fsize=12.&type=phone&mid='+o.mid);
        o.e_upi_contact.attr('src', '/image/member/pic?defs=0&fsize=12.&type=contact&mid='+o.mid);
    },
    
    rendNav: function() {
        var o = bmoon.memberhome.init();

        var url = o.mid ? '/member/home?mid='+o.mid : '/member/home';

        if (mgd.ntt > mgd.npp) {
            o.e_nav.mnnav({
                ntt: mgd.ntt,
                npg: mgd.npg,
                npp: mgd.npp,
                url: url
            });
        }
    }
};

$(document).ready(bmoon.memberhome.onready);
