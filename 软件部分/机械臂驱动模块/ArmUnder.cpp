#include "ArmUnder.h"
#include<math.h>




int ArmUnder::Arm_Motion(float LengthMoveMain, float LengthMoveDeputy, float TurnC)

{
	digitalWrite(Main.PinEn, StepEnable);   //娴ｈ儻鍏樻稉澶夐嚋濮濄儴绻橀悽鍨簚
	digitalWrite(Deputy.PinEn, StepEnable);
	digitalWrite(ArmBase.PinEn, StepEnable);

	int  CountMain = 0;     //閸掑棝顣堕弮鍓佹暏閻ㄥ嫯顓搁弫鐗堢垼韫囷拷
	int  CountDeputy = 0;
	int  CountArmBase = 0;

	int  MainFreDiv;      //閸掑棝顣堕弫锟�
	int  DeputyFreDiv;
	int  ArmBaseFreDiv;

	int  DelayTime;       //閸╂椽顣剁�电懓绨查惃鍕閺冿拷
	int  i = 0;           //鐠佲剝鏆熷顏嗗箚閸欐﹢鍣�
	int	 j = 0;
	int  k = 0;

	int  ErrorFlag = 0;   //閸戣姤鏆熸潻鏂挎礀閻ㄥ嫰鏁婄拠顖涚垼韫囨ぞ缍�

	int  OutMain = 1;     //妤傛ü缍嗛悽闈涢挬鐞涖劎銇氭担宥忕礉鏉堟挸鍤弬瑙勫皾閺冨墎鏁�
	int  OutDeputy = 1;
	int  OutArmBase = 1;

	//Serial.println(TurnC);
	ErrorFlag = PulsesCount(LengthMoveMain, LengthMoveDeputy, TurnC);//鐠侊紕鐣诲В蹇庨嚋閻㈠灚婧�閹碉拷闂囷拷閻ㄥ嫯鍓﹂崘鍙夋殶閿涘矁瀚㈠┃銏犲毉閸掓瑦濮ら柨锟�
	//Serial.println(PulsesArmBase);

	//閸掋倖鏌囬悽鍨簚鏉烆剙濮╅弬鐟版倻
	if (PulsesMain < 0)
		digitalWrite(Main.PinCW, LOW);
	else
		digitalWrite(Main.PinCW, HIGH);

	if (PulsesDeputy < 0)
		digitalWrite(Deputy.PinCW, HIGH);
	else
		digitalWrite(Deputy.PinCW, LOW);
	if (PulsesArmBase < 0)
		digitalWrite(ArmBase.PinCW, HIGH);
	else
		digitalWrite(ArmBase.PinCW, LOW);

	PulsesMain = 2 * abs(PulsesMain);        //鐎甸�涚艾娑撳鍨粙瀣碍閿涘本鐦￠崡濠傛噯閺堢剮ountmain閹存溈ountdeputy閹存溈ountarmbase++,閸ョ姵顒濈�圭偤妾拋鈩冩殶*2
	PulsesDeputy = 2 * abs(PulsesDeputy);
	PulsesArmBase = 2 * abs(PulsesArmBase);

	//DafaultSpeedArmBase = DAFAULT_ARMBASE_SPEED / 2;//转动量很小的情况下就不加减速了

	if (PulsesMain >= PulsesDeputy&&PulsesMain >= PulsesArmBase)
	{
		SpeedMain = DafaultSpeedMain;
		SpeedDeputy = round(1.0*SpeedMain*PulsesDeputy / PulsesMain);
		SpeedArmBase = round(1.0*SpeedMain*PulsesArmBase / PulsesMain);
	}
	else if (PulsesMain <= PulsesDeputy && PulsesDeputy >= PulsesArmBase)
	{
		SpeedDeputy = DafaultSpeedMain;
		SpeedMain = round(1.0*SpeedDeputy * PulsesMain / PulsesDeputy);
		SpeedArmBase = round(1.0*SpeedDeputy *PulsesArmBase / PulsesDeputy);
	}
	else
	{
		SpeedArmBase = DafaultSpeedArmBase;
		SpeedMain = round(1.0*SpeedArmBase*PulsesMain / PulsesArmBase);
		SpeedDeputy = round(1.0*SpeedArmBase*PulsesDeputy / PulsesArmBase);
	}

	if (SpeedArmBase > DafaultSpeedArmBase)
		SpeedArmBase = DafaultSpeedArmBase;


		MainFreDiv = BaseFre * 2 / (1.0*SpeedMain / 60 * SSPMain.Steps * SSPMain.StepStyle);           //鐠侊紕鐣婚崚鍡涱暥閸欏倹鏆�
		DeputyFreDiv = BaseFre * 2 / (1.0*SpeedDeputy / 60 * SSPDeputy.Steps * SSPDeputy.StepStyle);
		ArmBaseFreDiv = BaseFre * 2 / (1.0*SpeedArmBase / 60 * SSPArmBase.Steps * SSPArmBase.StepStyle);
   // Serial.println(PulsesArmBase);

		DelayTime = round(1000000 / BaseFre / 2);//鐠侊紕鐣荤�电懓绨查崺娲暥閻ㄥ嫬娆㈤弮锟�

		//Serial.println(millis());

		//鐎甸�涚瑏娑擃亞鏁搁張鍝勬倱閺冩儼绶崙楦垮墻閸愶拷
		if (PulsesArmBase < 800)
		{
			ArmBaseFreDiv = BaseFre * 2 / (1.0*DAFAULT_ARMBASE_SPEED / 60 * SSPArmBase.Steps * SSPArmBase.StepStyle)*3;//转动距离很小就直接1/3最高速转动

			while (CountMain != PulsesMain || CountDeputy != PulsesDeputy || CountArmBase != PulsesArmBase)
			{
				delayMicroseconds(DelayTime);

				i++;
				j++;
				k++;
				if (i == MainFreDiv && CountMain != PulsesMain)
				{
					CountMain++;
					i = 0;
					OutMain ^= 0x01;
					digitalWrite(Main.PinClk, OutMain);
				}
				if (j == DeputyFreDiv && CountDeputy != PulsesDeputy)
				{
					CountDeputy++;
					j = 0;
					OutDeputy ^= 0x01;
					digitalWrite(Deputy.PinClk, OutDeputy);
				}
				if (k == ArmBaseFreDiv && CountArmBase != PulsesArmBase)
				{
					CountArmBase++;
					k = 0;
					OutArmBase ^= 0x01;
					digitalWrite(ArmBase.PinClk, OutArmBase);
				}
			}
		}
		else //否则加减速
		{
			long int DivTemp;
			DivTemp = ArmBaseFreDiv;
			ArmBaseFreDiv *= 3;
			while (CountMain != PulsesMain || CountDeputy != PulsesDeputy || CountArmBase != PulsesArmBase)
			{
				delayMicroseconds(DelayTime);

				i++;
				j++;
				k++;
				if (i == MainFreDiv && CountMain != PulsesMain)
				{
					CountMain++;
					i = 0;
					OutMain ^= 0x01;
					digitalWrite(Main.PinClk, OutMain);
				}
				if (j == DeputyFreDiv && CountDeputy != PulsesDeputy)
				{
					CountDeputy++;
					j = 0;
					OutDeputy ^= 0x01;
					digitalWrite(Deputy.PinClk, OutDeputy);
				}
				if (k == ArmBaseFreDiv && CountArmBase != PulsesArmBase)
				{
					
					CountArmBase++;
					k = 0;
					OutArmBase ^= 0x01;
					digitalWrite(ArmBase.PinClk, OutArmBase);
					if (CountArmBase < 400 && ArmBaseFreDiv>DivTemp)
					{

						ArmBaseFreDiv = ceil(DivTemp*3.0-DivTemp*2.0*CountArmBase/400);
						//Serial.println(ArmBaseFreDiv);
					}
					else if (CountArmBase > PulsesArmBase - 400)
					{
						ArmBaseFreDiv = ceil(DivTemp*1.0+DivTemp*2.0*(CountArmBase-PulsesArmBase+400)/400);
						//Serial.println(ArmBaseFreDiv);
					}
				}
			}

		}
	//digitalWrite(Main.PinEn, HIGH);   //缁備焦顒涙稉璇插濮濄儴绻橀悽鍨簚閿涘苯鍣虹亸蹇曟暩濮圭姾绶崙楦跨閹凤拷
	//digitalWrite(Deputy.PinEn, HIGH);
	return ErrorFlag;


}

