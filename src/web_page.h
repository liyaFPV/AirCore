#pragma once
#include <Arduino.h>

static const char PAGE_INDEX[] PROGMEM = R"HTMLPAGE(<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>AirCore</title>
<style>
:root{
  --bg:#0b1120; --card:#111c31; --ink:#e2e8f0; --muted:#94a3b8;
  --line:#243248; --field:#0f172a; --brand:#6366f1; --brand2:#22d3ee;
  --ok:#22c55e; --err:#ef4444; --shadow:0 10px 30px rgba(0,0,0,.45);
}
body.light{
  --bg:#eef2f7; --card:#ffffff; --ink:#111827; --muted:#6b7280;
  --line:#e5e7eb; --field:#f9fafb; --brand:#4f46e5; --brand2:#0ea5e9;
  --shadow:0 10px 30px rgba(17,24,39,.08);
}
*{box-sizing:border-box}
body{margin:0;font-family:system-ui,-apple-system,"Segoe UI",Roboto,Ubuntu,sans-serif;
  background:var(--bg);color:var(--ink);-webkit-font-smoothing:antialiased;transition:background .2s,color .2s}
.wrap{max-width:620px;margin:0 auto;padding:18px 16px 40px}
header{display:flex;align-items:center;gap:14px;margin:10px 0 22px}
.logo{width:52px;height:52px;border-radius:16px;flex:0 0 auto;
  background:linear-gradient(135deg,var(--brand),var(--brand2));display:flex;align-items:center;
  justify-content:center;box-shadow:var(--shadow)}
