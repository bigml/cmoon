; var bmoon = bmoon || {};
bmoon.spdpost = {
    version: '1.0',

    _formAddr: function(name, addrs, loc) {
        if (addrs) {
            return '<div><strong>' + name + '</strong><br />' +
                [(addrs[0] &&
                  addrs[0].short_name || ''),
                 (addrs[1] &&
                  addrs[1].short_name || ''),
                 (addrs[2] &&
                  addrs[2].short_name || '')
                ].join(' ') +
                '（' + loc.lat().toFixed(4) + ', ' + loc.lng().toFixed(4) + '）';
        } else return ' ';
    },
    
    init: function() {
        var o = bmoon.spdpost;
        if (o.inited) return o;

        o.saddr = $('#saddr');
        o.eaddr = $('#eaddr');
        o.km = $('#km');
        o.marks = $('#marks');
        o.count = $('#count');
        o.phone = $('#phone');
        o.phoneimg = $('#img-phone');
        o.contact = $('#contact');
        o.contactimg = $('img-contact');
        o.next = $('#next');
        o.map = $('#map');

        o.plan = {};

        o.glat = new google.maps.LatLng(28.188, 113.033);
        o.gmap = new google.maps.Map($('#map')[0], {
            zoom: 10,
            center: o.glat,
            mapTypeId: google.maps.MapTypeId.ROADMAP,
            region: 'zh-CN'
        });

        o.gsauto = new google.maps.places.Autocomplete($('#saddr')[0]);
        o.geauto = new google.maps.places.Autocomplete($('#eaddr')[0]);
        o.ggeocode = new google.maps.Geocoder();
        o.gdirservice = new google.maps.DirectionsService();
        o.gdirrender = new google.maps.DirectionsRenderer();
        o.ginfow = new google.maps.InfoWindow();
        o.gsmarker = new google.maps.Marker({
            map: o.gmap,
            draggable: true,
            title: '设置起始位置'
        });
        o.gemarker = new google.maps.Marker({
            map: o.gmap,
            draggable: true,
            title: '设置终点位置'
        });
        

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.spdpost.init();

        o.bindClick();
        o.getPlan();
    },

    bindClick: function() {
        var o = bmoon.spdpost.init();

        o.gsauto.bindTo('bounds', o.gmap);
        o.geauto.bindTo('bounds', o.gmap);
        o.gdirrender.setMap(o.gmap);

        o.bindChange();
        o.next.click(o.savePlan);
    },

    getPlan: function() {
        var o = bmoon.spdpost.init();

        var opt = o.plan.id ? {exceptid: o.plan.id} : null;

        $.getJSON('/json/spd/post/do', opt, function(data) {
            if (data.success == 1 && bmoon.utl.type(data.plan) == 'Object') {
                var p = data.plan;
                o.saddr.val(p.saddr);
                o.eaddr.val(p.eaddr);
                o.plan = data.plan;
                o.member = data.member;
                o.phoneimg.attr('src', bmoon.utl.clotheHTML(data.member.phone));
                o.contactimg.attr('src', bmoon.utl.clotheHTML(data.member.contact));

                o.next.attr('disabled', 'disabled');
            } else {
                alert(data.errmsg || '获取数据失败，请稍后再试！');
            }
        });
    },

    // {address_components: [], formatted_address: "", geometry: {}...} gdata.js
    upPlan: function(x, data) {
        var o = bmoon.spdpost.init();

        var p = o.plan,
        addrs = data.address_components,
        city = '';

        for (var i = 0; i < addrs.length; i++) {
            if (addrs[i].types[1] == 'political') {
                city = addrs[i].short_name;
                break;
            }
        }

        if (x != 'e') {
            p.sll = [data.geometry.location.lat(), data.geometry.location.lng()];
            p.saddr = data.formatted_address;
            p.scity = city;
        } else {
            p.ell = [data.geometry.location.lat(), data.geometry.location.lng()];
            p.eaddr = data.formatted_address;
            p.ecity = city;
        }

        if (p.sll && p.ell) o.next.removeAttr('disabled');
    },

    savePlan: function() {
        var o = bmoon.spdpost.init();

        var phone = o.phone.val(),
        contact = o.contact.val(),
        p = $(this).parent();
        
        if (!o.plan.sll || !o.plan.ell) return;

        var pdata = {
            _op: 'mod',
            plan: JSON.stringify(o.plan),
            _type_plan: 'object'
        };
        if (phone.length || contact.length) {
            if (phone) o.member.phone = phone;
            if (contact) o.member.contact = contact;
            pdata.member = JSON.stringify(o.member);
            pdata._type_member = 'object';
        }
        
        o.plan.rect = '((' + o.plan.sll.join(',') + '),(' +
            o.plan.ell.join(',') + '))';

		$('.vres', p).remove();
		p.removeClass('success').removeClass('error').addClass('loading');

        $.post('/json/spd/post/do', pdata, function(data) {
            if (data.success == 1) {
                o.count.text(--mgd._ntt);
                p.removeClass('loading');
                p.addClass('success');
                o.getPlan();
            } else {
                p.addClass('error');
                $('<span class="vres">'+ data.errmsg +'</span>').appendTo(p);
            }
        }, 'json');
    },

    rendDirect: function() {
        var o = bmoon.spdpost.init();

        if (!o.plan.sll || !o.plan.ell) return;

        var opts = {
            origin: new google.maps.LatLng(o.plan.sll[0], o.plan.sll[1]),
            destination: new google.maps.LatLng(o.plan.ell[0], o.plan.ell[1]),
            travelMode: google.maps.TravelMode.DRIVING
        };
        
        o.gdirservice.route(opts, function(result, status) {
            if (status == google.maps.DirectionsStatus.OK) {
                o.gdirrender.setDirections(result);
            }
        });
    },

    bindChange: function() {
        var o = bmoon.spdpost.init();

        google.maps.event.addListener(o.gsmarker, 'dragend', function() {
            o.markDraged('s');
        });
        google.maps.event.addListener(o.gemarker, 'dragend', function() {
            o.markDraged('e');
        });

        google.maps.event.addListener(o.gsauto, 'place_changed', function() {
            o.autoChanged('s');
        });
        google.maps.event.addListener(o.geauto, 'place_changed', function() {
            o.autoChanged('e');
        });
    },

    markDraged: function(x) {
        var o = bmoon.spdpost.init();

        var marker = o.gsmarker,
        addr = o.saddr;
        if (x == 'e') {
            marker = o.gemarker;
            addr = o.eaddr;
        }

        var pos = marker.getPosition();
        o.ggeocode.geocode({latLng: pos}, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                addr.val(results[0].formatted_address);
                o.ginfow.setContent(o._formAddr(
                    results[0].address_components[0].short_name,
                    results[0].address_components,
                    results[0].geometry.location));
                o.ginfow.open(o.gmap, marker);

                o.upPlan(x, results[0]);
                o.rendDirect();
            }
        });
    },

    autoChanged: function(x) {
        var o = bmoon.spdpost.init();
        
        var marker = o.gsmarker,
        auto = o.gsauto;
        
        if (x == 'e') {
            marker = o.gemarker;
            auto = o.geauto;
        }

        o.ginfow.close();
        var place = auto.getPlace();
        if (place.geometry.viewport) {
            o.gmap.fitBounds(place.geometry.viewport);
        } else {
            o.gmap.setCenter(place.geometry.location);
        }
        
        marker.setPosition(place.geometry.location);
        
        o.ginfow.setContent(o._formAddr(
            place.name,
            place.address_components,
            place.geometry.location));
        o.ginfow.open(o.gmap, marker);

        o.upPlan(x, place);
        o.rendDirect();
    }
};

$(document).ready(bmoon.spdpost.onready);