ArmUnder::ArmUnder()
{
	   SSPMain.Lead = 4;
	   SSPMain.Steps = 200;
	   SSPMain.StepStyle = 2;

	   SSPDeputy.Lead = 4;
	   SSPDeputy.Steps = 200;
	   SSPDeputy.StepStyle = 2;

	   SSPArmBase.Lead = 0.54478;
	   SSPArmBase.Steps = 200;
	   SSPArmBase.StepStyle = 2;

		SpeedMain = 320;   //娑撹崵鏁搁張楦挎祮闁噦绱濇妯款吇300rmp
		SpeedDeputy = 320; //閸氬瞼鎮�
		SpeedArmBase = 150;//閸氬瞼鎮�
		
		Main.PinClk = 3;   //娑撹崵鏁搁張鍝勵嚠鎼存摐rduino娑撳﹤绱╅懘姘剧礉閸忔湹绮崥宀勫櫡
		Main.PinCW = 4;
		Main.PinEn = 5;

		Deputy.PinClk = 6;
		Deputy.PinCW = 7;
		Deputy.PinEn = 8;

		ArmBase.PinClk =9;
		ArmBase.PinCW = A3;
		ArmBase.PinEn = A4;
		pinMode(Main.PinClk, OUTPUT);
		pinMode(Main.PinCW, OUTPUT);
		pinMode(Main.PinEn, OUTPUT);

		pinMode(Deputy.PinClk, OUTPUT);
		pinMode(Deputy.PinCW, OUTPUT);
		pinMode(Deputy.PinEn, OUTPUT);

		pinMode(ArmBase.PinClk, OUTPUT);
		pinMode(ArmBase.PinCW, OUTPUT);
		pinMode(ArmBase.PinEn, OUTPUT);
		DafaultSpeedMain = DAFAULT_ARM_SPEED;
		DafaultSpeedArmBase= DAFAULT_ARMBASE_SPEED;

}

