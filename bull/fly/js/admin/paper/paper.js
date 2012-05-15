; var bmoon = bmoon || {};
bmoon.admpaperpaper = {
    version: '1.0',

    init: function() {
        var o = bmoon.admpaperpaper;
        if (o.inited) return o;

        o.e_nav = $('#inbox-nav');
        
        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.admpaperpaper.init();

        o.bindClick();

        bmoon.utl.after(o.rendNav, 'mgd.ntt != undefined', 10);
    },

    bindClick: function() {
        var o = bmoon.admpaperpaper.init();

    },
    
    rendNav: function() {
        var o = bmoon.admpaperpaper.init();

        if (mgd.ntt > mgd.npp) {
            o.e_nav.mnnav({
                ntt: mgd.ntt,
                npg: mgd.npg,
                npp: mgd.npp,
                url: '/admin/paper'
            });
        }
    }
};

$(document).ready(bmoon.admpaperpaper.onready);
