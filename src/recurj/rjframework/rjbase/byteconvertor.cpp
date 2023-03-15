#include "stdafx.h"
#include <recurj/rjframework/libs/base/datatools/byteconvertor.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/libs/base/strings/byte/var.h>
using namespace rjBase;
bool CRJByteConvertor::s_bBigEndian=CRJByteConvertor::DefineByteOrder();
const uint8_t CRJByteConvertor::s_uBytes2Int64[256] {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};
const int64_t CRJByteConvertor::s_uPairChars[256]= {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	19,-1,-1,-1,-1,-1,-1, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,-1,-1,-1,-1,-1,-1, 30,
	31, 32, 33, 34, 35, 36, 37, 38, 39,-1,-1,-1,-1,-1,-1, 40, 41, 42, 43, 44, 45, 46,
	47, 48, 49,-1,-1,-1,-1,-1,-1, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,-1,-1,-1,-1,
	-1,-1, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,-1,-1,-1,-1,-1,-1, 70, 71, 72, 73, 74,
	75, 76, 77, 78, 79,-1,-1,-1,-1,-1,-1, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,-1,-1,
	-1,-1,-1,-1, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};
const int8_t CRJByteConvertor::s_lHexNumbersFlag[256]= {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0,
};
const uint8_t CRJByteConvertor::s_lByte2Digits[256]= {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7,
	8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};
