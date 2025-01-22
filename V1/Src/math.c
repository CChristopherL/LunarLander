#include "math.h"

int16_t sinLUT(int16_t x) {
	// Initialize sin look up table
	static const int16_t SIN_LUT[512] = {0,201,402,603,804,1005,1205,1406,1606,1806,2005,2205,2404,2603,2801,2999,3196,3393,3590,3785,3981,4175,4370,4563,4756,4948,5139,5330,5519,5708,5896,6083,6270,6455,6639,6822,7005,7186,7366,7545,7723,7900,8075,8249,8423,8594,8765,8934,9102,9268,9433,9597,9759,9920,10079,10237,10393,10548,10701,10852,11002,11150,11297,11441,11585,11726,11865,12003,12139,12273,12405,12536,12664,12791,12915,13038,13159,13278,13394,13509,13622,13733,13841,13948,14052,14154,14255,14353,14449,14542,14634,14723,14810,14895,14977,15058,15136,15212,15285,15356,15425,15492,15556,15618,15678,15735,15790,15842,15892,15940,15985,16028,16068,16106,16142,16175,16206,16234,16260,16283,16304,16323,16339,16352,16363,16372,16378,16382,16383,16382,16378,16372,16363,16352,16339,16323,16304,16283,16260,16234,16206,16175,16142,16106,16068,16028,15985,15940,15892,15842,15790,15735,15678,15618,15556,15492,15425,15356,15285,15212,15136,15058,14977,14895,14810,14723,14634,14542,14449,14353,14255,14154,14052,13948,13841,13733,13622,13509,13394,13278,13159,13038,12915,12791,12664,12536,12405,12273,12139,12003,11865,11726,11585,11441,11297,11150,11002,10852,10701,10548,10393,10237,10079,9920,9759,9597,9433,9268,9102,8934,8765,8594,8423,8249,8075,7900,7723,7545,7366,7186,7005,6822,6639,6455,6270,6083,5896,5708,5519,5330,5139,4948,4756,4563,4370,4175,3981,3785,3590,3393,3196,2999,2801,2603,2404,2205,2005,1806,1606,1406,1205,1005,804,603,402,201,0,-201,-402,-603,-804,-1005,-1205,-1406,-1606,-1806,-2005,-2205,-2404,-2603,-2801,-2999,-3196,-3393,-3590,-3785,-3981,-4175,-4370,-4563,-4756,-4948,-5139,-5330,-5519,-5708,-5896,-6083,-6270,-6455,-6639,-6822,-7005,-7186,-7366,-7545,-7723,-7900,-8075,-8249,-8423,-8594,-8765,-8934,-9102,-9268,-9433,-9597,-9759,-9920,-10079,-10237,-10393,-10548,-10701,-10852,-11002,-11150,-11297,-11441,-11585,-11726,-11865,-12003,-12139,-12273,-12405,-12536,-12664,-12791,-12915,-13038,-13159,-13278,-13394,-13509,-13622,-13733,-13841,-13948,-14052,-14154,-14255,-14353,-14449,-14542,-14634,-14723,-14810,-14895,-14977,-15058,-15136,-15212,-15285,-15356,-15425,-15492,-15556,-15618,-15678,-15735,-15790,-15842,-15892,-15940,-15985,-16028,-16068,-16106,-16142,-16175,-16206,-16234,-16260,-16283,-16304,-16323,-16339,-16352,-16363,-16372,-16378,-16382,-16383,-16382,-16378,-16372,-16363,-16352,-16339,-16323,-16304,-16283,-16260,-16234,-16206,-16175,-16142,-16106,-16068,-16028,-15985,-15940,-15892,-15842,-15790,-15735,-15678,-15618,-15556,-15492,-15425,-15356,-15285,-15212,-15136,-15058,-14977,-14895,-14810,-14723,-14634,-14542,-14449,-14353,-14255,-14154,-14052,-13948,-13841,-13733,-13622,-13509,-13394,-13278,-13159,-13038,-12915,-12791,-12664,-12536,-12405,-12273,-12139,-12003,-11865,-11726,-11585,-11441,-11297,-11150,-11002,-10852,-10701,-10548,-10393,-10237,-10079,-9920,-9759,-9597,-9433,-9268,-9102,-8934,-8765,-8594,-8423,-8249,-8075,-7900,-7723,-7545,-7366,-7186,-7005,-6822,-6639,-6455,-6270,-6083,-5896,-5708,-5519,-5330,-5139,-4948,-4756,-4563,-4370,-4175,-3981,-3785,-3590,-3393,-3196,-2999,-2801,-2603,-2404,-2205,-2005,-1806,-1606,-1406,-1205,-1005,-804,-603,-402,-201};
	return (x < 0) ? SIN_LUT[(512 + (x & 511))] : SIN_LUT[x & 511];
}

int16_t cosLUT(int16_t x) {
	return sinLUT(x + 128);
}

void rotateVector(vector_t *v, int16_t theta) {
	v->x = v->x * cosLUT(theta) - v->y * sinLUT(theta);
	v->y = v->x * sinLUT(theta) + v->y * cosLUT(theta);
}
