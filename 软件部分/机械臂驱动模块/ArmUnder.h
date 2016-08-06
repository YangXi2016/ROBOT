#ifndef ARMUNDER_H_
#define ARMUNDER_H_
#include"Arduino.h"
#define FOCS 16000000 //鏅舵尟棰戠巼
#define BaseFre 100000  //鍩洪
#define DAFAULT_ARM_SPEED 1100  //涓诲壇鑷傞閫夐�熷害锛岀浉瀵瑰��
#define DAFAULT_ARMBASE_SPEED  1100//搴曠洏棣栭�夐�熷害锛岀浉瀵瑰��
#define StepEnable 0
#define StepDisable 1


/******************************************************************************
閸戣姤鏆熼崥锟�  閿涙rm_Motion
閸戣姤鏆熼崝鐔诲厴閿涙俺绶崙鐑樻煙濞夈垺甯堕崚鑸殿劄鏉╂稓鏁搁張鐚寸礉鏉╂稖锟藉奔濞囬張鐑橆潾閼峰倽绻嶉崝銊ュ煂閹稿洤鐣炬担宥囩枂
鏉堟挸鍙嗛崣姗�鍣洪敍锟�1. LengthMoveMain閵嗕俯engthMoveDeputy閸掑棗鍩嗘稉杞扮閺夊棜鐏В宥呭瀻閸掝偊娓剁憰浣告躬娑撴眹锟戒礁澹囨稉婵囨綄
             娑撳﹦娈戞担宥囆╂禒銉ㄧ箼缁傝瀵岄懛鍌濋叡閿涘牆澹囬懛鍌濋叡閿涘璐熷锝嗘煙閸氭埊绱濆В顐よ儗娑撳搫宕熸担锟�
          2. TurnC娑撻缚鍣︽惔鏇為獓闂囷拷鐟曚浇娴嗛惃鍕潡鎼达讣绱濇穱顖濐潒闁棙妞傞柦鍫滆礋濮濓綇绱濆褍瀹虫稉鍝勫礋娴ｅ稄绱�
鏉堟挸鍤�    閿涙俺绻戦崶鐐烘晩鐠囶垯淇婇幁顖ょ礉閼汇儴绻戦崶锟�0閸掓瑦顒滅敮闈╃礉鏉╂柨娲�-2閸掓瑨绶崙楦款攽缁嬪绉存潻鍥ㄦ簚濮婃壆绮ㄩ弸鍕畱闂勬劕鍩�
閻樿埖锟斤拷    閿涳拷2016/2/18 LIHB瀹稿弶鐗庢锟�
********************************************************************************/

/**************************************************************************************
 閺嬪嫰锟界姴鍤遍弫鐧唕mUnder();娑擃厼鍨垫慨瀣娴滐拷 濮濄儴绻樼化鑽ょ埠閸欏倹鏆熼妴浣诡劄鏉╂稓鏁搁張楦挎祮闁喆锟戒焦顒炴潻娑氭暩閺堝搫绱╅懘姘ワ拷鍌濐嚞閸︺劌鍤遍弫棰佽厬娣囶喗鏁�
***************************************************************************************/


struct PinStep {
	int PinClk;  //濮濄儴绻橀悽鍨簚妞瑰崬濮╅弮鍫曟寭clk+瀵洝鍓�
	int PinCW;   //濮濄儴绻橀悽鍨簚妞瑰崬濮ヽw+閹存牞锟藉崓tr+瀵洝鍓�
	int PinEn;   //濮濄儴绻橀悽鍨簚妞瑰崬濮〆n+瀵洝鍓�
};

struct StepSystemParameter    
{
	float  Lead;     //娑撴繃娼岀�佃偐鈻奸敍灞界俺鎼囱呮暩閺堝搫鍨弰顖滄暩閺堥缚娴嗘稉锟介崷鍫濈俺鎼囱嗘祮鏉╁洨娈戝褍瀹�
	float  Steps;    //濮濄儴绻橀悽鍨簚濮濄儲鏆�
	int    StepStyle;//閸楀﹥顒為幋鏍劀濮濄儻绱�1娑撶儤鏆ｅ銉礉2娑撳搫宕愬銉礉妞瑰崬濮╅弶澶哥瑐鐠嬪啳濡�
};

class ArmUnder{
public:
	int Arm_Motion(float LengthMoveMain,float LengthMoveDeputy, float TurnC); 
	void Init(float LengthMoveMain, float LengthMoveDeputy, float TurnC);
    int SpeedMain;   //娑撹崵鏁搁張楦挎祮闁喎鍙炬禒鏍ф倱閻烇拷
	int SpeedDeputy; 
	int SpeedArmBase;
	int DafaultSpeedMain;
	int DafaultSpeedArmBase;
	struct PinStep Main;                       //閻㈠灚婧�瀵洝鍓�
	struct PinStep Deputy;
	struct PinStep ArmBase;
	ArmUnder();      // 閺嬪嫰锟界姴鍤遍弫甯礉閸掓繂顫愰崠鏍ф倗缁夊秴褰夐柌锟�
	
private:
	int  PulsesCount(float LengthMoveMain, float LengthMoveDeputy, float TurnC);
	struct StepSystemParameter SSPMain;         //濮濄儴绻樼化鑽ょ埠閸欏倹鏆�
	struct StepSystemParameter SSPDeputy;       
	struct StepSystemParameter SSPArmBase;      
	long int  PulsesMain;                          //妞瑰崬濮╂稉顓熷闂囷拷閻ㄥ嫯鍓﹂崘鍙夋殶
	long int  PulsesDeputy;  
	long int  PulsesArmBase;
};

#endif





