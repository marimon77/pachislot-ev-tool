<!DOCTYPE html>
<html lang="ja">
<head>
<meta charset="UTF-8">
<title>転生の章2 期待値計算ツール</title>
<style>
body { font-family: system-ui, -apple-system, BlinkMacSystemFont, sans-serif; background:#0f172a; color:#fff; padding:20px; }
.card { background:#1e293b; padding:20px; border-radius:12px; max-width:480px; margin:auto; }
h1 { text-align:center; }
label { display:block; margin-top:12px; }
input, select, button { width:100%; padding:10px; margin-top:5px; border-radius:8px; border:none; font-size:16px; }
button { background:#22c55e; color:#000; font-weight:bold; margin-top:15px; }
.result { margin-top:20px; background:#020617; padding:15px; border-radius:10px; }
.good { color:#22c55e; font-weight:bold; }
.mid { color:#eab308; font-weight:bold; }
.bad { color:#ef4444; font-weight:bold; }
</style>
</head>
<body>

<div class="card">
<h1>転生の章2 期待値計算</h1>

<label>現在G数</label>
<input type="number" id="current" value="300">

<label>等価交換</label>
<select id="coin">
<option value="20">等価（20円）</option>
<option value="18">56枚交換（18円）</option>
<option value="17">非等価（17円）</option>
</select>

<label>モード示唆</label>
<select id="mode">
<option value="none">なし</option>
<option value="weak">弱</option>
<option value="strong">強</option>
</select>

<button onclick="calc()">期待値を計算</button>

<div class="result" id="result">結果がここに表示されます</div>
</div>

<script>
const ceiling = 777;
const avgPayout = 720;
const costPerGame = 50;

const modeWeight = {
  none: {A:0.6, B:0.3, T:0.1},
  weak: {A:0.3, B:0.5, T:0.2},
  strong:{A:0.1, B:0.3, T:0.6}
};

function hitRate(mode, g){
  if(mode=="T") return 0.8;
  if(g<=128) return mode=="A"?0.05:0.10;
  if(g<=256) return mode=="A"?0.10:0.20;
  if(g<=384) return mode=="A"?0.20:0.30;
  if(g<=512) return mode=="A"?0.30:0.40;
  return mode=="A"?0.35:0.50;
}

function judge(ev){
  if(ev>=3000) return "<span class='good'>◎ 超優秀</span>";
  if(ev>=1500) return "<span class='good'>◯ 狙い目</span>";
  if(ev>=500) return "<span class='mid'>△ 可</span>";
  if(ev>=0) return "<span class='mid'>様子見</span>";
  return "<span class='bad'>✕ 打たない</span>";
}

function calc(){
  const g = Number(document.getElementById("current").value);
  const coin = Number(document.getElementById("coin").value);
  const mode = document.getElementById("mode").value;

  let ev = 0;

  for(let m in modeWeight[mode]){
    let w = modeWeight[mode][m];
    let h = hitRate(m, g);
    let invest = Math.max(0, (ceiling - g) * costPerGame);
    let payout = avgPayout * coin * h;
    ev += (payout - invest) * w;
  }

  const time = (ceiling - g) / 800 * 60;
  const hourly = time > 0 ? ev / (time/60) : ev;

  document.getElementById("result").innerHTML =
    `期待値：<b>${Math.round(ev)}円</b><br>` +
    `時給：<b>${Math.round(hourly)}円</b><br>` +
    `判定：${judge(ev)}`;
}
</script>

</body>
</html>
