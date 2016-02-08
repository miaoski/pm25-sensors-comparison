目的
====
因為手上有很多組公認還算準的 sensors, 於是想測測看到底其間的誤差如何。
建議插上 SD 卡，以免過度寫入 log 造成 7688 上的 Flash 損壞。


安裝
====
請把 `compare.py` 複製到 `/Media/SD-P1/` 然後設定好 `thingspeak.key`，並安裝需要的套件

```
# pip install requests
```

最後在 `/etc/rc.local` 加上

```
cd /Media/SD-P1
python compare.py &
exit 0
```

腳位
====
```
A4 pin 1   -> +5V
A4 pin 2   -> GND
A4 pin 5   -> 7688 Duo pin D10
G3 pin 5   -> 7688 Duo pin D8
SHT-31 SDA -> 7688 Duo pin D2 <- AM2320 SDA
SHT-31 SCL -> 7688 Duo pin D3 <- AM2320 SCL
DHT-22 DAT -> 7688 Duo pin D4
```

Lesson Learn
============
* Linkit 7688 Duo 有一些眉角，常常沒辦法無痛上手。感恩阿海、讚嘆阿海。
* SoftwareSerial 用到兩組，記得要 call `portOne.listen()` 及 `portTwo.listen()`
* 把 `id_rsa.pub` 複製到 `/etc/dropbear/authorized_keys` 吧，省得打密碼 XD


License
=======
MIT.
