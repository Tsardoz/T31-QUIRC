#!/usr/bin/haserl
<%in p/common.cgi %>
<%in p/icons.cgi %>
<%
page_title="Camera preview"
rtsp_address=$network_address
rtsp_username="thingino" # TODO: get from config
rtsp_password="thingino" # TODO: get from config
rtsp_port=554 # TODO: get from config
rtsp_url="rtsp://${rtsp_username}:${rtsp_password}@${rtsp_address}:${rtsp_port}/ch0"

for i in "ispmode"; do
	eval "$i=\"$(/usr/sbin/imp-control $i)\""
done

check_flip() {
	[ $flip -eq 2 ] || [ $flip -eq 3 ] && echo -n " checked"
}

check_mirror() {
	[ $flip -eq 1 ] || [ $flip -eq 3 ] && echo -n " checked"
}
%>
<%in p/header.cgi %>

<div class="row preview">
<div class="col-md-8 col-xl-9 col-xxl-12 mb-3">
<div id="frame" class="position-relative mb-2">
<div class="smpte">
<div class="bar1"></div>
<div class="bar2"></div>
<div class="bar3"></div>
</div>
<img id="preview" class="img-fluid"></img>
<%in p/motors.cgi %>

<div id="controls" class="position-absolute bottom-0 start-0 end-0">
<div class="buttons p-3">
<div class="row">
<div class="col">
<div class="btn-group d-flex" role="group" aria-label="Night Mode">
<input type="checkbox" class="btn-check" name="daynight" id="daynight" value="1"<% checked_if $daynight 1 %>>
<label class="btn btn-lg btn-dark" for="daynight" title="Night mode"><%= $icon_moon %></label>
</div>
</div>
<div class="col">
<div class="btn-group d-flex" role="group" aria-label="Color">
<input type="checkbox" class="btn-check" name="ispmode" id="ispmode" value="1"<% checked_if $ispmode 1 %>>
<label class="btn btn-lg btn-dark" for="ispmode" title="Color mode"><%= $icon_color %></label>
</div>
</div>
<div class="col">
<div class="btn-group d-flex" role="group" aria-label="IR filter">
<input type="checkbox" class="btn-check" name="ircut" id="ircut" value="1"<% checked_if $ircut 1 %><% fw_printenv -n gpio_ircut >/dev/null || echo " disabled" %>>
<label class="btn btn-lg btn-dark" for="ircut" title="IR filter"><%= $icon_ircut %></label>
</div>
</div>
<div class="col">
<div class="btn-group d-flex" role="group" aria-label="Illumination">
<input type="checkbox" class="btn-check" name="ir850" id="ir850" value="1"<% checked_if $ir850 1 %><% fw_printenv -n gpio_ir850 >/dev/null || echo " disabled" %>>
<label class="btn btn-lg btn-dark" for="ir850" title="IR LED 850 nm"><%= $icon_ir850 %></label>
<input type="checkbox" class="btn-check" name="ir940" id="ir940" value="1"<% checked_if $ir940 1 %><% fw_printenv -n gpio_ir940 >/dev/null || echo " disabled" %>>
<label class="btn btn-lg btn-dark" for="ir940" title="IR LED 940 nm"><%= $icon_ir940 %></label>
<input type="checkbox" class="btn-check" name="white" id="white" value="1"<% checked_if $white 1 %><% fw_printenv -n gpio_whled >/dev/null || echo " disabled" %>>
<label class="btn btn-lg btn-dark" for="white" title="White LED"><%= $icon_white %></label>
</div>
</div>
<div class="col">
<div class="btn-group d-flex" role="group" aria-label="Flip and Mirror">
<input type="checkbox" class="btn-check" name="flip" id="flip" value="1"<% check_flip %>>
<label class="btn btn-lg btn-dark" for="flip" title="Flip vertically"><%= $icon_flip %></label>
<input type="checkbox" class="btn-check" name="mirror" id="mirror" value="1"<% check_mirror %>>
<label class="btn btn-lg btn-dark" for="mirror" title="Flip horizontally"><%= $icon_flop %></label>
</div>
</div>
</div>
</div>
</div>
</div>
<p class="small text-body-secondary">The image above refreshes once per second and may appear choppy.<br>
Please open RTSP stream at <i><a href="<%= $rtsp_url %>"><%= $rtsp_url %></a></i> in you favorite media player to see video feed.</p>
</div>
<div class="col-md-4 col-xl-3 col-xxl-12">
<div class="d-flex flex-row gap-2 mb-3">
<div class="input-group">
<button class="form-control btn btn-primary text-start" type="button" data-sendto="email">Email</button>
<div class="input-group-text"><a href="plugin-send2email.cgi" title="Email settings"><%= $icon_gear %></a></div>
</div>
<div class="input-group">
<button class="form-control btn btn-primary text-start" type="button" data-sendto="ftp">FTP</button>
<div class="input-group-text"><a href="plugin-send2ftp.cgi" title="FTP Storage settings"><%= $icon_gear %></a></div>
</div>
<div class="input-group">
<button class="form-control btn btn-primary text-start" type="button" data-sendto="telegram">Telegram</button>
<div class="input-group-text"><a href="plugin-send2telegram.cgi" title="Telegram bot settings"><%= $icon_gear %></a></div>
</div>
<div class="input-group">
<button class="form-control btn btn-primary text-start" type="button" data-sendto="mqtt">MQTT</button>
<div class="input-group-text"><a href="plugin-send2mqtt.cgi" title="MQTT settings"><%= $icon_gear %></a></div>
</div>
<div class="input-group">
<button class="form-control btn btn-primary text-start" type="button" data-sendto="webhook">WebHook</button>
<div class="input-group-text"><a href="plugin-send2webhook.cgi" title="Webhook settings"><%= $icon_gear %></a></div>
</div>
<div class="input-group">
<button class="form-control btn btn-primary text-start" type="button" data-sendto="yadisk">Yandex Disk</button>
<div class="input-group-text"><a href="plugin-send2yadisk.cgi" title="Yandex Disk bot settings"><%= $icon_gear %></a></div>
</div>
</div>
</div>
</div>

