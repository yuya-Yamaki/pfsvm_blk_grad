#! /bin/sh
TRAINING=akiyo
QP=32
SAO=0
LEVEL=3
C=2.0
GAMMA=0.25
GAIN=0.125
count=1
TASK="train-"`basename $TRAINING .pgm`"-q$QP-sao$SAO-c$C-gm$GAMMA-ga$GAIN-l$LEVEL"
MODEL="./testmodel/$TASK.svm"
MODELBLK="./testmodelblk/$TASK.svm"
LOG="./testhevclog/$TASK.log"
HM=~/HM-16.9/
HMOPT="-c $HM""cfg/encoder_intra_main_rext.cfg -cf 400 -f 1 -fr 1 --InternalBitDepth=8"
HMENC="$HM""bin/TAppEncoderStatic"
#HMDEC="$HM""bin/TAppDecoderStatic"
HMDEC="$HM""bin/TAppDecoderAnalyserStatic"
DIR=~/cif_pgm/
IMG=$DIR$TRAINING.pgm
echo "Running at "`uname -a` | tee $LOG
echo "Training image is $TRAINING.pgm" | tee -a $LOG
WIDTH=`pamfile $IMG | gawk '{print $4}'`
HEIGHT=`pamfile $IMG | gawk '{print $6}'`
tail -n +4 $IMG > input.y
$HMENC $HMOPT -q $QP -wdt $WIDTH -hgt $HEIGHT --SAO=$SAO -i input.y
SIZE=`ls -l str.bin | gawk '{print $5}'`
$HMDEC -d 8 -b str.bin -o rec8bit.y
rawtopgm $WIDTH $HEIGHT rec8bit.y > decoded.pgm
SNR=`pnmpsnr $IMG decoded.pgm 2>&1 | gawk '{print $7}'`
printf "QP:SIZE:SNR %3d%10d%10.2f\n" $QP $SIZE $SNR | tee -a $LOG
./import_TUinfo $count
./pfsvm_train -C $C -G $GAMMA -L $LEVEL -S $GAIN $IMG decoded.pgm $MODEL $MODELBLK| tee -a $LOG
for IMG in `ls $DIR*.pgm`
do
	echo "-----------------------------------" | tee -a $LOG
	echo "Evaluation for "`basename $IMG` | tee -a $LOG
	tail -n +4 $IMG > input.y
	$HMENC $HMOPT -q $QP -wdt $WIDTH -hgt $HEIGHT --SAO=$SAO -i input.y
	SIZE=`ls -l str.bin | gawk '{print $5}'`
	$HMDEC -d 8 -b str.bin -o rec8bit.y
	rawtopgm $WIDTH $HEIGHT rec8bit.y > decoded.pgm
	SNR=`pnmpsnr $IMG decoded.pgm 2>&1 | gawk '{print $7}'`
	printf "QP:SIZE:SNR %3d%10d%10.2f\n" $QP $SIZE $SNR | tee -a $LOG
	./import_TUinfo $count
	./pfsvm_eval -S $GAIN $IMG decoded.pgm $MODEL $MODELBLK modifiedtesthevc.pgm | tee -a $LOG
done
