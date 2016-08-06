#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "ArmControl.h"
#include "DEBUG.h"
//#include "stdafx.h"






int ArmPC::Arm_Move(float Target_X, float Target_Y, float Target_C)
{ 
	//Serial.println(millis());

	float A, B, C, D;  //杩愮畻鐨勪腑闂撮噺锛屾病鍟ユ剰涔�
	float AngleTemp;   //瑙掑害杩愮畻涓棿閲�
	float TargetX;     //鍑芥暟鍐呬娇鐢ㄧ殑鐗╀綋鐩爣鍧愭爣 
	float TargetY;
	float TargetC;
	float TargetLength;//涓昏噦涓濇潌铻烘瘝鎴栧壇鑷備笣鏉嗚灪姣嶉渶瑕佺Щ鍔ㄥ埌鐨勭粷瀵逛綅缃�
	int OverFlag;      //杩斿洖鐨勯敊璇俊鎭紝-2涓烘湭鐭ラ敊璇紱-1鏈垵濮嬪寲锛�0-姝ｅ父锛�1-闇�瑕佸簳搴х數鏈鸿緟鍔╋紱2-y瓒婄晫锛�3-y瓒婄晫涓旈渶瑕佸簳搴х數鏈鸿緟鍔�
	
	if (InitFlag == 0) //鑻ユ湭鍒濆鍖栵紝鍒欏厛鍒濆鍖�
	{
		return -1;     //杩斿洖鏈垵濮嬪寲鐨勯敊璇俊鎭�
	}
	TargetX = Target_X;//璧嬪�煎埌鍑芥暟鍐咃紝鏂逛究璋冪敤鍑芥暟
	TargetY = Target_Y;
	TargetC = Target_C;
	while (1)
	{
		if (TargetC < ArmP.AngleBaseMin)
			TargetC += PI * 2;
		else if (TargetC > ArmP.AngleBaseMax)
			TargetC -= PI * 2;
		else
			break;
		Serial.println(TargetC);
	}
	

	OverFlag = Base_Assist(&TargetX, &TargetY, &TargetC);//棰勫鐞�1銆佸緱鍒板簳搴х數鏈鸿緟鍔╄繍鍔ㄩ噺 2銆佽繑鍥炶秺鐣屼俊鎭� 3銆佸皢鐩爣鍧愭爣淇鍒版満姊拌噦鍙互鍒拌揪鐨勫尯鍩�
	
	//瑙ｇ畻涓濇潌闇�瑕佺Щ鍔ㄧ殑璺濈
	//閫氳繃瑙ｆ柟绋婰engthArm*(cos(a1)+cos(a2))=TargetX; 
	//          LengthArm*(sin(a1)+sin(a2))=TargetY;
	//          a1:AngleArmMain
	//          a2:AngleArmDeputy
	  A = TargetX * TargetX + TargetY * TargetY - 2 * ArmP.LengthArm * ArmP.LengthArm;
	  B = 2 * ArmP.LengthArm * ArmP.LengthArm;
	  if (fabs(A / B) > 1)
		  ArmS.ArmAngleMD = 0;    //瑙掑害鍙兘涓洪浂锛屽鏄撶畻鍑烘棤瑙�
	  else
	      ArmS.ArmAngleMD = acos(A/ B);     //寰楀埌涓昏噦鍓噦涔嬮棿澶硅

	  A = TargetX * (cos(ArmS.ArmAngleMD) + 1);
	  B = TargetY * sin(ArmS.ArmAngleMD);
	  C = ArmP.LengthArm * (2 + 2 * cos(ArmS.ArmAngleMD));
	ArmS.ArmAngleMain = acos((A-B)/C) ;//寰楀埌涓昏噦涓庢按骞抽潰澶硅

	ArmS.ArmAngleDeputy = ArmS.ArmAngleMain - ArmS.ArmAngleMD;  //寰楀埌鍓噦涓庢按骞抽潰澶硅                                                                    

	  AngleTemp = PI - ArmS.ArmAngleMain - ArmP.AngleMainHand;
	  A = ArmP.LengthMainHand * cos(AngleTemp);
	  B = ArmP.LengthMainConnect * ArmP.LengthMainConnect - ArmP.LengthMainHand * ArmP.LengthMainHand * sin(AngleTemp) * sin(AngleTemp);
	  TargetLength = A + sqrt(B);
	ArmM.LengthMoveMain = TargetLength - LengthMain;//寰楀埌涓昏噦涓濇潌铻烘瘝闇�瑕佺Щ鍔ㄧ殑浣嶇Щ
	LengthMain = TargetLength;//寰楀埌鏈�鏂扮殑涓昏噦涓濇潌铻烘瘝缁濆浣嶇疆

	  AngleTemp = PI - ArmS.ArmAngleMD - ArmP.AngleDeputyHand;
	  A = ArmP.LengthDeputyHand * cos(AngleTemp);
	  B = ArmP.LengthDeputyConnect * ArmP.LengthDeputyConnect - ArmP.LengthDeputyHand * ArmP.LengthDeputyHand * sin(AngleTemp) * sin(AngleTemp);
	  TargetLength = A + sqrt(B);
	ArmM.LengthMoveDeputy = TargetLength - LengthDeputy;//寰楀埌鍓噦涓濇潌铻烘瘝闇�瑕佺Щ鍔ㄧ殑璺濈
	LengthDeputy = TargetLength;//寰楀埌鏈�鏂扮殑鍓噦涓濇潌铻烘瘝浣嶇疆
	
	ArmM.TurnC = TargetC - ArmS.ArmC;
	ArmS.ArmTipX = TargetX;
	ArmS.ArmTipY = TargetY;
	ArmS.ArmC    = TargetC;

	//Serial.println(millis());
	//Serial.println("CacultionOver");
	//Serial.println(ArmM.LengthMoveMain);
	//Serial.println(ArmM.LengthMoveDeputy);
	//Serial.println(ArmM.TurnC);
	OverFlag+=ArmU.Arm_Motion(ArmM.LengthMoveMain, ArmM.LengthMoveDeputy, ArmM.TurnC);
	return OverFlag;


}

