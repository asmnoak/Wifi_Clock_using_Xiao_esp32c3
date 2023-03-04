<p><H3>Seeed Studio XIAO ESP32C3でWifiによる時刻設定が可能な時計の製作</H3></p>
<p><a title="XIAO_ESP32C3" href="https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/">Seeed Studio XIAO ESP32C3</a>はESP32互換でWifiが利用できるMCU（マイクロコントローラ）である。外形はSeeed Studio XIAOと同じ大きさ、超小型である。
<p>NTPから時刻と日付を参照し、XIAOのRTC（リアルタイムクロック）に設定する時計を製作した。</p>

<p>時刻関係のライブラリ（Timelib.h）は<a title="timelib" href="https://github.com/PaulStoffregen/Time">こちら</a>。表示は定番のSSD1306のOLED、ライブラリはライブラリ管理から検索可能（Adafruit版）。</p>
<p>Wifiは、２つのWifiステーションのうち、電波の強い方を自動で選択できるように、WiFiMulti.hライブリを利用した（一つしかない場合は一方を適当に指定すればよい）。ssidとpasswordをアクセスするWifiステーションのものに変更する。</p>
<p>I2Cの接続はデフォールト（SDA：４、SCK：５）でよい。</p>
<p>XIAO ESP32C3にはSleep機能があるので、時刻の更新時以外はスリープさせると、使用電力（十数mA）を節約できる。ただし、スリープさせるとArudionoIDEからスケッチの書き込みができなくなるので要注意である。スリープを有効にするのは、最後の書き込み時のみにする必要がある。</br>なお、スリープを有効にしてしまった後は以下の手順で復旧できる。これは、スケッチの誤りで、XIAO ESP32C3がリセットを繰り返すような場合にも利用できるので覚えておく価値があると思う。</p>
<ol>
<li>USBを抜く</li>
<li>BOOTボタンを押したままUSBを挿す、押したまま</li>
<li>IDEからスケッチを書き込む</li>
<li>書き込みが完了したら、BOOTボタンを離す</li>
<li>RESETボタンを押す</li>
</ol>
<p>帆のように見えるのはWifiのアンテナである。XIAOとはI-PEXコネクタで接続する。右側の基板はアンテナの台として利用、XIAOには未接続である。</p>
<p><img src="https://github.com/asmnoak/Wifi_Clock_using_Xiao_esp32c3/blob/main/xiao_esp32c3_clock.JPG" /></p>
<p>&nbsp;</p>
