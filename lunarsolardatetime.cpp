#include "lunarsolardatetime.h"

const unsigned int yearCode[] = {
	0x04bd8, 0x04ae0, 0x0a570, 0x054d5, 0x0d260, // 1904
	0x0d950, 0x16554, 0x056a0, 0x09ad0, 0x055d2, // 1909
	0x04ae0, 0x0a5b6, 0x0a4d0, 0x0d250, 0x1d255, // 1914
	0x0b540, 0x0d6a0, 0x0ada2, 0x095b0, 0x14977, // 1919
	0x04970, 0x0a4b0, 0x0b4b5, 0x06a50, 0x06d40, // 1924
	0x1ab54, 0x02b60, 0x09570, 0x052f2, 0x04970, // 1929
	0x06566, 0x0d4a0, 0x0ea50, 0x06e95, 0x05ad0, // 1934
	0x02b60, 0x186e3, 0x092e0, 0x1c8d7, 0x0c950, // 1939
	0x0d4a0, 0x1d8a6, 0x0b550, 0x056a0, 0x1a5b4, // 1944
	0x025d0, 0x092d0, 0x0d2b2, 0x0a950, 0x0b557, // 1949
	0x06ca0, 0x0b550, 0x15355, 0x04da0, 0x0a5d0, // 1954
	0x14573, 0x052d0, 0x0a9a8, 0x0e950, 0x06aa0, // 1959
	0x0aea6, 0x0ab50, 0x04b60, 0x0aae4, 0x0a570, // 1964
	0x05260, 0x0f263, 0x0d950, 0x05b57, 0x056a0, // 1969
	0x096d0, 0x04dd5, 0x04ad0, 0x0a4d0, 0x0d4d4, // 1974
	0x0d250, 0x0d558, 0x0b540, 0x0b5a0, 0x195a6, // 1979
	0x095b0, 0x049b0, 0x0a974, 0x0a4b0, 0x0b27a, // 1984
	0x06a50, 0x06d40, 0x0af46, 0x0ab60, 0x09570, // 1989
	0x04af5, 0x04970, 0x064b0, 0x074a3, 0x0ea50, // 1994
	0x06b58, 0x055c0, 0x0ab60, 0x096d5, 0x092e0, // 1999
	0x0c960, 0x0d954, 0x0d4a0, 0x0da50, 0x07552, // 2004
	0x056a0, 0x0abb7, 0x025d0, 0x092d0, 0x0cab5, // 2009
	0x0a950, 0x0b4a0, 0x0baa4, 0x0ad50, 0x055d9, // 2014
	0x04ba0, 0x0a5b0, 0x15176, 0x052b0, 0x0a930, // 2019
	0x07954, 0x06aa0, 0x0ad50, 0x05b52, 0x04b60, // 2024
	0x0a6e6, 0x0a4e0, 0x0d260, 0x0ea65, 0x0d530, // 2029
	0x05aa0, 0x076a3, 0x096d0, 0x04bd7, 0x04ad0, // 2034
	0x0a4d0, 0x1d0b6, 0x0d250, 0x0d520, 0x0dd45, // 2039
	0x0b5a0, 0x056d0, 0x055b2, 0x049b0, 0x0a577, // 2044
	0x0a4b0, 0x0aa50, 0x1b255, 0x06d20, 0x0ada0  // 2049
};

const wchar_t *LunarMonthName[] = {
	L"����", L"����", L"����", L"����", L"����", L"����",
	L"����", L"����", L"����", L"ʮ��", L"ʮһ��", L"����"
};
const wchar_t* LunarDayName[] = {
	L"��һ", L"����", L"����", L"����", L"����", L"����", L"����", L"����", L"����", L"��ʮ",
	L"ʮһ", L"ʮ��", L"ʮ��", L"ʮ��", L"ʮ��", L"ʮ��", L"ʮ��", L"ʮ��", L"ʮ��", L"��ʮ",
	L"إһ", L"إ��", L"إ��", L"إ��", L"إ��", L"إ��", L"إ��", L"إ��", L"إ��", L"��ʮ"
};

