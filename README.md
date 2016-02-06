前言
====
因為手上有很多組公認還算準的 sensors, 於是想測測看到底其間的誤差如何。

建議插上 SD 卡，以免過度寫入 log 造成 7688 上的 Flash 損壞。


Lesson Learn
============
* Linkit 7688 Duo 有一些眉角，常常沒辦法無痛上手。感恩阿海、讚嘆阿海。
* SoftwareSerial 用到兩組，記得要 call `portOne.listen()` 及 `portTwo.listen()`


License
=======
MIT.
