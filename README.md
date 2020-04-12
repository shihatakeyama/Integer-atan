整数型変数(int)を使用してatan()を計算します。
指数、掛け算、割り算 CPUの算術を使わないで実現します。

32bit変数は、数値0x80000000を1.0として扱っています。
角度はatam()同様ラジアンで返します。

FPGA組み込みなどでIPコアを使用しないで実現したい場合など、
アルゴリズムを参考にするために作成した。

atan()に与える引数は0～0.9999999997までの範囲を計算します。

VC2013の環境で作成した。