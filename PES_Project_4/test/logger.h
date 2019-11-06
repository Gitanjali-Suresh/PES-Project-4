#ifndef _LOGGER_H

#define _LOGGER_H

#endif

typedef enum {
	main_fun,
	read_Temp,
	alert_Temp,
	wait_fun,
	wrong_Temp,
	change_fun,
	test_init,
	test_shut,
	str_write,
	int_write,
	test_recover,
	test_safe
}eFunction_name;

typedef enum {
	Status,
	Debug,
	Test
}eLog_level;

void log_Str(eLog_level mode, eFunction_name func, char *str);
void log_Int(int number);
void log_Char(char ch);