const int64_t CRJByteConvertor::s_lTenDegrees[256]= {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50, 60, 70, 80,
	90, 0, 0, 0, 0, 0, 0, 0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 0, 0, 0, 0,
	0, 0, 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 0, 0, 0, 0, 0, 0, 0,
	10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 0, 0, 0, 0, 0, 0, 0,
	100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 0, 0, 0, 0,
	0, 0, 0, 1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000,
	0, 0, 0, 0, 0, 0, 0, 10000000, 20000000, 30000000, 40000000, 50000000, 60000000, 70000000,
	80000000, 90000000, 0, 0, 0, 0, 0, 0, 0, 100000000, 200000000, 300000000, 400000000,
	500000000, 600000000, 700000000, 800000000, 900000000, 0, 0, 0, 0, 0, 0, 0, 1000000000,
	2000000000, 3000000000, 4000000000, 5000000000, 6000000000, 7000000000, 8000000000,
	9000000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const int64_t CRJByteConvertor::s_lNumbers[256]= {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
const int64_t CRJByteConvertor::s_lPairChars[256]= {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 10, 11, 12, 13,
	14, 15, 16, 17, 18, 19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 20, 21, 22, 23, 24, 25,
	26, 27, 28, 29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 30, 31, 32, 33, 34, 35, 36, 37,
	38, 39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const char* CRJByteConvertor::s_uThreeDigitsToString[1024]= {
	"000","001","002","003","004","005","006","007","008","009","010","011","012","013",
	"014","015","016","017","018","019","020","021","022","023","024","025","026","027",
	"028","029","030","031","032","033","034","035","036","037","038","039","040","041",
	"042","043","044","045","046","047","048","049","050","051","052","053","054","055",
	"056","057","058","059","060","061","062","063","064","065","066","067","068","069",
	"070","071","072","073","074","075","076","077","078","079","080","081","082","083",
	"084","085","086","087","088","089","090","091","092","093","094","095","096","097",
	"098","099","100","101","102","103","104","105","106","107","108","109","110","111",
	"112","113","114","115","116","117","118","119","120","121","122","123","124","125",
	"126","127","128","129","130","131","132","133","134","135","136","137","138","139",
	"140","141","142","143","144","145","146","147","148","149","150","151","152","153",
	"154","155","156","157","158","159","160","161","162","163","164","165","166","167",
	"168","169","170","171","172","173","174","175","176","177","178","179","180","181",
	"182","183","184","185","186","187","188","189","190","191","192","193","194","195",
	"196","197","198","199","200","201","202","203","204","205","206","207","208","209",
	"210","211","212","213","214","215","216","217","218","219","220","221","222","223",
	"224","225","226","227","228","229","230","231","232","233","234","235","236","237",
	"238","239","240","241","242","243","244","245","246","247","248","249","250","251",
	"252","253","254","255","256","257","258","259","260","261","262","263","264","265",
	"266","267","268","269","270","271","272","273","274","275","276","277","278","279",
	"280","281","282","283","284","285","286","287","288","289","290","291","292","293",
	"294","295","296","297","298","299","300","301","302","303","304","305","306","307",
	"308","309","310","311","312","313","314","315","316","317","318","319","320","321",
	"322","323","324","325","326","327","328","329","330","331","332","333","334","335",
	"336","337","338","339","340","341","342","343","344","345","346","347","348","349",
	"350","351","352","353","354","355","356","357","358","359","360","361","362","363",
	"364","365","366","367","368","369","370","371","372","373","374","375","376","377",
	"378","379","380","381","382","383","384","385","386","387","388","389","390","391",
	"392","393","394","395","396","397","398","399","400","401","402","403","404","405",
	"406","407","408","409","410","411","412","413","414","415","416","417","418","419",
	"420","421","422","423","424","425","426","427","428","429","430","431","432","433",
	"434","435","436","437","438","439","440","441","442","443","444","445","446","447",
	"448","449","450","451","452","453","454","455","456","457","458","459","460","461",
	"462","463","464","465","466","467","468","469","470","471","472","473","474","475",
	"476","477","478","479","480","481","482","483","484","485","486","487","488","489",
	"490","491","492","493","494","495","496","497","498","499","500","501","502","503",
	"504","505","506","507","508","509","510","511","512","513","514","515","516","517",
	"518","519","520","521","522","523","524","525","526","527","528","529","530","531",
	"532","533","534","535","536","537","538","539","540","541","542","543","544","545",
	"546","547","548","549","550","551","552","553","554","555","556","557","558","559",
	"560","561","562","563","564","565","566","567","568","569","570","571","572","573",
	"574","575","576","577","578","579","580","581","582","583","584","585","586","587",
	"588","589","590","591","592","593","594","595","596","597","598","599","600","601",
	"602","603","604","605","606","607","608","609","610","611","612","613","614","615",
	"616","617","618","619","620","621","622","623","624","625","626","627","628","629",
	"630","631","632","633","634","635","636","637","638","639","640","641","642","643",
	"644","645","646","647","648","649","650","651","652","653","654","655","656","657",
	"658","659","660","661","662","663","664","665","666","667","668","669","670","671",
	"672","673","674","675","676","677","678","679","680","681","682","683","684","685",
	"686","687","688","689","690","691","692","693","694","695","696","697","698","699",
	"700","701","702","703","704","705","706","707","708","709","710","711","712","713",
	"714","715","716","717","718","719","720","721","722","723","724","725","726","727",
	"728","729","730","731","732","733","734","735","736","737","738","739","740","741",
	"742","743","744","745","746","747","748","749","750","751","752","753","754","755",
	"756","757","758","759","760","761","762","763","764","765","766","767","768","769",
	"770","771","772","773","774","775","776","777","778","779","780","781","782","783",
	"784","785","786","787","788","789","790","791","792","793","794","795","796","797",
	"798","799","800","801","802","803","804","805","806","807","808","809","810","811",
	"812","813","814","815","816","817","818","819","820","821","822","823","824","825",
	"826","827","828","829","830","831","832","833","834","835","836","837","838","839",
	"840","841","842","843","844","845","846","847","848","849","850","851","852","853",
	"854","855","856","857","858","859","860","861","862","863","864","865","866","867",
	"868","869","870","871","872","873","874","875","876","877","878","879","880","881",
	"882","883","884","885","886","887","888","889","890","891","892","893","894","895",
	"896","897","898","899","900","901","902","903","904","905","906","907","908","909",
	"910","911","912","913","914","915","916","917","918","919","920","921","922","923",
	"924","925","926","927","928","929","930","931","932","933","934","935","936","937",
	"938","939","940","941","942","943","944","945","946","947","948","949","950","951",
	"952","953","954","955","956","957","958","959","960","961","962","963","964","965",
	"966","967","968","969","970","971","972","973","974","975","976","977","978","979",
	"980","981","982","983","984","985","986","987","988","989","990","991","992","993",
	"994","995","996","997","998","999","000","001","002","003","004","005","006","007",
	"008","009","010","011","012","013","014","015","016","017","018","019","020","021",
	"022","023"
};
const char* CRJByteConvertor::s_uPairDigitsToString[128]= {
	"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16",
	"17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33",
	"34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50",
	"51","52","53","54","55","56","57","58","59","60","61","62","63","64","65","66","67",
	"68","69","70","71","72","73","74","75","76","77","78","79","80","81","82","83","84",
	"85","86","87","88","89","90","91","92","93","94","95","96","97","98","99","00","00",
	"00","00","00","00","00","00","00","00","00","00","00","00","00","00","00","00","00",
	"00","00","00","00","00","00","00","00","00"
};
#define _base64DecodeTriple\
 pDst[0] = (uint8_t)((pSrc[0] << 2) | (pSrc[1] >> 4));\
 pDst[1] = (uint8_t)((pSrc[1] << 4) | (pSrc[2] >> 2));\
 pDst[2] = (uint8_t)((pSrc[2] << 6) | pSrc[3])
 const char CRJByteConvertor::s_Base64Encode[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const uint8_t CRJByteConvertor::s_Base64Decode[128]= {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,
	0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
	0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
	0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00,
	0x00, 0x00,
};
const size_t CRJByteConvertor::s_Base64TailAdd[3]= {
	0, 1, 1
};
size_t CRJByteConvertor::ByteDigit2Size(uint8_t i) {
	return CRJByteConvertor::s_lByte2Digits[i];//	                case '=':i=0x0D;break;
}
int64_t CRJByteConvertor::ByteValue2Int(uint8_t b) {
	size_t s=b;
	s&=0xFF;
	return (int64_t) s;
}
void CRJByteConvertor::Byte2PairHex(uint8_t* pDst, uint8_t bInd) {
	*((uint16_t*) pDst)=*((uint16_t*) CRJBytePacker::s_clChars[bInd]);
}
uint8_t CRJByteConvertor::PairHex2Byte(uint8_t ch1, uint8_t ch2) {
	return ((CRJByteConvertor::s_lByte2Digits[ch1]<<4)& 0xF0)+CRJByteConvertor::s_lByte2Digits[ch2];
}
uint16_t CRJByteConvertor::BigEndianSet16(uint16_t v) {
	return CRJByteConvertor::IsBigEndian()?
		v:BigEndianReverse16(v);
}
uint32_t CRJByteConvertor::BigEndianSet32(uint32_t v) {
	return CRJByteConvertor::IsBigEndian()?
		v:BigEndianReverse32(v);
}
uint64_t CRJByteConvertor::BigEndianSet64(uint64_t v) {
	return CRJByteConvertor::IsBigEndian()?
		v:BigEndianReverse64(v);
}
uint16_t CRJByteConvertor::BigEndianReverse16(uint16_t v) {
	return ((v>>8)& 0x00FF)|((v<<8)& 0xFF00);
}
uint32_t CRJByteConvertor::BigEndianReverse32(uint32_t v) {
	return ((v>>24)& 0x000000FF)|((v>>8)& 0x0000FF00)|((v<<8)& 0x00FF0000)|((v<<24)& 0xFF000000);
}
uint64_t CRJByteConvertor::BigEndianReverse64(uint64_t v) {
	return ((v>>56)& 0x00000000000000FFLL)|((v>>40)& 0x000000000000FF00LL)|((v>>24)& 0x0000000000FF0000LL)|((v>>8)& 0x00000000FF000000LL)|((v<<8)& 0x000000FF00000000LL)|((v<<24)& 0x0000FF0000000000LL)|((v<<40)& 0x00FF000000000000LL)|((v<<56)& 0xFF00000000000000LL);
}
bool CRJByteConvertor::DefineByteOrder() {
	uint16_t w;
	uint8_t* p=(uint8_t*)& w;
	p[0]=0x12;
	p[1]=0x34;
	return w==0x1234;
}
bool CRJByteConvertor::IsDecDigit(uint8_t ch) {
	return CRJByteConvertor::s_lHexNumbersFlag[ch]==1;
}
bool CRJByteConvertor::IsHexDigit(uint8_t ch) {
	return CRJByteConvertor::s_lHexNumbersFlag[ch]>0;
}
size_t CRJByteConvertor::SizeToBytes(size_t v, uint8_t* pOut, size_t lAvailable) {
#ifdef _DEBUG
	if (pOut==nullptr) CRJDataTools::DebugBreak();
#endif 
	int rc=snprintf((char*) pOut, lAvailable,"%zu", v);
	if (rc>=0&&
		rc<lAvailable) return rc;
	return 0;
}
size_t CRJByteConvertor::HexToBytes(size_t v, uint8_t* pOut, size_t lAvailable) {
	int rc=snprintf((char*) pOut, lAvailable,"%zx", v);
	if (rc>=0&&
		rc<lAvailable) return rc;
	return 0;
}
size_t CRJByteConvertor::Int64ToBytes(int64_t v, uint8_t* pOut, size_t lAvailable) {
	if (v<0) {
		if (lAvailable<1) return 0;
		pOut[0]='-';
		return 1+SizeToBytes((size_t)(-v), pOut+1, lAvailable-1)+1;
	}
	return SizeToBytes((size_t) v, pOut, lAvailable);
}
size_t CRJByteConvertor::SizeToUnicodes(size_t v,wchar_t* pOut, size_t lAvailable) {
#ifdef _DEBUG
	if (pOut==nullptr) CRJDataTools::DebugBreak();
#endif 
	int rc=swprintf(pOut, lAvailable,L"%zu", v);
	if (rc>=0&&
		(size_t) rc<lAvailable) return rc;
	return 0;
}
size_t CRJByteConvertor::HexToUnicodes(size_t v,wchar_t* pOut, size_t lAvailable) {
	int rc=swprintf(pOut, lAvailable,L"%zx", v);
	if (rc>=0&&
		(size_t) rc<lAvailable) return rc;
	return 0;
}
size_t CRJByteConvertor::Int64ToUnicodes(int64_t v,wchar_t* pOut, size_t lAvailable) {
	if (v<0) {
		if (lAvailable<1) return 0;
		pOut[0]=L'-';
		return 1+SizeToUnicodes((size_t)(-v), pOut+1, lAvailable-1);
	}
	return SizeToUnicodes((size_t) v, pOut, lAvailable);
}
bool CRJByteConvertor::Bytes2Int64(const uint8_t* pSrc, size_t lLen, int64_t& rc) {
	size_t lInd;
	rc=0;
	while (lLen>0&&
		(pSrc[0]==0x09||
		pSrc[0]==0x20)) {
		pSrc++;
		lLen--;
	}
	if (lLen>0) {
		uint8_t chSign=pSrc[0];
		if (chSign=='-'||
			chSign=='+') {
			pSrc++;
			lLen--;
		}
		while (lLen>0) {
			lInd=CRJByteConvertor::s_uBytes2Int64[pSrc[0]];
			if (!lInd) return false;
			pSrc++;
			lLen--;
			rc=(int64_t)((((rc<<2)+rc)<<1)+(lInd-0x30));
		}
		if (chSign=='-') rc=-rc;
	}
	return true;
}
bool CRJByteConvertor::Bytes2Size(const uint8_t* pSrc, size_t lLen, size_t& rc) {
	size_t lInd;
	rc=0;
	while (lLen>0&&
		(pSrc[0]==0x09||
		pSrc[0]==0x20)) {
		pSrc++;
		lLen--;
	}
	if (lLen>0) {
		while (lLen>0) {
			lInd=CRJByteConvertor::s_uBytes2Int64[pSrc[0]];
			if (!lInd) return false;
			pSrc++;
			lLen--;
			rc=(((rc<<2)+rc)<<1)+(lInd-0x30);
		}
	}
	return true;
}
int64_t CRJByteConvertor::PairChars2NumSafed(const uint8_t* pSrc) {
	uint16_t w=*((uint16_t*) pSrc);
	w-=0x3030;
	w=(uint16_t)(((w>>8)& 0x0F)+((w<<4)& 0xF0));
	return CRJByteConvertor::s_uPairChars[w];
}
int64_t CRJByteConvertor::HexPair2IntSafedCheck(const uint8_t* p, int64_t lerr) {
#ifdef _DEBUG
	if (!p) CRJDataTools::DebugBreak();
#endif 
#pragma warning(suppress: 6011)
	if (CRJByteConvertor::s_lHexNumbersFlag[p[0]]& CRJByteConvertor::s_lHexNumbersFlag[p[1]]) {
		return (int64_t)(size_t) PairHex2Byte(p[0], p[1]);
	}
	return lerr;
}
#pragma warning(suppress: 26430)
int64_t CRJByteConvertor::HexStr2IntSafed(const uint8_t* p, size_t lLen) {
#ifdef _DEBUG
	if (!p) CRJDataTools::DebugBreak();
#endif 
#pragma warning(suppress: 6011)
	int64_t rc=CRJByteConvertor::s_lByte2Digits[p[0]];
	while (lLen>1) {
		lLen--;
		p++;
		rc=((rc<<4)& 0xFFFFFFFFFFFFFFF0);
		rc+=CRJByteConvertor::s_lByte2Digits[p[0]];
	}
	return rc;
}
int64_t CRJByteConvertor::DecStr2IntSafed(const uint8_t* pSrc, size_t lLen) {
#ifdef _DEBUG
	if (!pSrc) CRJDataTools::DebugBreak();
#endif 
#pragma warning(suppress: 6011)
	int64_t rc=0;
	const int64_t* pMultiply=CRJByteConvertor::s_lTenDegrees;//	                таблица умножения
	lLen&=0x0F;//	                ограничили по простому
	pSrc+=lLen;//	                за последним символом
	while (lLen>0) {
		lLen--;
		pSrc--;
		rc+=pMultiply[CRJByteConvertor::s_lNumbers[* pSrc]];
		pMultiply+=16;
	}
	return rc;
}
int64_t CRJByteConvertor::DecPair2IntSafedNoCheck(const uint8_t* p) {
#ifdef _DEBUG
	if (!p) CRJDataTools::DebugBreak();
#endif 
#pragma warning(suppress: 6011)
	uint16_t w=*((uint16_t*) p);
	w-=0x3030;
	w=(uint16_t)(((w>>8)& 0x0F)+((w<<4)& 0xF0));
	return CRJByteConvertor::s_lPairChars[w];
}
int64_t CRJByteConvertor::DecPair2IntSafedCheck(const uint8_t* p, int64_t lerr) {
#ifdef _DEBUG
	if (!p) CRJDataTools::DebugBreak();
#endif 
#pragma warning(suppress: 6011)
	if ((CRJByteConvertor::s_lHexNumbersFlag[p[0]]& CRJByteConvertor::s_lHexNumbersFlag[p[1]])==1) {
		uint16_t w=*((uint16_t*) p);
		w-=0x3030;
		w=(uint16_t)(((w>>8)& 0x0F)+((w<<4)& 0xF0));
		return CRJByteConvertor::s_lPairChars[w];
	}
	return lerr;
}
int64_t CRJByteConvertor::DecThree2IntSafedCheck(const uint8_t* p, int64_t lerr) {
#ifdef _DEBUG
	if (!p) CRJDataTools::DebugBreak();
#endif 
#pragma warning(suppress: 6011)
	if ((CRJByteConvertor::s_lHexNumbersFlag[p[0]]& CRJByteConvertor::s_lHexNumbersFlag[p[1]]& CRJByteConvertor::s_lHexNumbersFlag[p[2]])==1) {
		const int64_t* pMultiply=CRJByteConvertor::s_lTenDegrees;//	                таблица умножения
		int64_t rc=pMultiply[CRJByteConvertor::s_lNumbers[p[2]]];//	                третий символ
		pMultiply+=16;
		rc+=pMultiply[CRJByteConvertor::s_lNumbers[p[1]]];//	                второй символ
		pMultiply+=16;
		return rc+pMultiply[CRJByteConvertor::s_lNumbers[p[0]]];//	                первый символ
	}
	return lerr;
}
bool CRJByteConvertor::Base64Encode(const CRJStringByte& clSrc, CRJStringByteVar& clDst) {
	const uint8_t* pSrc=clSrc.GetHeadForRead();
	uint8_t* pDst=clDst.GetHeadForUpdate();
	size_t lSrcLen=clSrc.GetLength();
	size_t m=lSrcLen/ 3, k=lSrcLen% 3;
	if (!clDst.SetLength((m+CRJByteConvertor::s_Base64TailAdd[k])<<2).OK()) return false;
	while (m>0) {
		m--;
		pDst[0]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[0]>>2)& 0x03F)];
		pDst[1]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[0]<<4)& 0x30)+((pSrc[1]>>4)& 0x0F)];
		pDst[2]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[1]<<2)& 0x3C)+((pSrc[2]>>6)& 0x03)];
		pDst[3]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)(pSrc[2]& 0x3F)];
		pSrc+=3;
		pDst+=4;
	}
	if (k>0) {
		if (k==1) {
			pDst[0]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[0]>>2)& 0x03F)];
			pDst[1]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[0]<<4)& 0x30)];
			pDst[2]='=';
		}
		else {
			pDst[0]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[0]>>2)& 0x03F)];
			pDst[1]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[0]<<4)& 0x30)+((pSrc[1]>>4)& 0x0F)];
			pDst[2]=(uint8_t) CRJByteConvertor::s_Base64Encode[(uint32_t)((pSrc[1]<<2)& 0x3C)];
		}
		pDst[3]='=';
	}
	return true;
}
bool CRJByteConvertor::Base64DecodeInPlace(CRJStringByteVar& clDst) {
	uint8_t* pBuf=clDst.GetHeadForUpdate();
	size_t lLen=clDst.GetLength();
	uint8_t ch;
	uint8_t pSrc[4];
	size_t j, i=0, lRes=0;
	uint8_t* pDst=pBuf;
	while (lLen>0) {
		ch=* pBuf;
		lLen--;
		pBuf++;
		if (ch=='=')break;
		pSrc[i]=(uint8_t)(ch& 0x7F);
		i++;
		if (i==4) {
			for (j=0; j<4; j++) pSrc[j]=CRJByteConvertor::s_Base64Decode[pSrc[j]];
			_base64DecodeTriple;
			pDst+=3;
			lRes+=3;
			i=0;
		}
	}
	if (i>1) {
		for (j=i; j<4; j++) pSrc[j]=0;
		for (j=0; j<i; j++) pSrc[j]=CRJByteConvertor::s_Base64Decode[pSrc[j]];
		i--;
		_base64DecodeTriple;
		lRes+=i;
	}
	return clDst.SetLength(lRes).OK();
}
#define _Base94_MUL(__val) __val=((__val<<6)&0xFFFFFFC0)+((__val<<5)&0xFFFFFFE0)-((__val<<1)&0xFFFFFFFE)
size_t CRJByteConvertor::Base94Encode(const uint8_t* pSrc, size_t lSrcLen,char* pDst, size_t lDstLen) {
	size_t rc=lSrcLen+((lSrcLen>>2)& 0x3FFFFFFF);
	if ((lSrcLen& 3)||
		(lDstLen<rc)) return 0;
	uint32_t v;
	while (lSrcLen>3) {
		v=*((uint32_t*) pSrc);
		pSrc+=4;
		lSrcLen-=4;
		pDst[0]=(char)((v% 94)+32);
		v/=94;
		pDst++;
		pDst[0]=(char)((v% 94)+32);
		v/=94;
		pDst++;
		pDst[0]=(char)((v% 94)+32);
		v/=94;
		pDst++;
		pDst[0]=(char)((v% 94)+32);
		v/=94;
		pDst++;
		pDst[0]=(char)(v+32);
		pDst++;
	}
	return rc;
}
size_t CRJByteConvertor::Base94DecodeInPlace(uint8_t* pSrc, size_t lLen) {
	if (lLen% 5) return 0;
	int32_t rc=(int32_t)(((lLen/ 5)<<2)& 0xFFFFFFFC);
	uint32_t v;
	uint8_t* pDst=pSrc;
	while (lLen>4) {
		lLen-=5;
		v=(uint32_t)(pSrc[4]-32);
		v=_Base94_MUL(v)+pSrc[3]-32;
		v=_Base94_MUL(v)+pSrc[2]-32;
		v=_Base94_MUL(v)+pSrc[1]-32;
		v=_Base94_MUL(v)+pSrc[0]-32;
		pSrc+=5;
		*((uint32_t*) pDst)=v;
		pDst+=4;
	}
	return rc;
}
const uint8_t* CRJByteConvertor::Int2PairDecDigits(size_t s) {
	return (const uint8_t*) s_uPairDigitsToString[s& 0x7F];
}
const uint8_t* CRJByteConvertor::Int2ThreeDecDigits(size_t ind) {
	return (const uint8_t*) CRJByteConvertor::s_uThreeDigitsToString[ind& 0x3FF];
}

