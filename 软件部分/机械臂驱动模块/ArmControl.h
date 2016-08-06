#ifndef _ARM_CONTROL_H
#define _ARM_CONTROL_H
#ifndef PI
#define PI 3.1415926
#endif 
#include"ArmUnder.h"

/****************************************************************************************************
鍑芥暟鍚�  锛欰rm_Move
鍑芥暟鍔熻兘锛氭牴鎹緭鍏ョ殑鏈烘鑷傛湯绔潗鏍囷紙x,y,c锛夎绠楀嚭涓濇潌铻烘瘝鐨勪綅绉婚噺鍜岃噦搴曞骇鐨勮浆鍔ㄩ噺
杈撳叆    锛歍arget_X,Target_Y,Target_C鍒嗗埆涓烘満姊拌噦鏈闇�瑕佸埌杈剧殑鐩爣鍧愭爣锛屽潗鏍囧畾涔夎鍙傜収椤圭洰鍧愭爣绯�
杈撳嚭    锛氳繑鍥炵殑閿欒淇℃伅锛�-1-鏈垵濮嬪寲锛�0-姝ｅ父锛�1-闇�瑕佸簳搴х數鏈鸿緟鍔╋紱2-y瓒婄晫锛�3-y瓒婄晫涓旈渶瑕佸簳搴х數鏈鸿緟鍔�;
澶囨敞    锛�1. 鍑芥暟鏀瑰彉ArmM.ArmS锛孊PM涓殑鍊硷紝璇诲彇ArmM涓殑鎸囧嵆鍙幏寰楀綋鍓嶆満姊拌噦闇�瑕佺殑绉诲姩閲忥紝璇诲彇ArmS涓殑鍊煎嵆鍙�
             鑾峰緱缁忚繃褰撳墠绉诲姩鍚庢満姊拌噦灏栫鐨勪綅缃�,璇诲彇BPM涓殑鍊煎嵆鍙緱鍒板綋鍓嶆暣鏈洪渶瑕佺Щ鍔ㄧ殑閲�
		  2. 杩愯鍑芥暟涔嬪墠鏈烘鑷傝嚦灏戝簲璇ュ垵濮嬪寲杩囦竴娆�
鐘舵��    锛�2016/2/18 LIHB宸叉牎楠�
******************************************************************************************************/

/****************************************************************************************************
鍑芥暟鍚�  锛欼nit_Arm()
鍑芥暟鍔熻兘锛氬皢鏈烘鑷傚垵濮嬪寲鍒伴瀹氫綅缃�
杈撳叆    锛氭棤
杈撳嚭    锛氭棤
澶囨敞    锛氬彲鍦ㄦ牴鎹疄闄呮満姊拌噦鍒拌揪鐨勪綅缃慨鏀瑰垵濮嬪弬鏁�
鐘舵��    锛�2016/2/18 LIHB宸叉牎楠�
******************************************************************************************************/

/*************************************************************
鏋勯�犲嚱鏁癆rmPC(); 涓鏈烘鑷傚悇绉嶆満姊板弬鏁拌祴鍊硷紝鍏朵腑鏈烘鑷傜殑
                 鏋侀檺浣嶇疆鏄彲浠ユ牴鎹疄闄呮儏鍐典慨鏀圭殑
**************************************************************/



struct ArmMotion {
	float LengthMoveMain;   //涓讳笣鏉嗚灪姣嶉渶瑕佺Щ鍔ㄧ殑闀垮害锛岃繙绂讳富鑷備富杞翠负姝ｆ柟鍚�  
	float LengthMoveDeputy; //鍓笣鏉嗚灪姣嶉渶瑕佺Щ鍔ㄧ殑闀垮害锛岃繙绂诲壇鑷備富杞翠负姝ｆ柟鍚�  
	float TurnC;            //鑷傚簳搴ч渶瑕佽浆杩囩殑瑙掑害锛屼刊瑙嗭紝閫嗘椂閽堜负姝ｏ紝椤烘椂閽堜负璐�
};

struct ArmStatus {
	float ArmTipX;          //鏈烘鑷傝繍鍔ㄥ钩闈腑锛屽壇鑷傛湯绔按骞虫柟鍚戝潗鏍囷紝浠ヤ富鑷備富杞翠负鍘熺偣锛屽悜鍓嶄负姝ｆ柟鍚�
	float ArmTipY;          //鏈烘鑷傝繍鍔ㄥ钩闈腑锛屽壇鑷傛湯绔珫鐩存柟鍚戝潗鏍囷紝浠ヤ富鑷備富杞翠负鍘熺偣锛屽悜涓婁负姝ｆ柟鍚�
	float ArmC;             //鏈烘鑷傝繍鍔ㄥ钩闈笌涓诲钩闈㈠す瑙�
	float ArmAngleMain;     //涓昏噦涓庢按骞抽潰鐨勫す瑙�
	float ArmAngleDeputy;   //鍓噦涓庢按骞抽潰鐨勫す瑙�
	float ArmAngleMD;       //涓昏噦寤堕暱绾夸笌鍓噦涔嬮棿鐨勫す瑙�
};

