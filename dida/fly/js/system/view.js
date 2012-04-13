; var bmoon = bmoon || {};
bmoon.systemview = {
    version: '1.0',

    _showTooltip: function(x, y, contents) {
        $('<div id="tooltip">' + contents + '</div>').css( {
            position: 'absolute',
            display: 'none',
            top: y + 5,
            left: x + 5,
            border: '1px solid #fdd',
            padding: '2px',
            'background-color': '#fee',
            opacity: 0.80
        }).appendTo('body').fadeIn(200);
    },
        
    init: function() {
        var o = bmoon.systemview;
        if (o.inited) return o;

        o.e_viewpic = $('#viewpic');
        o.e_viewwho = $('#viewwho');
        o.e_viewdetail = $('#viewdetail');
        o.e_dateinput = $('.datep');
        
        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.systemview.init();

        $.tools.dateinput.localize("zh",  {
            months:        '一月,二月,三月,四月,五月,六月,七月,八月,九月,十月,十一月,十二月',
            shortMonths:   '一,二,三,四,五,六,七,八,九,十,十一,十二',
            days:          '星期日,星期一,星期二,星期三,星期四,星期五,星期六',
            shortDays:     '日,一,二,三,四,五,六'
        });
        
        o.calendar = o.e_dateinput.dateinput({
            format: 'yyyy-mm-dd',
            lang: 'zh',
            trigger: true,
            firstDay: 1
        });

        bmoon.utl.after(o.rendPageview, 'mgd._done != undefined', 10);

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.systemview.init();

        o.e_viewpic.bind('plothover', o.plotOver);
        o.e_viewpic.bind('plotclick', o.plotClick);
        o.calendar.bind("onShow onHide", function()  {
	        $(this).parent().toggleClass("active"); 
        });
    },

    rendPageview: function() {
        var o = bmoon.systemview.init();

        $.plot(o.e_viewpic,
               [
                   {label: '访问量', data:mgd.pageviews},
                   {label: '用户量', data:mgd.userviews}
               ],
               {
                   xaxis: {mode: 'time', timeformat: '%m-%d %H'},
                   series: {
                       lines: {show: true},
                       points: {show:true}
                   },
                   grid: {
                       hoverable: true,
                       clickable: true
                   }
               });
    },

    plotOver: function(event, pos, item) {
        var o = bmoon.systemview.init();

        if (item) {
            if (o.previousPoint != item.datapoint) {
                o.previousPoint = item.datapoint;
                
                $('#tooltip').remove();
                
                var x = item.datapoint[0],
                y = item.datapoint[1],
                dt = new Date(x),
                Y = dt.getFullYear(), m = dt.getMonth()+1, d = dt.getDate(),
                h = dt.getHours(),
                tm = Y+'-'+m+'-'+d+' '+h;
                
                o._showTooltip(item.pageX, item.pageY,
                               tm + ' ' + item.series.label + ' : ' + y);
            }
        } else {
            $('#tooltip').remove();
            o.previousPoint = null;            
        }
    },

    plotClick: function(evt, pos, item) {
        var o = bmoon.systemview.init();

        if (item) {
            var dt = new Date(item.datapoint[0]),
            Y = dt.getFullYear(), m = dt.getMonth()+1, d = dt.getDate(),
            h = dt.getHours(),
            tm = Y+'-'+m+'-'+d+' '+h+':00:00';

            $.getJSON('/json/system/who', {hour: tm}, function(data) {
                if (data.success == 1 && bmoon.utl.type(data.who) == 'Array') {
                    o.e_viewwho.empty();
                    $(document).mntable(
                        {
                            sender: ['用户名', function(row, col, val, tr) {
                                var content = $('<a day="'+row.day+'" rel="'+val+'">'+val+'</a>');
                                content.click(o.getViewDetail);
                                return content;
                            }],
                            count: '页面访问次数'
                        },
                        data.who,
                        {tbattr: 'class="list"'}
                    ).appendTo(o.e_viewwho);
                }
            });
        }
    },

    getViewDetail: function() {
        var o = bmoon.systemview.init();

        var me = $(this),
        sender = me.attr('rel'),
        day = me.attr('day'),
        pdata = {sender: sender, day: day};

        $.getJSON('/json/system/view/detail', pdata, function(data) {
            if (data.success == 1 && bmoon.utl.type(data.details) == 'Array') {
                o.e_viewdetail.empty();
                $(document).mntable(
                    {
                        intime: '时间',
                        es_three: ['页面', function(row, col, val, tr) {
                            var content = $('<a target="_blank" href="'+row.es_one+row.es_two+'">'+val+'</a>');
                            return content;
                        }]
                    },
                    data.details,
                    {tbattr: 'class="list"'}
                ).appendTo(o.e_viewdetail);
            }
        });
    }
};

$(document).ready(bmoon.systemview.onready);
