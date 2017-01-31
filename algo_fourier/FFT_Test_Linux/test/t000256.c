#include "FFTTest.h"
static const complex ffttest_256_in[256] = {
	{ 2768,    	0 },
	{ 4840,    	0 },
	{ 3472,    	0 },
	{ 1200,    	0 },
	{ 3360,    	0 },
	{ 1568,    	0 },
	{ -2200,    	0 },
	{ -2464,    	0 },
	{ -3360,    	0 },
	{ -7056,    	0 },
	{ -6800,    	0 },
	{ -6688,    	0 },
	{ -7960,    	0 },
	{ -5072,    	0 },
	{ -4272,    	0 },
	{ -5400,    	0 },
	{ -184,    	0 },
	{ 3896,    	0 },
	{ 3320,    	0 },
	{ 3568,    	0 },
	{ 5248,    	0 },
	{ 4440,    	0 },
	{ 1680,    	0 },
	{ -880,    	0 },
	{ -2288,    	0 },
	{ 576,    	0 },
	{ 2576,    	0 },
	{ -3048,    	0 },
	{ -6552,    	0 },
	{ 976,    	0 },
	{ 5384,    	0 },
	{ -320,    	0 },
	{ -2552,    	0 },
	{ 2856,    	0 },
	{ 3528,    	0 },
	{ 984,    	0 },
	{ 552,    	0 },
	{ 1928,    	0 },
	{ 2984,    	0 },
	{ 2160,    	0 },
	{ 728,    	0 },
	{ 1968,    	0 },
	{ 2184,    	0 },
	{ -608,    	0 },
	{ -1304,    	0 },
	{ -1464,    	0 },
	{ -1424,    	0 },
	{ 280,    	0 },
	{ 528,    	0 },
	{ -1344,    	0 },
	{ -1368,    	0 },
	{ 304,    	0 },
	{ 1504,    	0 },
	{ 112,    	0 },
	{ -1984,    	0 },
	{ -1448,    	0 },
	{ 1160,    	0 },
	{ -72,    	0 },
	{ -3856,    	0 },
	{ -3216,    	0 },
	{ -1248,    	0 },
	{ -1464,    	0 },
	{ -2256,    	0 },
	{ -1912,    	0 },
	{ -808,    	0 },
	{ -1112,    	0 },
	{ -3864,    	0 },
	{ -4368,    	0 },
	{ -808,    	0 },
	{ 1992,    	0 },
	{ 3656,    	0 },
	{ 5880,    	0 },
	{ 10304,    	0 },
	{ 8560,    	0 },
	{ 5184,    	0 },
	{ 4104,    	0 },
	{ -592,    	0 },
	{ -632,    	0 },
	{ -640,    	0 },
	{ -7160,    	0 },
	{ -8712,    	0 },
	{ 3824,    	0 },
	{ 2536,    	0 },
	{ -5024,    	0 },
	{ 976,    	0 },
	{ 6144,    	0 },
	{ 4168,    	0 },
	{ 3504,    	0 },
	{ 3424,    	0 },
	{ 3832,    	0 },
	{ 5120,    	0 },
	{ 616,    	0 },
	{ -1872,    	0 },
	{ 2344,    	0 },
	{ 1208,    	0 },
	{ -2352,    	0 },
	{ -2992,    	0 },
	{ -6216,    	0 },
	{ -8240,    	0 },
	{ -6560,    	0 },
	{ -5896,    	0 },
	{ -5512,    	0 },
	{ -3416,    	0 },
	{ -496,    	0 },
	{ 3064,    	0 },
	{ 3280,    	0 },
	{ 1952,    	0 },
	{ 2064,    	0 },
	{ 3336,    	0 },
	{ 1440,    	0 },
	{ -2096,    	0 },
	{ -3632,    	0 },
	{ -2960,    	0 },
	{ -1752,    	0 },
	{ -2352,    	0 },
	{ -4448,    	0 },
	{ -3168,    	0 },
	{ -1512,    	0 },
	{ -2760,    	0 },
	{ -3968,    	0 },
	{ -880,    	0 },
	{ 1976,    	0 },
	{ 512,    	0 },
	{ -896,    	0 },
	{ 4488,    	0 },
	{ 6144,    	0 },
	{ 256,    	0 },
	{ -496,    	0 },
	{ -216,    	0 },
	{ 4168,    	0 },
	{ 4696,    	0 },
	{ -2640,    	0 },
	{ -6328,    	0 },
	{ 4376,    	0 },
	{ 6288,    	0 },
	{ -5496,    	0 },
	{ -2712,    	0 },
	{ 5944,    	0 },
	{ 3960,    	0 },
	{ -560,    	0 },
	{ 328,    	0 },
	{ 4768,    	0 },
	{ 6984,    	0 },
	{ 4456,    	0 },
	{ 1184,    	0 },
	{ 3256,    	0 },
	{ 4072,    	0 },
	{ -1176,    	0 },
	{ -3896,    	0 },
	{ -6184,    	0 },
	{ -7768,    	0 },
	{ -5288,    	0 },
	{ -2544,    	0 },
	{ -2680,    	0 },
	{ -1648,    	0 },
	{ -24,    	0 },
	{ 3368,    	0 },
	{ 5688,    	0 },
	{ 5640,    	0 },
	{ 5200,    	0 },
	{ 4472,    	0 },
	{ 2240,    	0 },
	{ -520,    	0 },
	{ 48,    	0 },
	{ -864,    	0 },
	{ -2448,    	0 },
	{ -3792,    	0 },
	{ -7144,    	0 },
	{ -7256,    	0 },
	{ -5704,    	0 },
	{ -5744,    	0 },
	{ -6912,    	0 },
	{ -4624,    	0 },
	{ -88,    	0 },
	{ 472,    	0 },
	{ -1424,    	0 },
	{ 1960,    	0 },
	{ 10480,    	0 },
	{ 5912,    	0 },
	{ -2000,    	0 },
	{ -1112,    	0 },
	{ 3176,    	0 },
	{ 7704,    	0 },
	{ 4296,    	0 },
	{ -3800,    	0 },
	{ -2400,    	0 },
	{ 8112,    	0 },
	{ 1264,    	0 },
	{ -7216,    	0 },
	{ 1872,    	0 },
	{ 6368,    	0 },
	{ 528,    	0 },
	{ -720,    	0 },
	{ 3896,    	0 },
	{ 7104,    	0 },
	{ 6912,    	0 },
	{ 3912,    	0 },
	{ 2776,    	0 },
	{ 5832,    	0 },
	{ 3640,    	0 },
	{ 16,    	0 },
	{ -528,    	0 },
	{ -2480,    	0 },
	{ -4736,    	0 },
	{ -4424,    	0 },
	{ -3664,    	0 },
	{ -4832,    	0 },
	{ -4896,    	0 },
	{ -4480,    	0 },
	{ -2816,    	0 },
	{ -432,    	0 },
	{ -592,    	0 },
	{ -2048,    	0 },
	{ -2376,    	0 },
	{ -4456,    	0 },
	{ -6344,    	0 },
	{ -6024,    	0 },
	{ -6816,    	0 },
	{ -8584,    	0 },
	{ -8832,    	0 },
	{ -7520,    	0 },
	{ -5288,    	0 },
	{ -2232,    	0 },
	{ -424,    	0 },
	{ -104,    	0 },
	{ 2928,    	0 },
	{ 5968,    	0 },
	{ 4304,    	0 },
	{ 1648,    	0 },
	{ 4400,    	0 },
	{ 9952,    	0 },
	{ 2536,    	0 },
	{ -3160,    	0 },
	{ 608,    	0 },
	{ 5024,    	0 },
	{ 7120,    	0 },
	{ 4600,    	0 },
	{ 1144,    	0 },
	{ 4328,    	0 },
	{ 11808,    	0 },
	{ 2520,    	0 },
	{ -9192,    	0 },
	{ -3336,    	0 },
	{ 344,    	0 },
	{ -7096,    	0 },
	{ -8208,    	0 },
	{ -3192,    	0 },
	{ 480,    	0 },
	{ 1744,    	0 },
	{ 3200,    	0 },
	{ 5288,    	0 },
	{ 8704,    	0 },
	{ 8216,    	0 },
	{ 7016,    	0 },
	{ 6736,    	0 },
	{ 3512,    	0 }
};
static const complex ffttest_256_out[256] ={
	{ 17720.00000,    	0.00000 },
	{ 22213.34766,    	-8748.16211 },
	{ 28306.76172,    	-29773.97656 },
	{ 62009.32812,    	49705.67188 },
	{ 103811.82031,    	42351.71094 },
	{ -130162.39844,    	215829.32812 },
	{ -61850.04688,    	47537.94922 },
	{ 44833.42969,    	-87710.11719 },
	{ 100249.78125,    	-42511.27344 },
	{ 83996.42188,    	43696.37891 },
	{ 139451.23438,    	14371.00879 },
	{ 150159.00000,    	152210.73438 },
	{ 66620.93750,    	-2205.30444 },
	{ 93153.70312,    	27476.39062 },
	{ 127691.89844,    	259417.00000 },
	{ -25144.14062,    	-258579.71875 },
	{ 39695.93750,    	-3345.06982 },
	{ 59178.77344,    	52555.16406 },
	{ 27473.27344,    	-6248.95264 },
	{ -99516.96875,    	82621.88281 },
	{ -128946.08594,    	18644.18359 },
	{ -53008.75781,    	25714.46680 },
	{ -63820.07422,    	-68658.87500 },
	{ -81376.82031,    	10466.37598 },
	{ -18101.00195,    	55803.94531 },
	{ 37760.36328,    	-79229.14844 },
	{ -28127.30664,    	8088.24365 },
	{ -2846.27856,    	-11476.91016 },
	{ 21709.51172,    	-18528.06250 },
	{ 21480.80078,    	-6997.79004 },
	{ -22509.61719,    	-4733.89746 },
	{ -9079.20508,    	20188.29102 },
	{ 20853.10156,    	1929.09668 },
	{ -22065.19922,    	-9785.72266 },
	{ -33305.92969,    	-14812.79590 },
	{ -6181.61816,    	18242.14453 },
	{ -16333.69727,    	-9389.22656 },
	{ -14279.60254,    	8235.44922 },
	{ 826.89258,    	-19296.94141 },
	{ 12559.11523,    	6030.09180 },
	{ 4494.73047,    	27837.21289 },
	{ 981.61133,    	-19020.98828 },
	{ 10474.68262,    	6512.10254 },
	{ -24046.99023,    	24051.33398 },
	{ -3536.22852,    	-60988.54688 },
	{ 18998.73828,    	-16751.18750 },
	{ -10753.18555,    	1607.05566 },
	{ -5473.99316,    	11806.83789 },
	{ -11930.70508,    	-5473.50977 },
	{ 23322.97852,    	32515.99609 },
	{ 12464.35352,    	11326.15039 },
	{ -1687.79688,    	-4439.20166 },
	{ 3968.07080,    	2645.72021 },
	{ -74005.00000,    	29061.78125 },
	{ -3806.23950,    	-74393.80469 },
	{ -35827.76172,    	-36042.15625 },
	{ -20822.18164,    	-15103.65430 },
	{ -35176.21094,    	-21469.94141 },
	{ 30580.63672,    	-94612.95312 },
	{ 142764.87500,    	140034.34375 },
	{ 50220.59375,    	-32500.72461 },
	{ 44931.66406,    	-18716.84375 },
	{ 49082.57031,    	-7483.09375 },
	{ 36738.72266,    	130771.36719 },
	{ -118488.00000,    	-77088.00000 },
	{ -30270.12305,    	25984.26367 },
	{ -17049.02734,    	19122.55469 },
	{ -24862.25781,    	44070.99219 },
	{ -55926.85156,    	1485.79858 },
	{ 59223.70312,    	-38561.91406 },
	{ -22331.33789,    	-31930.32031 },
	{ -18525.41406,    	-9182.69922 },
	{ -20440.64648,    	-7156.10254 },
	{ -13890.99609,    	-61122.18750 },
	{ -7779.92285,    	14774.27930 },
	{ 11470.86523,    	-6563.73438 },
	{ 16526.95117,    	-27696.07812 },
	{ 42157.32422,    	-41978.82031 },
	{ 36912.18750,    	49166.64062 },
	{ 18260.02734,    	-35988.01562 },
	{ -9003.18164,    	-11478.96680 },
	{ -6969.50098,    	17782.62891 },
	{ -21254.24023,    	41745.02344 },
	{ -71049.81250,    	-58631.54297 },
	{ -27331.35742,    	34228.51562 },
	{ 6807.50635,    	10769.62695 },
	{ -27035.23242,    	-1310.76953 },
	{ -52793.03516,    	-24487.00000 },
	{ 40448.58203,    	-13859.21289 },
	{ -23762.51953,    	-11054.74512 },
	{ -8847.98828,    	10271.50195 },
	{ 14095.14453,    	-385.67981 },
	{ 8356.79590,    	414.34424 },
	{ -5402.36426,    	4696.02881 },
	{ 6398.70117,    	1678.46899 },
	{ 1617.35254,    	-4282.82080 },
	{ 1370.89746,    	-118.90332 },
	{ 1126.07446,    	-2463.76172 },
	{ 3345.32129,    	4102.53906 },
	{ 4481.80859,    	-4371.61475 },
	{ 292.08960,    	-1714.67053 },
	{ -768.34857,    	3615.48389 },
	{ -6255.54395,    	-2096.15479 },
	{ 625.06104,    	4187.09961 },
	{ -8008.31836,    	-5119.86523 },
	{ -3770.13281,    	-2412.00195 },
	{ -5791.89746,    	-4758.59473 },
	{ 12482.27246,    	-5657.26318 },
	{ -2742.11523,    	4252.91846 },
	{ 3039.84570,    	1263.14856 },
	{ 7379.33496,    	-2096.16235 },
	{ 8723.76367,    	4258.68555 },
	{ -13610.05273,    	10489.47363 },
	{ 6557.19873,    	-2465.88672 },
	{ -1157.32031,    	-647.92188 },
	{ -3558.26562,    	1180.63135 },
	{ -456.65430,    	1270.71729 },
	{ 7632.38672,    	-4254.65820 },
	{ -2272.80078,    	-1982.60083 },
	{ 2515.95703,    	2160.28418 },
	{ 2115.06055,    	864.89648 },
	{ 4873.33789,    	3928.47852 },
	{ -5572.65332,    	6657.57959 },
	{ 5231.41504,    	2382.87793 },
	{ -1033.77344,    	-1065.79614 },
	{ 331.07910,    	2107.23145 },
	{ -307.48608,    	1203.28906 },
	{ 142.52832,    	-2005.10938 },
	{ -232.00000,    	0.00000 },
	{ 142.52734,    	2005.11133 },
	{ -307.48633,    	-1203.29199 },
	{ 331.08008,    	-2107.23267 },
	{ -1033.77344,    	1065.79614 },
	{ 5231.41406,    	-2382.87793 },
	{ -5572.65039,    	-6657.57812 },
	{ 4873.33789,    	-3928.47852 },
	{ 2115.06641,    	-864.89746 },
	{ 2515.95703,    	-2160.28418 },
	{ -2272.79688,    	1982.60083 },
	{ 7632.38281,    	4254.65039 },
	{ -456.65625,    	-1270.71802 },
	{ -3558.26562,    	-1180.63037 },
	{ -1157.32031,    	647.92188 },
	{ 6557.19727,    	2465.88672 },
	{ -13610.05273,    	-10489.47363 },
	{ 8723.75977,    	-4258.68750 },
	{ 7379.33594,    	2096.16162 },
	{ 3039.85156,    	-1263.15369 },
	{ -2742.11719,    	-4252.91846 },
	{ 12482.27148,    	5657.25830 },
	{ -5791.90234,    	4758.59277 },
	{ -3770.13281,    	2412.00439 },
	{ -8008.31836,    	5119.86572 },
	{ 625.05859,    	-4187.10254 },
	{ -6255.54492,    	2096.15186 },
	{ -768.34570,    	-3615.48486 },
	{ 292.08984,    	1714.67139 },
	{ 4481.80957,    	4371.61572 },
	{ 3345.32324,    	-4102.54004 },
	{ 1126.07422,    	2463.76318 },
	{ 1370.89746,    	118.90332 },
	{ 1617.35254,    	4282.81885 },
	{ 6398.70215,    	-1678.46802 },
	{ -5402.36426,    	-4696.02588 },
	{ 8356.79688,    	-414.34521 },
	{ 14095.15137,    	385.67993 },
	{ -8847.98730,    	-10271.50488 },
	{ -23762.52148,    	11054.74414 },
	{ 40448.58203,    	13859.21289 },
	{ -52793.03125,    	24486.99805 },
	{ -27035.22656,    	1310.77051 },
	{ 6807.50195,    	-10769.62109 },
	{ -27331.35352,    	-34228.51562 },
	{ -71049.81250,    	58631.54688 },
	{ -21254.23633,    	-41745.01953 },
	{ -6969.50293,    	-17782.62891 },
	{ -9003.18164,    	11478.96680 },
	{ 18260.02539,    	35988.01172 },
	{ 36912.18750,    	-49166.64062 },
	{ 42157.32031,    	41978.82031 },
	{ 16526.95508,    	27696.07812 },
	{ 11470.85938,    	6563.74219 },
	{ -7779.92090,    	-14774.27930 },
	{ -13890.99609,    	61122.18750 },
	{ -20440.64258,    	7156.10352 },
	{ -18525.41797,    	9182.70020 },
	{ -22331.33594,    	31930.32227 },
	{ 59223.69531,    	38561.92188 },
	{ -55926.85156,    	-1485.79858 },
	{ -24862.25781,    	-44070.99609 },
	{ -17049.02344,    	-19122.55273 },
	{ -30270.12109,    	-25984.26367 },
	{ -118488.00000,    	77088.00000 },
	{ 36738.72266,    	-130771.36719 },
	{ 49082.57031,    	7483.09668 },
	{ 44931.66797,    	18716.84180 },
	{ 50220.58984,    	32500.72461 },
	{ 142764.87500,    	-140034.35938 },
	{ 30580.63477,    	94612.96094 },
	{ -35176.21484,    	21469.94141 },
	{ -20822.18555,    	15103.65527 },
	{ -35827.76172,    	36042.15625 },
	{ -3806.24146,    	74393.80469 },
	{ -74004.99219,    	-29061.77344 },
	{ 3968.06958,    	-2645.72021 },
	{ -1687.79736,    	4439.19922 },
	{ 12464.35352,    	-11326.15039 },
	{ 23322.98047,    	-32515.99219 },
	{ -11930.70508,    	5473.50977 },
	{ -5473.99512,    	-11806.83789 },
	{ -10753.18945,    	-1607.05518 },
	{ 18998.74219,    	16751.18555 },
	{ -3536.23291,    	60988.54688 },
	{ -24046.98828,    	-24051.33203 },
	{ 10474.67773,    	-6512.09961 },
	{ 981.61035,    	19020.98633 },
	{ 4494.73047,    	-27837.21289 },
	{ 12559.11523,    	-6030.09082 },
	{ 826.89502,    	19296.93750 },
	{ -14279.60254,    	-8235.44531 },
	{ -16333.69727,    	9389.22656 },
	{ -6181.61621,    	-18242.14062 },
	{ -33305.92969,    	14812.79688 },
	{ -22065.19922,    	9785.72461 },
	{ 20853.10156,    	-1929.09668 },
	{ -9079.20605,    	-20188.29102 },
	{ -22509.62109,    	4733.89648 },
	{ 21480.79883,    	6997.79199 },
	{ 21709.51172,    	18528.06250 },
	{ -2846.28662,    	11476.90625 },
	{ -28127.30664,    	-8088.24268 },
	{ 37760.36328,    	79229.14844 },
	{ -18101.00195,    	-55803.94531 },
	{ -81376.82031,    	-10466.37695 },
	{ -63820.07422,    	68658.86719 },
	{ -53008.75781,    	-25714.47070 },
	{ -128946.08594,    	-18644.18359 },
	{ -99516.96875,    	-82621.87500 },
	{ 27473.27344,    	6248.95166 },
	{ 59178.77344,    	-52555.16016 },
	{ 39695.93750,    	3345.06982 },
	{ -25144.14258,    	258579.71875 },
	{ 127691.90625,    	-259417.00000 },
	{ 93153.69531,    	-27476.39258 },
	{ 66620.93750,    	2205.30518 },
	{ 150159.01562,    	-152210.73438 },
	{ 139451.23438,    	-14371.00879 },
	{ 83996.42188,    	-43696.37891 },
	{ 100249.77344,    	42511.27344 },
	{ 44833.42969,    	87710.11719 },
	{ -61850.04688,    	-47537.94531 },
	{ -130162.39062,    	-215829.32812 },
	{ 103811.81250,    	-42351.71094 },
	{ 62009.32812,    	-49705.67188 },
	{ 28306.76172,    	29773.97461 },
	{ 22213.34766,    	8748.16016 }
};

extern const FFTData FFTData_256;

const FFTData FFTData_256 = {
256, (complex*)ffttest_256_in, (complex*)ffttest_256_out
};
