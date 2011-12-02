; var bmoon = bmoon || {};
bmoon.spd58 = {
    version: '1.0',

    init: function() {
        var o = bmoon.spd58;
        if (o.inited) return o;

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.spd58.init();

        var href = location.href;
        
        if (href.match(/.*\/([0-9]+)x\.shtml$/)) {
            o.parseNode(href.match(/.*\/([0-9]+)x\.shtml$/)[1]);
        } else if (href.match(/.*58.com\/pinche\//)) {
            o.parseList();
        }
    },

    bindClick: function() {
        var o = bmoon.spd58.init();
    },

    parseNode: function(id) {
        var o = bmoon.spd58.init();

        var
        sizes = ['小轿车', '商务车', '越野车', '面包车', '客车', '货车'],
        dad = 0, saddr = '', eaddr = '', marks = '', time = '', size = 0,
        repeat = 0, sdate = '', stime = '', attach = '';

        var x = $('.info').html().match(/[^>]+→[^<]+/);
        x = x && x[0].split('→');
        if (x) {
            saddr = $.trim(x[0]);
            eaddr = $.trim(x[1]);
        }

        dad = $('.headline').html().match('有车') ? 1: 0;

        x = $('.info').html().match(/途经：<\/i>([^\<]+)/);
        marks = x && $.trim(x[1]);
        
        x = $('.info').html().match(/时间：<\/i>([^\<]+)/);
        time = x && $.trim(x[1]).replace("&nbsp;", " ");

        x = $('.info').html().match(/类型：<\/i>([^\<]+)/)[1];
        x = x.split('\n');
        repeat = $.trim(x[0]) == '上下班拼车' ? 2: 0;
        x = $.trim(x[1]);
        if (x) {
            for (var i = 0; i < sizes.length; i++) {
                if (x.match(sizes[i])) {
                    size = i;
                    break;
                }
            }
        }

        sdate = repeat == 2 ? '1,2,3,4,5': time.split(' ')[0];
        stime = time.split(' ')[1] ? time.split(' ')[1]: time.split(' ')[0];

        attach = $('.maincon').html().replace(/\<[^\>]+\>/g, "");

        var
        uid = $('.user').html().match('uid: \'([0-9]+)\''),
        uname = $('.user').html().match('username:\'(.*)\''),
        phone = $('.user > .vuser').html(),
        contact = $('.user > .userinfo').html(),
        city = $('.user > .vuser').html().match('（归属地：(.*)）');
        
        uid = uid && uid[1];
        uname = uname && uname[1];
        phone = phone.match('http://image.58.com/showphone[^\'"]+');
        phone = phone && phone[0];
        contact = contact.match('http://image.58.com/showphone[^\'"]+');
        contact = contact && contact[0];
        city = city && city[1];

        $.getJSON('http://user.58.com/userdata/?callback=?',
                  {userid: uid},
                  function(data) {
                      $.getJSON('http://admin.dida.com/json/spd/do?JsonCallback=?',
                            {
                                _op: 'add',

                                ori: '58',
                                oid: id,
                                mname: data.name + '@58.com',
                                verify: data.license,
                                credit: data.credit,
                                city: city, // need convert to cityid
                                phone: phone,
                                contact: contact,

                                size: size,
                                
                                dad: dad,
                                nick: uname,
                                saddr: saddr,
                                eaddr: eaddr,
                                marks: marks, // convert
                                repeat: repeat,
                                sdate: sdate,
                                stime: stime,
                                attach: attach
                            }, function() {
                                window.opener = null;
                                window.close();
                            });
                  });

        console.log('dad ' + dad);
        console.log('saddr ' + saddr);
        console.log('eaddr ' + eaddr);
        console.log('marks ' + marks);
        console.log('size ' + sizes[size]);
        console.log('repeat ' + repeat);
        console.log('sdate ' + sdate);
        console.log('stime ' + stime);
        console.log('attach ' + attach);

        console.log('uid' + uid);
        console.log('uname' + uname);
        console.log('phone' + phone);
        console.log('contact' + contact);
        console.log('city' + city);
    },
    
    parseList: function() {
        var o = bmoon.spd58.init();

        var
        ids = [], urls = {},
        objs = $('a.t', '#infolist'),
        reg = /.*\/pinche\/([0-9]+)x\.shtml$/;

        $.each(objs, function(i, obj) {
            if ($(obj).attr('href').match(reg)) {
                ids.push($(obj).attr('href').match(reg)[1]);
                urls[$(obj).attr('href').match(reg)[1]] = $(obj).attr('href');
            }
        });

        $.getJSON('http://admin.dida.com/json/spd/pre?JsonCallback=?',
                 {
                     ori: '58',
                     oids: ids
                 }, function(data) {
                     if (data.success == 1 && bmoon.utl.type(data.oids) == 'Object') {
                         $.each(data.oids, function(key, val) {
                             console.log(val);
                             window.open(urls[val]);
                         });
                     } else {
                         alert(data.errmsg || '操作失败');
                     }
                 })
    }
};

$(document).ready(bmoon.spd58.onready);