const wchar_t *Gan[] = {L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��"};
const wchar_t *Zhi[] = {L"��", L"��", L"��", L"î", L"��", L"��", L"��", L"δ", L"��", L"��", L"��", L"��"};
const wchar_t *ZodiacName[] = {L"��", L"ţ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��"};
const wchar_t *solarTerm[] = {
	L"С��", L"��", L"����", L"��ˮ", L"����", L"����", 
	L"����", L"����", L"����", L"С��", L"â��", L"����", 
	L"С��", L"����", L"����", L"����", L"��¶", L"���",
	L"��¶", L"˪��", L"����", L"Сѩ", L"��ѩ", L"����", 
};
#if 0
const unsigned int solarTermCode[] = {
	0, 21208, 42467, 63836, 85337, 107014,
	128867, 150921, 173149, 195551, 218072, 240693,
	263343, 285989, 308563, 331033, 353350, 375494,
	397447, 419210, 440795, 462224, 483532, 504758
};
#else

//����gLanarHoliDay���ÿ��Ķ�ʮ�Ľ�����Ӧ����������
//ÿ��Ķ�ʮ�Ľ�����Ӧ���������ڼ����̶���ƽ���ֲ���ʮ��������
//   1��          2��         3��         4��         5��         6��   
//С�� ��   ����  ��ˮ   ���� ����   ���� ����   ���� С��   â�� ����
//   7��          8��         9��         10��       11��        12��  
//С�� ����   ����  ����   ��¶ ���   ��¶ ˪��   ���� Сѩ   ��ѩ ����

/*********************************************************************************
 �������κ�ȷ������,����ֻ�ô��,Ҫ��ʡ�ռ�,����....
  �������ִ淨ʵ����̫��̬��,��ͽ����ſ���
**********************************************************************************/
//���ݸ�ʽ˵��:
//��1901��Ľ���Ϊ
//  1��     2��     3��   4��    5��   6��   7��    8��   9��    10��  11��     12��
// 6, 21, 4, 19,  6, 21, 5, 21, 6,22, 6,22, 8, 23, 8, 24, 8, 24, 8, 24, 8, 23, 8, 22
// 9, 6,  11,4,   9, 6,  10,6,  9,7,  9,7,  7, 8,  7, 9,  7,  9, 7,  9, 7,  8, 7, 15
//�����һ������Ϊÿ�½�����Ӧ����,15��ȥÿ�µ�һ������,ÿ�µڶ���������ȥ15�õڶ���
// ����ÿ������������Ӧ���ݶ�С��16,ÿ����һ���ֽڴ��,��λ��ŵ�һ����������,��λ���
//�ڶ�������������,�ɵ��±�

unsigned char solarTermCode[]=
{
	0X95, 0XB3, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1900
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1901
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1902
	0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1903
	0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //1904
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1905
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1906
	0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1907
	0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1908
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1909
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1910
	0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1911
	0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1912
	0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1913
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1914
	0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1915
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1916
	0X95, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1917
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   //1918
	0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1919
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1920
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1921
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   //1922
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1923
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1924
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1925
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1926
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1927
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1928
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1929
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1930
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1931
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1932
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1933
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1934
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1935
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1936
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1937
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1938
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1939
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1940
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1941
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1942
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1943
	0X96, 0XA5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1944
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1945
	0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1946
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1947
	0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1948
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X79, 0X77, 0X87,   //1949
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1950
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1951
	0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1952
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1953
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X68, 0X78, 0X87,   //1954
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1955
	0X96, 0XA5, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1956
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1957
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1958
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1959
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1960
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1961
	0X96, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1962
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1963
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1964
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1965
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1966
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1967
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1968
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1969
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1970
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1971
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1972
	0XA5, 0XB5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1973
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1974
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1975
	0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X89, 0X88, 0X78, 0X87, 0X87,   //1976
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1977
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //1978
	0X96, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1979
	0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1980
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X77, 0X87,   //1981
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1982
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1983
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //1984
	0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1985
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1986
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X69, 0X78, 0X87,   //1987
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1988
	0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1989
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1990
	0X95, 0XB4, 0X96, 0XA5, 0X86, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1991
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1992
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1993
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1994
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X76, 0X78, 0X69, 0X78, 0X87,   //1995
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1996
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1997
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1998
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1999
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2000
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2001
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2002
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //2003
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2004
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2005
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2006
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //2007
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2008
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2009
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2010
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //2011
	0X96, 0XB4, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2012
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2013
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2014
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2015
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2016
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2017
	0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2018
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2019
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   //2020
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2021
	0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2022
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2023
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2024
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2025
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2026
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2027
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2028
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2029
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2030
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2031
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2032
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X86,   //2033
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X78, 0X88, 0X78, 0X87, 0X87,   //2034
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2035
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2036
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2037
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2038
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2039
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2040
	0XA5, 0XC3, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2041
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2042
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2043
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X88, 0X87, 0X96,   //2044
	0XA5, 0XC3, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2045
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2046
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2047
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA5, 0X97, 0X87, 0X87, 0X88, 0X86, 0X96,   //2048
	0XA4, 0XC3, 0XA5, 0XA5, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   //2049
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X78, 0X78, 0X87, 0X87    //2050
};
#endif


const HOLIDAY_t solarHoliday[] = {
	{ 1,1, L"Ԫ��" },
	{ 2,14, L"���˽�" },
	{ 3,8, L"��Ů��" },
	{ 3,12, L"ֲ����" },
	{ 4,1, L"���˽�" },
	{ 5,1, L"�Ͷ���" },
	{ 5,4, L"�����" }, 
	{ 6,1, L"��ͯ��" },
	{ 7,1, L"������" },
	{ 8,1, L"������" },
	{ 9,10, L"��ʦ��" },
	{ 10,1, L"�����" },
	{ 12,25, L"ʥ����" }
};

const HOLIDAY_t lunarHoliday[] = {
	{ 1,1, L"����" },
	{ 1,15, L"Ԫ��" },
	{ 5,5, L"����" },
	{ 7,7, L"��Ϧ" },
	{ 8,15, L"����" },
	{ 9,9, L"����" },
	{ 12,8, L"����" }
};

const unsigned char solarDaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned char lunarMonthDay[] = {29, 30};

//����
enum jcIndex{ JIAN = 0, CHU = 1, MAN, PING, DING, ZHI, PO, WEI, CHEN, SHOU, KAI, BI};
const unsigned char JianCangs[12][12] = {
	{JIAN,CHU,MAN,PING,DING,ZHI,PO,WEI,CHEN,SHOU,KAI,BI},
	{BI,JIAN,CHU,MAN,PING,DING,ZHI,PO,WEI,CHEN,SHOU,KAI},
	{KAI,BI,JIAN,CHU,MAN,PING,DING,ZHI,PO,WEI,CHEN,SHOU},
	{SHOU,KAI,BI,JIAN,CHU,MAN,PING,DING,ZHI,PO,WEI,CHEN},
	{CHEN,SHOU,KAI,BI,JIAN,CHU,MAN,PING,DING,ZHI,PO,WEI},
	{WEI,CHEN,SHOU,KAI,BI,JIAN,CHU,MAN,PING,DING,ZHI,PO},
	{PO,WEI,CHEN,SHOU,KAI,BI,JIAN,CHU,MAN,PING,DING,ZHI},
	{ZHI,PO,WEI,CHEN,SHOU,KAI,BI,JIAN,CHU,MAN,PING,DING},
	{DING,ZHI,PO,WEI,CHEN,SHOU,KAI,BI,JIAN,CHU,MAN,PING},
	{PING,DING,ZHI,PO,WEI,CHEN,SHOU,KAI,BI,JIAN,CHU,MAN},
	{MAN,PING,DING,ZHI,PO,WEI,CHEN,SHOU,KAI,BI,JIAN,CHU},
	{CHU,MAN,PING,DING,ZHI,PO,WEI,CHEN,SHOU,KAI,BI,JIAN},
};
//��
const wchar_t* huangli_yi[] = {
	L"����,����,����,����,����",
	L"����,�Ʋ�,����,��ж,��լ",
	L"��,����,����,����,����",
	L"����,�޷�,Ϳ��,������ȡ ",
	L"����,��ȯ,����,ǩԼ,����",
	L"��,����,����,���,��Լ",
	L"��ҽ,����,����,������ȡ ",
	L"��Ӫ,����,���,����,����",
	L"��,��ѧ,����,��ҽ,�ɷ�",
	L"����,���,ǩԼ,��Ȣ,����",
	L"�Ʋ�,���,����,���,��ְ",
	L"����,����,�ղ�,����     ",
};

//��
const wchar_t* huangli_ji[] = {
	L"����,����,��Ȣ,�ɲ�     ",
	L"���,����,����,���,̽��",
	L"��ҩ,��ҽ,����,����,Ǩ��",
	L"����,��լ,��Ȣ,����,����",
	L"��ֲ,��ҵ,����,��,�촬",
	L"����,����,���,Զ��     ",
	L"����,����,����,����,����",
	L"�Ǹ�,�д�,����,��լ,����",
	L"����,���T,����          ",
	L"����,����,����,��լ,����",
	L"����,����,���          ",
	L"���,����,����,��Ȣ,����",
};

//����
const wchar_t* huangli_ts[] = {
	L"��ֵ���� ���²���",
	L"��ֵ���� ���²���",
	L"��ֵ��˷ ���²���",
	L"��ֵ�ʮ���� ���²���",
};

// Calculate days in a year
unsigned int LunarSolarDateTime::daysOfYear(LSDate date){
	DWORD _year = date.year - 1;
	int days = _year*365 + _year/4 - _year/100 + _year/400;
	for(int i = 1; i < date.month; i++){
		days += SolarDaysInMonth(date.year,i);
	}
	days += date.day;
	return days;
}


int LunarSolarDateTime::diffDate(LSDate d1,LSDate d2){
	return (daysOfYear(d1) - daysOfYear(d2));
}


// Determines if is solar leap year
bool LunarSolarDateTime::IsSolarLeapYear(DWORD year){
	return (year % 4 == 0 &&
		year % 100 != 0 ||
		year % 400 == 0);
}


// Returns count of days in month
unsigned char LunarSolarDateTime::SolarDaysInMonth(DWORD year, DWORD month){
	return (IsSolarLeapYear(year) && month == 2) ? 
		29 : solarDaysInMonth[month - 1];
}


// Returns offset days from base date
int LunarSolarDateTime::SolarDaysFromBaseDate(){
	return diffDate(solar,baseDate);
}

// ũ��ת���� 
void LunarSolarDateTime::LunarToSolar(){
	int isets = lunar.year - baseDate.year;
	int offset = 0;
	for(int iYear = baseDate.year; iYear < baseDate.year + isets; iYear++){
		offset += LunarSolarDateTime(iYear).LunarYearDays();
	}

	LunarSolarDateTime a(lunar.year);
	a.CalcLunarDaysInMonth();

	if(LunarLeapMonth() > 0 && lunar.month >= LunarLeapMonth()){
		lunar.month += 1;
	}

	for(int iMonth = 0; iMonth < lunar.month - 1; iMonth++){
		offset += a.lunarDaysInMonth[iMonth];
	}
	offset += lunar.day - 1;
	int ld = julianDayFromDate(baseDate.year,baseDate.month,baseDate.day);
	getDateFromJulianDay(ld + offset, &solar.year, &solar.month, &solar.day);

}

// ����תũ��
void LunarSolarDateTime::SolarToLunar(){
		int offset = SolarDaysFromBaseDate();

		int isets = sizeof(yearCode) / sizeof(yearCode[0]);
		for(int iYear = baseDate.year; iYear < baseDate.year + isets; iYear++){
			int tmp = LunarSolarDateTime(iYear).LunarYearDays();
			if(offset < tmp){
				lunar.year = iYear;
				break;
			}
			offset -= tmp;
		}
		
		for(int iMonth = 0; iMonth < 13; iMonth++){
			LunarSolarDateTime a(lunar.year, iMonth + 1);
			a.CalcLunarDaysInMonth();
			if(offset < a.lunarDaysInMonth[iMonth]){
				lunar.month = iMonth + 1;
				lunar.imonth = iMonth + 1;
				break;
			}
			offset -= a.lunarDaysInMonth[iMonth];
		}

		lunar.day = offset + 1;

		if(LunarLeapMonth() > 0 && lunar.month > LunarLeapMonth()){
			lunar.month -= 1;
		}
}

	// Returns which month is lunar leap month;
	// Returns 0 if no lunar leap month
unsigned char LunarSolarDateTime::LunarLeapMonth(){
	return yearCode[lunar.year - baseDate.year] & 0xf;
}


	// Calculates lunar days in month;
	// Stores data in a global list
void LunarSolarDateTime::CalcLunarDaysInMonth(){
	unsigned int code = yearCode[lunar.year - baseDate.year];
	code >>= 4;

	lunarDaysInMonth[12] = 0;

	for(int iMonth = 0; iMonth < 12; iMonth++){
		lunarDaysInMonth[11 - iMonth] = lunarMonthDay[code & 0x1];
		code >>= 1;
	}

	int lunarmonth = LunarLeapMonth();
	if(lunarmonth > 0){
		//insert
		//TODO
		for(int i = 12; i > lunarmonth; i--){
			lunarDaysInMonth[i] = lunarDaysInMonth[i - 1];
		}
		lunarDaysInMonth[lunarmonth] = lunarMonthDay[code & 0x1];
	}
}

// ����ũ���������
unsigned int LunarSolarDateTime::LunarYearDays(){
	CalcLunarDaysInMonth();
	lunarYearDays = 0;
	int monthNum = (LunarLeapMonth() == 0) ? 12 : 13;
	for(int m = 0; m < monthNum; m++){
		lunarYearDays += lunarDaysInMonth[m];
	}
	return lunarYearDays;
}

//ũ��������
CMzString LunarSolarDateTime::LunarMonth(){
	return LunarMonthName[lunar.month - 1];
}
		
		
//ũ��������
CMzString LunarSolarDateTime::LunarDay(){
	if(lunar.day == 1 && LunarLeapMonth() > 0){
		if( LunarLeapMonth() == lunar.month && lunar.imonth != lunar.month){
			CMzString retstr = L"��";
			return (retstr + LunarMonth());//LunarDayName[LunarLeapMonth() - 1]);
		}else{
			return LunarMonth();
		}
	}
	if( lunar.day == 1){
		return LunarMonth();
	}
	return LunarDayName[lunar.day - 1];
}

//�����������ũ��������
CMzString LunarSolarDateTime::OriginalLunarDay(){
	return LunarDayName[lunar.day - 1];
}


//��������
// 0 is Monday, 1 is TuesDay, ...
unsigned char LunarSolarDateTime::Weekday(){
	int offset = SolarDaysFromBaseDate();
	return (offset + baseDate.weekday) % 7 + 1;
}

//���ظ�֧��
CMzString LunarSolarDateTime::GanZhiYear(unsigned char *gan, unsigned char *zhi){
	int x = lunar.year % 60 - 4;
	int y = (x >= 0) ? x : x + 60;

	if(gan){
		*gan = y % 10;
	}
	if(zhi){
		*zhi = y % 12;
	}
	CMzString strgz = Gan[y % 10];
	strgz = strgz + Zhi[y % 12];
	return strgz;
}

//���ظ�֧��
CMzString LunarSolarDateTime::GanZhiMonth(unsigned char *gan, unsigned char *zhi){
	CMzString monthGanz;
	
	if(_monthGanzhiByJieqi){
		DWORD lmonth;
		lmonth = (solar.year - 1900 + 1) * 12 + solar.month - 1;
		if(solar.day >= GetFirstJieqiDay()){
			lmonth += 1;
		}
		monthGanz = Gan[lmonth % 10];
		monthGanz = monthGanz + Zhi[lmonth % 12];
	}else{
		unsigned char yearGan;
		GanZhiYear(&yearGan);
		monthGanz = Zhi[(2 + lunar.month - 1) % 12];

		if(yearGan == 0 || yearGan == 5){
			monthGanz = monthGanz + Gan[(2 + lunar.month - 1) % 10];
		}else if(yearGan == 1 || yearGan == 6){
			monthGanz = monthGanz + Gan[(4 + lunar.month - 1) % 10];
		}else if(yearGan == 2 || yearGan == 7){
			monthGanz = monthGanz + Gan[(6 + lunar.month - 1) % 10];
		}else if(yearGan == 3 || yearGan == 8){
			monthGanz = monthGanz + Gan[(8 + lunar.month - 1) % 10];
		}else if(yearGan == 4 || yearGan == 9){
			monthGanz = monthGanz + Gan[(0 + lunar.month - 1) % 10];
		}
	}
	return monthGanz;
}

	//���ظ�֧��
CMzString LunarSolarDateTime::GanZhiDay(unsigned char *gan, unsigned char *zhi){
	//1900��2��20��Ϊ������
	LunarSolarDateTime jiazi(1900,2,20);
	int offset = jiazi.SolarDaysFromBaseDate();
	int offsetNow = SolarDaysFromBaseDate();
	if(offsetNow < offset){
		offsetNow = 60 - ((offset - offsetNow) % 60);
	}else{
		offsetNow = (offsetNow - offset) % 60;	//60��һ������
	}
	DWORD g = offsetNow%10;
	DWORD z = offsetNow%12;

	if(gan){
		*gan = g;
	}
	if(zhi){
		*zhi = z;
	}

	CMzString strgz = Gan[g];
	strgz = strgz + Zhi[z];
	return strgz;
}

	//������Ф
CMzString LunarSolarDateTime::Zodiac(unsigned char *zodiac){
		unsigned char zhi;
		GanZhiYear(0,&zhi);
		if(zodiac){
			*zodiac = zhi;
		}
		return ZodiacName[zhi];
}
		
// Returns 24 solar terms in the solar year
LS24TERM_ptr LunarSolarDateTime::SolarTerm(void){
	unsigned char code = solarTermCode[(solar.year - baseDate.year) * 12 + solar.month - 1];
	terms[0].day = 15 - ((code>>4)&0xf);
	terms[1].day = (code&0xf) + 15;
	terms[0].name = solarTerm[(solar.month - 1) * 2];
	terms[1].name = solarTerm[(solar.month - 1) * 2 + 1];
	return terms;
}

//���ع�������
wchar_t* LunarSolarDateTime::SolarHoliday(){
	int nsets = sizeof(solarHoliday) / sizeof(solarHoliday[0]);
	wchar_t* s = 0;
	for(int i = 0; i < nsets; i++){
		if(solar.month == solarHoliday[i].month &&
			solar.day == solarHoliday[i].day){
				s = solarHoliday[i].name;
				break;
		}
	}
	return s;
}

//����ũ������
wchar_t* LunarSolarDateTime::LunarHoliday(){
	int nsets = sizeof(lunarHoliday) / sizeof(lunarHoliday[0]);
	wchar_t* s = 0;
	for(int i = 0; i < nsets; i++){
		if(lunar.month == lunarHoliday[i].month &&
			!(LunarLeapMonth() == lunar.month && lunar.month != lunar.imonth) &&	//������
			lunar.day == lunarHoliday[i].day){
				s = lunarHoliday[i].name;
				break;
		}
	}
	return s;
}
//==================����===============
char LunarSolarDateTime::CalConv2(int yy,int mm,int dd,int y,int d,int m,int dt,int nm,int nd) {
	int dy = dd > 10 ? d * 100 + dd : d * 10 + dd;
	if((yy==0 && dd==6)||
		(yy==6 && dd==0)||
		(yy==1 && dd==7)||
		(yy==7 && dd==1)||
		(yy==2 && dd==8)||
		(yy==8 && dd==2)||
		(yy==3 && dd==9)||
		(yy==9 && dd==3)||
		(yy==4 && dd==10)||
		(yy==10 && dd==4)||
		(yy==5 && dd==11)||
		(yy==11 && dd==5)) {
			return 0;
	}else if((mm==0 && dd==6)||
		(mm==6 && dd==0)||
		(mm==1 && dd==7)||
		(mm==7 && dd==1)||
		(mm==2 && dd==8)||
		(mm==8 && dd==2)||
		(mm==3 && dd==9)||
		(mm==9 && dd==3)||
		(mm==4 && dd==10)||
		(mm==10 && dd==4)||
		(mm==5 && dd==11)||
		(mm==11 && dd==5)) { 
			return 1;
	}else if((y==0 && dy==911)||
		(y==1 && dy==55)||
		(y==2 && dy==111)||
		(y==3 && dy==75)||
		(y==4 && dy==311)||
		(y==5 && dy==95)||
		(y==6 && dy==511)||
		(y==7 && dy==15)||
		(y==8 && dy==711)||
		(y==9 && dy==35)) {
			return 2;
	}else if((m==1 && dt==13)||
		(m==2 && dt==11)||
		(m==3 && dt==9)||
		(m==4 && dt==7)||
		(m==5 && dt==5)||
		(m==6 && dt==3)||
		(m==7 && dt==1)||
		(m==7 && dt==29)||
		(m==8 && dt==27)||
		(m==9 && dt==25)||
		(m==10 && dt==23)||
		(m==11 && dt==21)||
		(m==12 && dt==19)) {
			return 3;
	}else{
		return -1;
	}
}

bool LunarSolarDateTime::HuangliYiJi(CMzString &yi, CMzString &ji){
	bool ret = false;
	int ly,lm,ld;
	ly = solar.year - 1900 + 36;
	lm = (solar.year - 1900) * 12 + (solar.month - 1) + 12;	//13
	if(solar.day >= GetFirstJieqiDay()){
		lm += 1;
	}
	ld = diffDate(LSDate(solar.year,solar.month,1),LSDate(1900,1,1)) + 10 + solar.day - 1;
	unsigned char jc = JianCangs[lm%12][ld%12];
	char is = CalConv2(ly%12,lm%12,ld%12,ly%10,ld%10,lunar.month,lunar.day,solar.month,solar.day);
	if(is >= 0){
		ret = true;
		yi = huangli_ts[is];
	}else{
		yi = huangli_yi[jc];
		ji = huangli_ji[jc];
	}
	return ret;
}

DWORD LunarSolarDateTime::GetFirstJieqiDay(){
	unsigned char code = solarTermCode[(solar.year - baseDate.year) * 12 + solar.month - 1];
	return (15 - ((code>>4)&0xf));
}

//�����ڵõ�������
//from qt
unsigned int LunarSolarDateTime::julianDayFromDate(int year, int month, int day)
{
    if (year < 0)
        ++year;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
        // Gregorian calendar starting from October 15, 1582
        // Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
        return (1461 * (year + 4800 + (month - 14) / 12)) / 4
               + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
               - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
               + day - 32075;
    } else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        int a = (14 - month) / 12;
        return (153 * (month + (12 * a) - 3) + 2) / 5
               + (1461 * (year + 4800 - a)) / 4
               + day - 32083;
    } else {
        // the day following October 4, 1582 is October 15, 1582
        return 0;
    }
}