struct BasePlateMotion {
	float BasePlateMoveA;   //鍦伴潰鍐呭缓绔嬪潗鏍囩郴锛屾暣涓満鍣ㄤ汉闇�瑕佸悜A鏂瑰悜绉诲姩鐨勮窛绂�
	float BasePlateMoveB;   //鍦伴潰鍐呭缓绔嬪潗鏍囩郴锛屾暣涓満鍣ㄤ汉闇�瑕佸悜B鏂瑰悜绉诲姩鐨勮窛绂�   
};



struct ArmParameter {
	float LengthMainHand;        //涓昏噦鏂滄敮鍑哄幓閮ㄥ垎鐨勯暱搴�
	float AngleMainHand;         //涓昏噦鏂滄敮鍑哄幓閮ㄥ垎涓庝富鑷傞棿鐨勮搴︼紝寮у害鍊肩殑41搴�
	float LengthMainConnect;     //涓昏噦浼犲姩杩炴潌鐨勭瓑鏁堢洿绾块暱搴�
	float LengthDeputyHand;      //鍓噦鏂滄敮鍑哄幓閮ㄥ垎鐨勯暱搴�
	float AngleDeputyHand;       //鍓噦鏂滄敮鍑哄幓閮ㄥ垎涓庡壇鑷傞棿鐨勮搴︼紝寮у害鍊肩殑50搴�
	float LengthDeputyConnect;   //鍓噦浼犲姩杩炴潌鐨勭瓑鏁堢洿绾块暱搴�
	float LengthArm;             //涓昏噦鍜屽壇鑷傜殑闀垮害
	float AngleMainMin;          //涓昏噦涓庢按骞抽潰澶硅鐨勬渶灏忓�� 
	float AngleMainMax;          //鍚岀悊 
	float AngleMDMin;            //涓昏噦寤堕暱绾夸笌鍓噦澶硅鏈�灏忓�� 
	float AngleMDMax;            //鍚岀悊 
	float AngleBaseMin;          //鑷傚簳搴у湪AB鍧愭爣绯讳笅鏈�灏忚兘鍒扮殑瑙掑害
	float AngleBaseMax;          //鍚岀悊
};




class ArmPC{
public:
	int    Arm_Move(float Target_X, float Target_Y, float Target_C);
	void   Move_Forward(float Target_X, float Target_Y, float Target_C);//鐢ㄤ簬姘村钩鎺ヨ繎鐩爣
	void   Init_Arm(int firsttimeflag);
	void   Arm_Wait();          //浣挎満姊拌噦绛夊緟鍦ㄦ寚瀹氫綅缃�
	struct ArmStatus ArmS;      //鏈烘鑷傜殑鐘舵�佸弬鏁� 
	struct BasePlateMotion BPM; //鏁存満闇�瑕佽緟鍔╄繍鍔ㄧ殑鍙傛暟
	struct ArmMotion ArmM;      //鏈烘鑷傜殑杩愬姩鍙傛暟
	ArmUnder ArmU;              //搴曞眰鎺у埗绫�
	ArmPC();                    //缁欏悇绉嶅弬鏁拌祴鍊�
private:
	int    Base_Assist(float *Target_X, float *Target_Y, float *Target_C); //0姝ｅ父锛�1-闇�瑕佸簳搴х數鏈鸿緟鍔╋紱2-y瓒婄晫锛�3-y鍧囩晫涓旈渶瑕佸簳搴х數鏈鸿緟鍔�
	struct ArmParameter ArmP;                   //鏈烘鑷傚弬鏁�
	int    InitFlag;    //鏍囪鏄惁缁忚繃鍒濆鍖�
	float  LengthMain;    //鍘嗗彶涓昏噦铻烘瘝缁濆浣嶇疆,浠ヤ富鑷備富杞翠负鍘熺偣锛岃繙绂昏酱涓烘鏂瑰悜
	float  LengthDeputy;  //鍘嗗彶鍓噦铻烘瘝缁濆浣嶇疆锛屼互鍓噦涓昏酱涓哄師鐐癸紝杩滅杞翠负姝ｆ柟鍚�
};

#endif