void ArmPC::Move_Forward(float Target_X, float Target_Y, float Target_C)
{
	int flag;
	float TargetX;     //鍑芥暟鍐呬娇鐢ㄧ殑鐗╀綋鐩爣鍧愭爣 
	float TargetY;
	float TargetC;
	float Distance;//鐖皷閲岀洰鏍囩殑璺濈
	TargetX = Target_X;//璧嬪�煎埌鍑芥暟鍐咃紝鏂逛究璋冪敤鍑芥暟
	TargetY = Target_Y;
	TargetC = Target_C;
//	flag = Arm_Move(0, TargetY, TargetC);//鎵惧埌鏈�杩戠殑婊¤冻Y鐨剎;
	//flag = Arm_Move(TargetX, TargetY, TargetC);//鎵惧埌鏈�杩戠殑婊¤冻Y鐨剎;
  //Serial.println("fuck");
	DEB("TargetX ");
	DEB(TargetX);
	DEB("TargetY ");
	DEB(TargetY);
	DEB("TargetC ");
	DEB(TargetC);
	DEB("TipX");
	DEB(ArmS.ArmTipX);
	DEB("TipY");
	DEB(ArmS.ArmTipY);
	DEB("ArmM.LengthMoveMain");
	DEB(ArmM.LengthMoveMain);
	DEB("ArmM.LengthMoveDeputy");
	DEB(ArmM.LengthMoveDeputy);
   do
	{
		Distance = TargetX - ArmS.ArmTipX;
		DEB("Distance ");
		DEB(Distance);
		DEB("TipX");
		DEB(ArmS.ArmTipX);
		DEB("TipY");
		DEB(ArmS.ArmTipY);
		DEB("ArmM.LengthMoveMain");
		DEB(ArmM.LengthMoveMain);
		DEB("ArmM.LengthMoveDeputy");
		DEB(ArmM.LengthMoveDeputy);
		DEB("\n");
		if (Distance >= 10)
		{   

			flag = Arm_Move(ArmS.ArmTipX + 10, TargetY, TargetC);//涓�娆″墠杩�10mm;
		}
		else if(Distance <= -5)
			flag = Arm_Move(ArmS.ArmTipX - 10, TargetY, TargetC);//涓�娆￠��10mm;

		else
		{
			flag = Arm_Move(TargetX, TargetY, TargetC);//鑻ョ洰鏍囩鑷傚皷宸蹭笉瓒呰繃10mm,鍒欎竴姝ュ埌浣�;
			break;
		}
   } while (ArmM.LengthMoveMain != 0 || ArmM.LengthMoveDeputy != 0 || ArmM.TurnC != 0);
	}