.logo svg{width:28px;height:28px}
.title{font-size:24px;font-weight:700;line-height:1.1}
.subtitle{font-size:13px;color:var(--muted)}
.ctrl{margin-left:auto;display:flex;gap:8px;align-items:center}
.lang{display:flex;border:1px solid var(--line);border-radius:10px;overflow:hidden;background:var(--card)}
.lang button{border:0;background:transparent;padding:7px 12px;font-size:13px;font-weight:600;color:var(--muted);cursor:pointer}
.lang button.active{background:linear-gradient(135deg,var(--brand),var(--brand2));color:#fff}
.iconbtn{width:40px;height:40px;border:1px solid var(--line);background:var(--card);color:var(--ink);
  border-radius:10px;font-size:18px;cursor:pointer;line-height:1}
.card{background:var(--card);border:1px solid var(--line);border-radius:18px;box-shadow:var(--shadow);padding:18px;margin-bottom:16px}
.card h2{margin:0 0 14px;font-size:16px;display:flex;align-items:center;gap:8px}
.card h2 .ic{font-size:18px}
label{display:block;font-size:13px;font-weight:600;color:var(--muted);margin:12px 0 6px}
input[type=text],input[type=password]{width:100%;padding:12px 14px;border:1px solid var(--line);
  border-radius:12px;font-size:15px;background:var(--field);color:var(--ink);outline:none;transition:.15s}
input::placeholder{color:var(--muted);opacity:.7}
input:focus{border-color:var(--brand);background:var(--card);box-shadow:0 0 0 3px rgba(99,102,241,.22)}
.btn{border:0;border-radius:12px;padding:12px 16px;font-size:15px;font-weight:600;cursor:pointer;transition:.15s}
.btn.primary{background:linear-gradient(135deg,var(--brand),var(--brand2));color:#fff;box-shadow:var(--shadow)}
.btn.primary:active{transform:translateY(1px)}
.btn.ghost{background:var(--field);color:var(--ink);border:1px solid var(--line)}
.btn.danger{background:transparent;color:var(--err);border:1px solid var(--err)}
.btn.block{width:100%}
.btn[disabled]{opacity:.6;cursor:default}
.actions{display:flex;gap:10px;margin-top:4px}
.actions .primary{flex:2}
.actions .danger{flex:1}
.tabs{display:flex;background:var(--field);border:1px solid var(--line);border-radius:12px;padding:4px;margin-bottom:6px}
.tabs button{flex:1;border:0;background:transparent;padding:10px;border-radius:9px;font-size:14px;font-weight:600;color:var(--muted);cursor:pointer}
.tabs button.active{background:var(--card);color:var(--brand);box-shadow:0 2px 8px rgba(0,0,0,.25)}
body.light .tabs button.active{box-shadow:0 2px 8px rgba(17,24,39,.08)}
.scan-head{display:flex;align-items:center;justify-content:space-between;gap:10px}
.nets{margin-top:10px;max-height:230px;overflow:auto;border:1px solid var(--line);border-radius:12px;display:none;background:var(--field)}
.nets.show{display:block}
.net{display:flex;align-items:center;gap:10px;padding:11px 13px;border-bottom:1px solid var(--line);cursor:pointer}
.net:last-child{border-bottom:0}
.net:hover{background:var(--card)}
.net .n{flex:1;font-size:14px;font-weight:600;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.net .s{font-size:12px;color:var(--muted)}
.net .lock{font-size:12px}
.hint{font-size:12px;color:var(--muted);margin-top:8px;line-height:1.45}
.searchbox{position:relative}
.sug{position:absolute;left:0;right:0;top:calc(100% + 4px);z-index:20;background:var(--card);
  border:1px solid var(--line);border-radius:12px;box-shadow:var(--shadow);max-height:250px;overflow:auto;display:none}
.sug.show{display:block}
.sug .it{padding:10px 13px;border-bottom:1px solid var(--line);cursor:pointer}
.sug .it:last-child{border-bottom:0}
.sug .it:hover{background:var(--field)}
.sug .it .c{font-size:14px;font-weight:600}
.sug .it .r{font-size:12px;color:var(--muted)}
.resolved{margin-top:10px;font-size:13px;color:var(--ok);display:none;align-items:center;gap:6px}
.resolved.show{display:flex}
.hidden{display:none!important}
.toast{position:fixed;left:50%;bottom:24px;transform:translateX(-50%) translateY(20px);
  background:var(--card);color:var(--ink);border:1px solid var(--line);padding:12px 18px;border-radius:12px;font-size:14px;
  opacity:0;pointer-events:none;transition:.25s;max-width:90%;text-align:center;box-shadow:var(--shadow)}
.toast.show{opacity:1;transform:translateX(-50%) translateY(0)}
.toast.err{color:#fff;background:var(--err);border-color:transparent}
.toast.ok{color:#04220f;background:var(--ok);border-color:transparent}
.spin{width:14px;height:14px;border:2px solid rgba(148,163,184,.5);border-top-color:var(--ink);border-radius:50%;
  display:inline-block;vertical-align:-2px;margin-right:6px;animation:sp .7s linear infinite}
@keyframes sp{to{transform:rotate(360deg)}}
footer{text-align:center;font-size:12px;color:var(--muted);margin-top:8px}
</style>
</head>
<body>
<div class="wrap">
  <header>
    <div class="logo">
      <svg viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
        <path d="M5 12.5a9 9 0 0 1 14 0"/><path d="M8 15.5a5 5 0 0 1 8 0"/>
        <circle cx="12" cy="19" r="1.6" fill="#fff" stroke="none"/><path d="M2 9.5a13 13 0 0 1 20 0"/>
      </svg>
    </div>
    <div>
      <div class="title">AirCore</div>
      <div class="subtitle" id="sub">Настройка</div>
    </div>
    <div class="ctrl">
      <div class="lang">
        <button id="btn-ru" onclick="setLang('ru')">RU</button>
        <button id="btn-en" onclick="setLang('en')">EN</button>
      </div>
      <button class="iconbtn" id="themeBtn" onclick="toggleTheme()">&#9788;</button>
    </div>
  </header>

  <div class="card">
    <h2><span class="ic">&#128246;</span><span data-i18n="wifi">Wi-Fi</span></h2>
    <div class="scan-head">
      <span class="hint" data-i18n="scanHint">Выберите сеть из списка или введите вручную</span>
      <button class="btn ghost" id="scanBtn" onclick="scan()" data-i18n="scan">Сканировать</button>
    </div>
    <div class="nets" id="nets"></div>
    <label data-i18n="ssid">Имя сети (SSID)</label>
    <input type="text" id="ssid" autocomplete="off" placeholder="MyWiFi">
    <label data-i18n="pass">Пароль</label>
    <input type="password" id="pass" autocomplete="off" placeholder="********">
  </div>

  <div class="card">
    <h2><span class="ic">&#128205;</span><span data-i18n="location">Локация и время</span></h2>
    <div class="tabs">
      <button id="tab-auto" onclick="setMode('auto')" data-i18n="auto">Авто</button>
      <button id="tab-city" onclick="setMode('city')" data-i18n="city">Город</button>
    </div>
    <div id="autoBox">
      <button class="btn primary block" id="detectBtn" onclick="detect()" data-i18n="detect">Определить автоматически</button>
      <div class="hint" data-i18n="detectHint">Определяется в браузере: геолокация и часовой пояс телефона.</div>
    </div>
    <div id="cityBox" class="hidden">
      <label data-i18n="cityLabel">Город</label>
      <div class="searchbox">
        <input type="text" id="city" autocomplete="off" placeholder="Москва" oninput="onCityInput()" onfocus="onCityInput()">
        <div class="sug" id="sug"></div>
      </div>
      <div class="hint" data-i18n="cityHint">Начните вводить название — появятся совпадения, выберите нужное.</div>
    </div>
    <div class="resolved" id="resolved"><span>&#10003;</span><span id="resolvedText"></span></div>
  </div>

  <div class="actions">
    <button class="btn primary" id="saveBtn" onclick="save()" data-i18n="save">Сохранить и перезагрузить</button>
    <button class="btn danger" onclick="resetCfg()" data-i18n="reset">Сброс</button>
  </div>
  <footer>AirCore &middot; __FWVER__</footer>
</div>
<div class="toast" id="toast"></div>

<script>
var I18N = {
  ru:{wifi:"Wi-Fi",scanHint:"Выберите сеть из списка или введите вручную",scan:"Сканировать",
      ssid:"Имя сети (SSID)",pass:"Пароль",location:"Локация и время",auto:"Авто",city:"Город",
      detect:"Определить автоматически",detectHint:"Определяется в браузере: геолокация и часовой пояс телефона.",
      cityLabel:"Город",cityHint:"Начните вводить название — появятся совпадения, выберите нужное.",
      save:"Сохранить и перезагрузить",reset:"Сброс",saved:"Настройки сохранены. Перезагрузка...",
      scanErr:"Не удалось получить список сетей",detecting:"Определение...",detected:"Локация определена",
      geoErr:"Не удалось определить автоматически",confirm:"Сбросить настройки Wi-Fi и локации?",
      noSsid:"Введите имя сети",searching:"Поиск...",notFound:"Город не найден, попробуйте иначе",
      needLoc:"Укажите город или нажмите «Определить автоматически»",pickCity:"Выберите город из списка",
      resolved:"Локация:",setup:"Настройка"},
  en:{wifi:"Wi-Fi",scanHint:"Pick a network from the list or enter manually",scan:"Scan",
      ssid:"Network name (SSID)",pass:"Password",location:"Location & time",auto:"Auto",city:"City",
      detect:"Detect automatically",detectHint:"Detected in the browser: phone geolocation and timezone.",
      cityLabel:"City",cityHint:"Start typing a name — pick the right match from the list.",
      save:"Save & reboot",reset:"Reset",saved:"Saved. Rebooting...",
      scanErr:"Failed to get network list",detecting:"Detecting...",detected:"Location set",
      geoErr:"Auto detection failed",confirm:"Reset Wi-Fi and location settings?",
      noSsid:"Enter network name",searching:"Searching...",notFound:"City not found, try another name",
      needLoc:"Enter a city or tap Detect automatically",pickCity:"Pick a city from the list",
      resolved:"Location:",setup:"Setup"}
};
var lang = localStorage.getItem('lang') || 'ru';
var theme = localStorage.getItem('theme') || 'dark';
var mode = 'city';
var cur = {lat:null, lon:null, tz_min:null, city:''};

function t(k){ return (I18N[lang] && I18N[lang][k]) || k; }
function setLang(l){
  lang=l; localStorage.setItem('lang',l); document.documentElement.lang=l;
  document.querySelectorAll('[data-i18n]').forEach(function(el){ el.textContent=t(el.getAttribute('data-i18n')); });
  document.getElementById('btn-ru').classList.toggle('active',l==='ru');
  document.getElementById('btn-en').classList.toggle('active',l==='en');
  document.getElementById('sub').textContent=t('setup');
  document.getElementById('city').placeholder = (l==='ru')?'Москва':'Berlin';
  updateResolved();
}
function applyTheme(){
  document.body.classList.toggle('light', theme==='light');
  document.getElementById('themeBtn').innerHTML = (theme==='light') ? '&#9790;' : '&#9788;';
}
function toggleTheme(){
  theme = (theme==='light') ? 'dark' : 'light';
  localStorage.setItem('theme', theme); applyTheme();
}
function toast(msg,cls){
  var el=document.getElementById('toast');
  el.textContent=msg; el.className='toast show '+(cls||'');
  clearTimeout(el._t); el._t=setTimeout(function(){ el.className='toast '+(cls||''); },3000);
}
function setMode(m){
  mode=m;
  document.getElementById('tab-auto').classList.toggle('active',m==='auto');
  document.getElementById('tab-city').classList.toggle('active',m==='city');
  document.getElementById('autoBox').classList.toggle('hidden',m!=='auto');
  document.getElementById('cityBox').classList.toggle('hidden',m!=='city');
}
function strength(rssi){ if(rssi>=-60) return '\u2582\u2584\u2586\u2588'; if(rssi>=-75) return '\u2582\u2584\u2586'; if(rssi>=-85) return '\u2582\u2584'; return '\u2582'; }
function scan(){
  var btn=document.getElementById('scanBtn');
  btn.disabled=true; btn.innerHTML='<span class="spin"></span>'+t('scan');
  fetch('/scan').then(function(r){return r.json();}).then(function(list){
    var box=document.getElementById('nets'); box.innerHTML='';
    if(!list.length){ toast(t('scanErr'),'err'); }
    list.sort(function(a,b){return b.rssi-a.rssi;});
    list.forEach(function(n){
      var d=document.createElement('div'); d.className='net';
      d.innerHTML='<span class="lock">'+(n.secure?'\uD83D\uDD12':'\uD83D\uDD13')+'</span>'+
                  '<span class="n">'+n.ssid.replace(/</g,'&lt;')+'</span>'+
                  '<span class="s">'+strength(n.rssi)+'</span>';
      d.onclick=function(){ document.getElementById('ssid').value=n.ssid; box.classList.remove('show'); document.getElementById('pass').focus(); };
      box.appendChild(d);
    });
    box.classList.add('show');
  }).catch(function(){ toast(t('scanErr'),'err'); })
    .finally(function(){ btn.disabled=false; btn.textContent=t('scan'); });
}
function updateResolved(){
  var el=document.getElementById('resolved');
  if(cur.lat===null){ el.classList.remove('show'); return; }
  var tzH=(cur.tz_min/60);
  var tzS='UTC'+(tzH>=0?'+':'')+ (Math.round(tzH*10)/10);
  var s=cur.city? cur.city+' \u00b7 '+tzS : tzS;
  document.getElementById('resolvedText').textContent=t('resolved')+' '+s;
  el.classList.add('show');
}
function pickCity(lat,lon,tzmin,name){
  cur.lat=lat; cur.lon=lon; cur.tz_min=tzmin; cur.city=name;
  document.getElementById('city').value=name;
  document.getElementById('sug').classList.remove('show');
  updateResolved(); toast(t('detected'),'ok');
}
var sugTimer=null, lastGeo=0;
function onCityInput(){
  var q=document.getElementById('city').value.trim();
  if(q.length<2){ document.getElementById('sug').classList.remove('show'); return; }
  clearTimeout(sugTimer);
  sugTimer=setTimeout(function(){ geocode(q); },350);
}
function renderSug(items){
  var box=document.getElementById('sug');
  if(!items.length){ box.classList.remove('show'); toast(t('notFound'),'err'); return; }
  box.innerHTML='';
  items.forEach(function(it){
    var d=document.createElement('div'); d.className='it';
    d.innerHTML='<div class="c">'+it.name.replace(/</g,'&lt;')+'</div>'+
                '<div class="r">'+(it.region?it.region.replace(/</g,'&lt;')+', ':'')+it.country.replace(/</g,'&lt;')+'</div>';
    d.onclick=function(){ pickCity(it.lat,it.lon,it.tz_min,it.name); };
    box.appendChild(d);
  });
  box.classList.add('show');
}
function geocode(q){
  fetch('https://geocoding-api.open-meteo.com/v1/search?name='+encodeURIComponent(q)+'&count=5&language='+lang+'&format=json')
    .then(function(r){return r.json();})
    .then(function(d){
      if(d && d.results && d.results.length){
        var items=d.results.map(function(x){
          return {name:x.name, region:x.admin1||'', country:x.country||'', lat:x.latitude, lon:x.longitude,
                  tz_min: (x.utc_offset_seconds!==undefined? Math.round(x.utc_offset_seconds/60): null)};
        });
        renderSug(items);
      } else { geocodeNom(q); }
    }).catch(function(){ geocodeNom(q); });
}
function geocodeNom(q){
  fetch('https://nominatim.openstreetmap.org/search?format=json&limit=5&accept-language='+lang+'&q='+encodeURIComponent(q))
    .then(function(r){return r.json();})
    .then(function(d){
      if(!d || !d.length){ toast(t('notFound'),'err'); return; }
      var tzmin=-new Date().getTimezoneOffset();
      var items=d.map(function(x){
        var p=x.display_name.split(',');
        return {name:p[0].trim(), region:(p.length>2?p[1].trim():''), country:(p.length?p[p.length-1].trim():''),
                lat:parseFloat(x.lat), lon:parseFloat(x.lon), tz_min:tzmin};
      });
      renderSug(items);
    }).catch(function(){ toast(t('notFound'),'err'); });
}
function detect(){
  var btn=document.getElementById('detectBtn');
  btn.disabled=true; btn.innerHTML='<span class="spin"></span>'+t('detecting');
  var tzmin=-new Date().getTimezoneOffset();
  var reset=function(){ btn.disabled=false; btn.textContent=t('detect'); };
  var done=function(la,lo,name){ cur.lat=la; cur.lon=lo; cur.tz_min=tzmin; cur.city=name||''; updateResolved(); toast(t('detected'),'ok'); };
  if(navigator.geolocation){
    navigator.geolocation.getCurrentPosition(
      function(p){ done(p.coords.latitude,p.coords.longitude,''); reset(); },
      function(){ ipFallback(done); reset(); },
      {enableHighAccuracy:false,timeout:8000,maximumAge:600000});
  } else { ipFallback(done); reset(); }
}
function ipFallback(done){
  fetch('https://ipwho.is/').then(function(r){return r.json();}).then(function(d){
    if(d && d.success){ done(d.latitude,d.longitude,d.city||''); }
    else toast(t('geoErr'),'err');
  }).catch(function(){ toast(t('geoErr'),'err'); });
}
function save(){
  var ssid=document.getElementById('ssid').value.trim();
  if(!ssid){ toast(t('noSsid'),'err'); return; }
  if(cur.lat===null){ toast(t('needLoc'),'err'); return; }
  var params=new URLSearchParams({
    ssid:ssid, pass:document.getElementById('pass').value, loc_mode:mode,
    lat:cur.lat, lon:cur.lon, tz_min:cur.tz_min, city:cur.city
  });
  var btn=document.getElementById('saveBtn'); btn.disabled=true;
  fetch('/save',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:params})
    .then(function(r){return r.json();}).then(function(){ toast(t('saved'),'ok'); })
    .catch(function(){ toast(t('saved'),'ok'); });
}
function resetCfg(){
  if(!confirm(t('confirm'))) return;
  fetch('/reset',{method:'POST'}).then(function(){ toast(t('saved'),'ok'); }).catch(function(){});
}
document.addEventListener('click',function(e){
  if(!e.target.closest('.searchbox')) document.getElementById('sug').classList.remove('show');
});
applyTheme(); setLang(lang); setMode('city');
try{ fetch('/location').then(function(r){return r.json();}).then(function(d){
  if(d && d.valid){
    document.getElementById('ssid').value=d.ssid||'';
    cur.lat=d.lat; cur.lon=d.lon; cur.tz_min=d.tz_min; cur.city=d.city||'';
    if(cur.city) document.getElementById('city').value=cur.city;
    setMode(d.loc_mode==='auto'?'auto':'city'); updateResolved();
  }
}).catch(function(){}); }catch(e){}
</script>
</body>
</html>
)HTMLPAGE";