//閸戣姤鏆熼悽銊ょ艾鐠侊紕鐣婚幍锟介棁锟介懘澶婂暱閺侊拷
int ArmUnder::PulsesCount(float LengthMoveMain, float LengthMoveDeputy, float TurnC)
{
	float MainMin;     //濮ｅ繋閲滈懘澶婂暱鐎电懓绨叉稉婵囨綄閾荤儤鐦濇担宥囆�
	float DeputyMin;
	float ArmBaseMin;
	float CountTemp;   //鐠侊紕鐣绘稉顓犳畱娑擃參妫块崣姗�鍣�
	int   ErrorFlag = 1;


	if (LengthMoveMain > 140 || LengthMoveMain < -140 || LengthMoveDeputy>150 || LengthMoveDeputy < -150 || TurnC>2 * PI || TurnC < -2 * PI)
		return -2; //鐡掑懓绻冮張鐑樼�担宥囆╅張锟芥径褍锟斤拷

	MainMin = SSPMain.Lead / SSPMain.Steps / SSPMain.StepStyle;
	DeputyMin = SSPDeputy.Lead / SSPDeputy.Steps / SSPDeputy.StepStyle;
	ArmBaseMin = SSPArmBase.Lead / SSPArmBase.Steps / SSPArmBase.StepStyle;

	PulsesMain = round(LengthMoveMain / MainMin);
	PulsesDeputy = round(LengthMoveDeputy / DeputyMin);
	PulsesArmBase = round(TurnC / ArmBaseMin);
	return 0;


}
void ArmUnder::Init(float LengthMoveMain, float LengthMoveDeputy, float TurnC)
{

	digitalWrite(Main.PinEn, StepEnable);   //娴ｈ儻鍏樻稉澶夐嚋濮濄儴绻橀悽鍨簚
	digitalWrite(Deputy.PinEn, StepEnable);
	digitalWrite(ArmBase.PinEn, StepEnable);

	int  CountMain = 0;     //閸掑棝顣堕弮鍓佹暏閻ㄥ嫯顓搁弫鐗堢垼韫囷拷
	int  CountDeputy = 0;
	int  CountArmBase = 0;

	int  MainFreDiv;      //閸掑棝顣堕弫锟�
	int  DeputyFreDiv;
	int  ArmBaseFreDiv;

	int  DelayTime;       //閸╂椽顣剁�电懓绨查惃鍕閺冿拷
	int  i = 0;           //鐠佲剝鏆熷顏嗗箚閸欐﹢鍣�
	int	 j = 0;
	int  k = 0;

	int  ErrorFlag = 0;   //閸戣姤鏆熸潻鏂挎礀閻ㄥ嫰鏁婄拠顖涚垼韫囨ぞ缍�

	int  OutMain = 1;     //妤傛ü缍嗛悽闈涢挬鐞涖劎銇氭担宥忕礉鏉堟挸鍤弬瑙勫皾閺冨墎鏁�
	int  OutDeputy = 1;
	int  OutArmBase = 1;


	ErrorFlag = PulsesCount(LengthMoveMain, LengthMoveDeputy, TurnC);//鐠侊紕鐣诲В蹇庨嚋閻㈠灚婧�閹碉拷闂囷拷閻ㄥ嫯鍓﹂崘鍙夋殶閿涘矁瀚㈠┃銏犲毉閸掓瑦濮ら柨锟�


																	 //閸掋倖鏌囬悽鍨簚鏉烆剙濮╅弬鐟版倻
	if (PulsesMain < 0)
		digitalWrite(Main.PinCW, LOW);
	else
		digitalWrite(Main.PinCW, HIGH);

	if (PulsesDeputy < 0)
		digitalWrite(Deputy.PinCW, HIGH);
	else
		digitalWrite(Deputy.PinCW, LOW);
	if (PulsesArmBase < 0)
		digitalWrite(ArmBase.PinCW, HIGH);
	else
		digitalWrite(ArmBase.PinCW, LOW);

	PulsesMain = 2 * abs(PulsesMain);        //鐎甸�涚艾娑撳鍨粙瀣碍閿涘本鐦￠崡濠傛噯閺堢剮ountmain閹存溈ountdeputy閹存溈ountarmbase++,閸ョ姵顒濈�圭偤妾拋鈩冩殶*2
	PulsesDeputy = 2 * abs(PulsesDeputy);
	PulsesArmBase = 2 * abs(PulsesArmBase);

/*
	if (PulsesMain >= PulsesDeputy&&PulsesMain >= PulsesArmBase)
	{
		SpeedMain = DafaultSpeedMain;
		SpeedDeputy = round(1.0*SpeedMain*PulsesDeputy / PulsesMain);
		SpeedArmBase = round(1.0*SpeedMain*PulsesArmBase / PulsesMain);
	}
	else if (PulsesMain <= PulsesDeputy && PulsesDeputy >= PulsesArmBase)
	{
		SpeedDeputy = DafaultSpeedMain;
		SpeedMain = round(1.0*SpeedDeputy * PulsesMain / PulsesDeputy);
		SpeedArmBase = round(1.0*SpeedDeputy *PulsesArmBase / PulsesDeputy);
	}
	else
	{
		SpeedArmBase = DafaultSpeedArmBase;
		SpeedMain = round(1.0*SpeedArmBase*PulsesMain / PulsesArmBase);
		SpeedDeputy = round(1.0*SpeedArmBase*PulsesDeputy / PulsesArmBase);
	}

	if (SpeedArmBase > DafaultSpeedArmBase)
		SpeedArmBase = DafaultSpeedArmBase;
*/
	SpeedMain = DafaultSpeedMain;
	SpeedDeputy = DafaultSpeedMain;
	SpeedArmBase = DafaultSpeedArmBase;
	MainFreDiv = BaseFre * 2 / (1.0*SpeedMain / 60 * SSPMain.Steps * SSPMain.StepStyle);           //鐠侊紕鐣婚崚鍡涱暥閸欏倹鏆�
	DeputyFreDiv = BaseFre * 2 / (1.0*SpeedDeputy / 60 * SSPDeputy.Steps * SSPDeputy.StepStyle);
	ArmBaseFreDiv = BaseFre * 2 / (1.0*SpeedArmBase / 60 * SSPArmBase.Steps * SSPArmBase.StepStyle);

	DelayTime = round(1000000 / BaseFre / 2);//鐠侊紕鐣荤�电懓绨查崺娲暥閻ㄥ嫬娆㈤弮锟�

											 //Serial.println(millis());

											 //鐎甸�涚瑏娑擃亞鏁搁張鍝勬倱閺冩儼绶崙楦垮墻閸愶拷
	while (CountMain != PulsesMain || CountDeputy != PulsesDeputy || CountArmBase != PulsesArmBase)
	{
		delayMicroseconds(DelayTime);

		i++;
		j++;
		k++;
		if (i == MainFreDiv && CountMain != PulsesMain)
		{
			CountMain++;
			i = 0;
			OutMain ^= 0x01;
			digitalWrite(Main.PinClk, OutMain);
		}
		if (j == DeputyFreDiv && CountDeputy != PulsesDeputy)
		{
			CountDeputy++;
			j = 0;
			OutDeputy ^= 0x01;
			digitalWrite(Deputy.PinClk, OutDeputy);
		}
		if (k == ArmBaseFreDiv && CountArmBase != PulsesArmBase)
		{
			CountArmBase++;
			k = 0;
			OutArmBase ^= 0x01;
			digitalWrite(ArmBase.PinClk, OutArmBase);
		}
	}


	//digitalWrite(Main.PinEn, HIGH);   //缁備焦顒涙稉璇插濮濄儴绻橀悽鍨簚閿涘苯鍣虹亸蹇曟暩濮圭姾绶崙楦跨閹凤拷
	//digitalWrite(Deputy.PinEn, HIGH);


}







