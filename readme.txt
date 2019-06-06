ブロック内部と境界部で異なるSVM分類器を構成し，かつブロック境界線の方向を統一することで再生誤差の性質を合わせたもの．
ブロックの隅において対象画素とその周辺2画素との差分をとり勾配の大きい方向に回転を加え特徴量を統一したプログラム
最適オフセット値を各クラスに属する画素の再生値に加えることによって符号化による画質の低下を抑えるプログラムのフォルダ

一つのプロジェクトを動かすのに必要なもの
ファイル
svm.h
svm.cpp
pfsvm.h
pfsvm_common.c
pfsvm_eval.c
pfsvm_train_loo.c
Makefile
loo.sh(looHD.sh,looHD_all.sh,loo_all.sh)
import_TUinfo.c
makeyuvCrCb(makeyuv)-mainCrCb.c,yuvCrCb.h,yuvCrCb.c
TUplotblk.c

フォルダ
livsvm-3.21
model
modelblk
log
dec_dir

（一つ前の場所）
avsnr
HD_pgm
cif_pgm