//�������յõ�����
void LunarSolarDateTime::getDateFromJulianDay(unsigned int julianDay, DWORD *year, DWORD *month, DWORD *day)
{
    int y, m, d;

    if (julianDay >= 2299161) {
        // Gregorian calendar starting from October 15, 1582
        // This algorithm is from Henry F. Fliegel and Thomas C. Van Flandern
        unsigned long ell, n, i, j;
        ell = (unsigned long)(julianDay) + 68569;
        n = (4 * ell) / 146097;
        ell = ell - (146097 * n + 3) / 4;
        i = (4000 * (ell + 1)) / 1461001;
        ell = ell - (1461 * i) / 4 + 31;
        j = (80 * ell) / 2447;
        d = ell - (2447 * j) / 80;
        ell = j / 11;
        m = j + 2 - (12 * ell);
        y = 100 * (n - 49) + i + ell;
    } else {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        julianDay += 32082;
        int dd = (4 * julianDay + 3) / 1461;
        int ee = julianDay - (1461 * dd) / 4;
        int mm = ((5 * ee) + 2) / 153;
        d = ee - (153 * mm + 2) / 5 + 1;
        m = mm + 3 - 12 * (mm / 10);
        y = dd - 4800 + (mm / 10);
        if (y <= 0)
            --y;
    }
    *year = y;
    *month = m;
    *day = d;
}