void ArmPC::Init_Arm(int firsttime)
{


    LengthMain = 201.4;
	LengthDeputy = 151;
	ArmM.LengthMoveDeputy = 130;
	ArmM.LengthMoveMain = 85;
	ArmM.TurnC = -2 * PI;
	ArmS.ArmC = ArmP.AngleBaseMin;//鍧愭爣绯荤殑鍘熷洜
	ArmS.ArmTipX = 206.88;
	ArmS.ArmTipY = 267.15;
	BPM.BasePlateMoveA = 0;
	BPM.BasePlateMoveB = 0;
	
	if (firsttime == 1)
	{ 
		ArmU.DafaultSpeedMain = DAFAULT_ARM_SPEED / 2;
		ArmM.LengthMoveDeputy = 30;
		ArmM.LengthMoveMain = 30;
		ArmM.TurnC = 0;
		ArmU.Init(ArmM.LengthMoveMain, ArmM.LengthMoveDeputy, ArmM.TurnC);
		
		ArmU.DafaultSpeedMain = DAFAULT_ARM_SPEED ;
		ArmM.LengthMoveDeputy = 100;
		ArmM.LengthMoveMain = 55;
		ArmM.TurnC = -2*PI;
		ArmU.Init(ArmM.LengthMoveMain, ArmM.LengthMoveDeputy, ArmM.TurnC);
	}
	else
	{
		ArmU.DafaultSpeedMain = DAFAULT_ARM_SPEED;
		ArmU.Init(ArmM.LengthMoveMain, ArmM.LengthMoveDeputy, ArmM.TurnC);
	}
	ArmU.DafaultSpeedMain = DAFAULT_ARM_SPEED;
	InitFlag = 1;

}

void ArmPC::Arm_Wait()
{
	//Serial.println("backing");
	int i;
	//Serial.println("movingtoinit");
	i = Arm_Move(206.88, 267.15,ArmS.ArmC);//鍥炲幓

	i= ArmU.Arm_Motion(15, 15, 0);//閫�鍒颁笣鏉嗚灪姣嶇殑鏋侀檺浣嶇疆

	ArmS.ArmTipX = 206.88;
	ArmS.ArmTipY = 267.15;
	LengthMain = 201.4;
	LengthDeputy = 151;

}

ArmPC::ArmPC()
{

	ArmP.LengthMainHand = 66;          //涓昏噦鏂滄敮鍑哄幓閮ㄥ垎鐨勯暱搴�
	ArmP.AngleMainHand = 0.7155850;    //涓昏噦鏂滄敮鍑哄幓閮ㄥ垎涓庝富鑷傞棿鐨勮搴︼紝寮у害鍊肩殑41搴�
	ArmP.LengthMainConnect = 156;      //涓昏噦浼犲姩杩炴潌鐨勭瓑鏁堢洿绾块暱搴�
	ArmP.LengthDeputyHand = 62.5;      //鍓噦鏂滄敮鍑哄幓閮ㄥ垎鐨勯暱搴�
	ArmP.AngleDeputyHand = 0.8726646;  //鍓噦鏂滄敮鍑哄幓閮ㄥ垎涓庡壇鑷傞棿鐨勮搴︼紝寮у害鍊肩殑50搴�
	ArmP.LengthDeputyConnect = 106.07; //鍓噦浼犲姩杩炴潌鐨勭瓑鏁堢洿绾块暱搴�
	ArmP.LengthArm = 250;              //涓昏噦鍜屽壇鑷傜殑闀垮害
	ArmP.AngleMainMin = 0.5149;          //涓昏噦涓庢按骞抽潰澶硅鐨勬渶灏忓�� 
	ArmP.AngleMainMax = 100.5/180*PI;          //鍚岀悊 
	ArmP.AngleMDMin = 0;                 //涓昏噦寤堕暱绾夸笌鍓噦澶硅鏈�灏忓�� 
	ArmP.AngleMDMax = 94.5/180*PI;            //鍚岀悊 
	ArmP.AngleBaseMax = PI*1.45;
	ArmP.AngleBaseMin = -PI / 2;

	InitFlag = 0;
}