<script src="/a/imp-config.js"></script>
<script>
const network_address = "<%= $network_address %>";

<% [ "true" != "$email_enabled"    ] && echo "\$('button[data-sendto=email]').disabled = true;" %>
<% [ "true" != "$ftp_enabled"      ] && echo "\$('button[data-sendto=ftp]').disabled = true;" %>
<% [ "true" != "$mqtt_enabled"     ] && echo "\$('button[data-sendto=mqtt]').disabled = true;" %>
<% [ "true" != "$webhook_enabled"  ] && echo "\$('button[data-sendto=webhook]').disabled = true;" %>
<% [ "true" != "$telegram_enabled" ] && echo "\$('button[data-sendto=telegram]').disabled = true;" %>
<% [ "true" != "$yadisk_enabled"   ] && echo "\$('button[data-sendto=yadisk]').disabled = true;" %>

$$("button[data-sendto]").forEach(el => {
	el.addEventListener("click", ev => {
		ev.preventDefault();
		if (!confirm("Are you sure?")) return false;
		const tgt = ev.target.dataset["sendto"];
		xhrGet("/cgi-bin/send.cgi?to=" + tgt);
	});
});

const l = document.location;
const pimg = '/cgi-bin/image.cgi';
const jpg = document.getElementById("preview");

document.addEventListener('DOMContentLoaded', loaded, false);

async function loaded() {
	while (true) {
		await jpg.decode().catch(function() {
			jpg.src = pimg;
		});
		await new Promise((resolve) => setTimeout(resolve, 5000));
	}
}

$("#daynight")?.addEventListener("change", ev => {
	if (ev.target.checked) {
		$("#ispmode").checked = false;
		$("#ircut").checked = false;
		["ir850", "ir940", "white"].forEach(n => $("#" + n).checked = true)
		mode = "night";
	} else {
		$("#ispmode").checked = true;
		$("#ircut").checked = true;
		["ir850", "ir940", "white"].forEach(n => $("#" + n).checked = false)
		mode = "day";
	}
});
</script>

<style>
#controls div.buttons { background: #88888888; visibility: hidden; width: 100%; height: 100%; }
#controls:hover div.buttons { visibility: visible; }
</style>

<%in p/footer.cgi %>
