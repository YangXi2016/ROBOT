#include "Communication.h"

int* splitcommand(String command, char flag, int length){
	int i;
	String parameter;
	int parameters[5];
	int commaPosition;
	for (i = 0; i < length - 1; i++)
	{
		commaPosition = command.indexOf(flag);
		if (commaPosition == -1){
			parameters[i] = -1;
			break;
		}

		parameter = command.substring(0, commaPosition);
		command = command.substring(commaPosition + 1, command.length());
		parameters[i] = parameter.toInt();
	}
	parameters[length - 1] = command.toInt();

	return parameters;
}