int ArmPC::Base_Assist(float *Target_X, float *Target_Y, float *Target_C)
{
	float TargetX, TargetY, TargetC;
	
	TargetX = *Target_X;//璧嬪�煎埌鍑芥暟鍐咃紝鏂逛究璋冪敤鍑芥暟
	TargetY = *Target_Y;
	TargetC = *Target_C;
	
	int OverLimitFlag=0;//0-鏈秺鐣岋紱1-x瓒婄晫锛�2-y瓒婄晫锛�3xy鍧囪秺鐣�
	int i;//寰幆鍙橀噺 
	float BaseMovetoC = 0; //搴曞骇鐢垫満鏈滳鏂瑰悜杈呭姪杩愬姩閲� 
	float MotionTemp;    //璁＄畻涓殑涓棿鍙橀噺 
	struct Boundary  //瀹氫箟涓�涓壇鑷傛湯绔彲鍒拌寖鍥磋竟鐣岀殑绫�
					 //鑷鐢诲嚭鍓噦灏栧彲浠ュ埌杈剧殑鑼冨洿
	{
		float CenterX; //杈圭晫鍦嗗績
		float CenterY;
		float Radius;  //杈圭晫鍗婂緞骞虫柟
		float Ymax;    //杈圭晫Y鑼冨洿
		float Ymin;
		float Distance; //鐩爣鐐瑰埌鍦嗗績鐨勮窛绂诲钩鏂� 
	}B[4];

	B[0].CenterX = ArmP.LengthArm*cos(ArmP.AngleMainMax);
	B[0].CenterY = ArmP.LengthArm*sin(ArmP.AngleMainMax);
	B[0].Radius = ArmP.LengthArm*ArmP.LengthArm;
	if (ArmP.AngleMainMax - ArmP.AngleMDMin >= PI / 2)
		B[0].Ymax = B[0].CenterY + ArmP.LengthArm;
	else
		B[0].Ymax = B[0].CenterY + ArmP.LengthArm*sin(ArmP.AngleMainMax - ArmP.AngleMDMin);
	B[0].Ymin = B[0].CenterY + ArmP.LengthArm*sin(ArmP.AngleMainMax - ArmP.AngleMDMax);

	B[1].CenterX = 0;
	B[1].CenterY = 0;
	B[1].Radius = 2 * ArmP.LengthArm*ArmP.LengthArm*(1 - cos(PI - ArmP.AngleMDMax));
	B[1].Ymax = B[0].Ymin;
	B[1].Ymin = ArmP.LengthArm*(sin(ArmP.AngleMainMin) + sin(ArmP.AngleMainMin - ArmP.AngleMDMax));

	B[2].CenterX = ArmP.LengthArm*cos(ArmP.AngleMainMin);
	B[2].CenterY = ArmP.LengthArm*sin(ArmP.AngleMainMin);
	B[2].Radius = ArmP.LengthArm*ArmP.LengthArm;
	B[2].Ymax = B[2].CenterY + ArmP.LengthArm*sin(ArmP.AngleMainMin - ArmP.AngleMDMin);
	B[2].Ymin = B[1].Ymin;

	B[3].CenterX = 0;
	B[3].CenterY = 0;
	if (ArmP.AngleMDMin <= 0)
	{
		B[3].Radius = 4 * ArmP.LengthArm*ArmP.LengthArm;
		if (ArmP.AngleMainMax <= PI / 2)
			B[3].Ymax = 2 * ArmP.LengthArm*sin(ArmP.AngleMainMax);
		else
			B[3].Ymax = 2 * ArmP.LengthArm;
	}
	else
	{
		B[3].Radius = 2 * ArmP.LengthArm*ArmP.LengthArm*(1 - cos(PI - ArmP.AngleMDMin));
		if (ArmP.AngleMainMax - ArmP.AngleMDMin / 2 <= PI / 2)
			B[3].Ymax = sqrt(B[3].Radius)*sin(ArmP.AngleMainMax - ArmP.AngleMDMin / 2);
		else
			B[3].Ymax = sqrt(B[3].Radius);
	}
	B[3].Ymin = B[1].Ymin;

	//y鏂瑰悜瓒婄晫鍒ゆ柇 
	if (TargetY>B[3].Ymax)
	{
		TargetY = B[3].Ymax;
		OverLimitFlag = OverLimitFlag + 2;
	}
	else if (TargetY<B[1].Ymin)
	{
		TargetY = B[1].Ymin;
		OverLimitFlag = OverLimitFlag + 2;

	}

	//x鏂瑰悜瓒婄晫绮楃暐鍒ゆ柇锛岄槻姝鍊艰繃澶у湪鍚庨潰杩愮畻涓孩鍑� 

	if (TargetX > 2 * ArmP.LengthArm + 1)       //澶氬姞涓�涓�1锛屼娇鍏跺湪鍚庨潰鐨勭簿纭繍绠椾腑涓�瀹氳秺鐣岋紝濂界粰OverLimitFlag璧嬪�� 
	{
		BaseMovetoC = TargetX - 2 * ArmP.LengthArm -1 + BaseMovetoC;
		TargetX = 2 * ArmP.LengthArm + 1;

	}
	else if (TargetX < -ArmP.LengthArm)     //鍚岀悊 
	{
		BaseMovetoC = TargetX + ArmP.LengthArm + 1 + BaseMovetoC;
		TargetX = -ArmP.LengthArm;
	}

	//x鏂瑰悜瓒婄晫鍒ゆ柇锛屽苟缁欏嚭搴曞骇鐢垫満杈呭姪绉诲姩閲�
	for (i = 0; i<4; i++)
	{
		B[i].Distance = (TargetX - B[i].CenterX)*(TargetX - B[i].CenterX) + (TargetY - B[i].CenterY)*(TargetY - B[i].CenterY);
		if (B[i].Distance<B[i].Radius && TargetY <= B[i].Ymax && TargetY >= B[i].Ymin && (i == 0 || i == 1))//鍦ㄥ墠涓や釜杈圭晫鍦嗙殑鍐呴儴锛屾槸TargetX澶皬鐨勬儏鍐�
		{
			MotionTemp = sqrt(B[i].Radius - (TargetY - B[i].CenterY)*(TargetY - B[i].CenterY)) + B[i].CenterX; //姝ゅMotionTemp鐨勫�间负TargetY涓嬶紝鍓噦鏈鍒扮殑鏈�灏忕殑X銆�
			BaseMovetoC = TargetX - MotionTemp + BaseMovetoC;  //搴曞骇鐢垫満娌跨潃C鏂瑰悜绉诲姩鐨勮窛绂伙紝绉诲姩鍚庯紝鏈烘鑷傝兘浼稿埌鐩爣澶�
			TargetX = MotionTemp;  //鎺ヤ笅鍘荤殑涓濇潌铻烘瘝杩愬姩鎸夌収绠楀緱鐨勮兘婊¤冻瑕佹眰鐨勬渶灏廥杩愮畻锛�
			OverLimitFlag = OverLimitFlag + 1;
			break;
		}
		else if (B[i].Distance>B[i].Radius && TargetY <= B[i].Ymax && TargetY >= B[i].Ymin && (i == 2 || i == 3)) ////鍦ㄥ悗涓や釜杈圭晫鍦嗙殑澶栭儴锛屾槸TargetX澶ぇ鐨勬儏鍐�
		{
			MotionTemp = sqrt(B[i].Radius - (TargetY - B[i].CenterY)*(TargetY - B[i].CenterY)) + B[i].CenterX; //姝ゅMotionTemp鐨勫�间负TargetY涓嬶紝鍓噦鏈鍒扮殑鏈�澶х殑X銆�
			BaseMovetoC = TargetX - MotionTemp + BaseMovetoC;  //搴曞骇鐢垫満娌跨潃C鏂瑰悜绉诲姩鐨勮窛绂伙紝绉诲姩鍚庯紝鏈烘鑷傝兘浼稿埌鐩爣澶�
			TargetX = MotionTemp;  //鎺ヤ笅鍘荤殑涓濇潌铻烘瘝杩愬姩鎸夌収绠楀緱鐨勮兘婊¤冻瑕佹眰鐨勬渶澶杩愮畻锛�
			OverLimitFlag = OverLimitFlag + 1;
			break;
		}
	}

	BPM.BasePlateMoveA = BaseMovetoC*cos(TargetC);
	BPM.BasePlateMoveB = BaseMovetoC*sin(TargetC);

	*Target_X=TargetX;//璧嬪�煎埌鍑芥暟鍐咃紝鏂逛究璋冪敤鍑芥暟
	*Target_Y=TargetY;
	*Target_C=TargetC;
	return OverLimitFlag;

}





