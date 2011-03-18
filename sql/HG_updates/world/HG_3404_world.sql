DELETE FROM `script_waypoint` WHERE `entry`=3465;
INSERT INTO `script_waypoint` VALUES
   (3465,0,-2095.840820,-3650.001221,61.716,0, ''),
   (3465,1,-2100.193604,-3613.949219,61.604,0, ''),
   (3465,2,-2098.549561,-3601.557129,59.154,0, ''),
   (3465,3,-2093.796387,-3595.234375,56.658,0, ''),
   (3465,4,-2072.575928,-3578.827637,48.844,0, ''),
   (3465,5,-2023.858398,-3568.146240,24.636,0, ''),
   (3465,6,-2013.576416,-3571.499756,22.203,0, ''),
   (3465,7,-2009.813721,-3580.547852,21.791,0, ''),
   (3465,8,-2015.296021,-3597.387695,21.760,0, ''),
   (3465,9,-2020.677368,-3610.296143,21.759,0, ''),
   (3465,10,-2019.990845,-3640.155273,21.759,0, ''),
   (3465,11,-2016.110596,-3664.133301,21.758,0, ''),
   (3465,12,-1999.397095,-3679.435059,21.316,0, ''),
   (3465,13,-1987.455811,-3688.309326,18.495,0, ''),
   (3465,14,-1973.966553,-3687.666748,14.996,0, ''),
   (3465,15,-1949.163940,-3678.054932,11.293,0, ''),
   (3465,16,-1934.091187,-3682.859619,9.897,30000, 'SAY_GIL_AT_LAST'),
   (3465,17,-1935.383911,-3682.322021,10.029,1500, 'SAY_GIL_PROCEED'),
   (3465,18,-1879.039185,-3699.498047,6.582,7500, 'SAY_GIL_FREEBOOTERS'),
   (3465,19,-1852.728149,-3703.778809,6.875,0, ''),
   (3465,20,-1812.989990,-3718.500732,10.572,0, ''),
   (3465,21,-1788.171265,-3722.867188,9.663,0, ''),
   (3465,22,-1767.206665,-3739.923096,10.082,0, ''),
   (3465,23,-1750.194580,-3747.392090,10.390,0, ''),
   (3465,24,-1729.335571,-3776.665527,11.779,0, ''),
   (3465,25,-1715.997925,-3802.404541,12.618,0, ''),
   (3465,26,-1690.711548,-3829.262451,13.905,0, ''),
   (3465,27,-1674.700684,-3842.398682,13.872,0, ''),
   (3465,28,-1632.726318,-3846.109619,14.401,0, ''),
   (3465,29,-1592.734497,-3842.225342,14.981,0, ''),
   (3465,30,-1561.614746,-3839.320801,19.118,0, ''),
   (3465,31,-1544.567627,-3834.393311,18.761,0, ''),
   (3465,32,-1512.514404,-3831.715820,22.914,0, ''),
   (3465,33,-1486.889771,-3836.639893,23.964,0, ''),
   (3465,34,-1434.193604,-3852.702881,18.843,0, ''),
   (3465,35,-1405.794678,-3854.488037,17.276,0, ''),
   (3465,36,-1366.592041,-3852.383789,19.273,0, ''),
   (3465,37,-1337.360962,-3837.827148,17.352,2000, 'SAY_GIL_ALMOST'),
   (3465,38,-1299.744507,-3810.691406,20.801,0, ''),
   (3465,39,-1277.144409,-3782.785156,25.918,0, ''),
   (3465,40,-1263.686768,-3781.251953,26.447,0, ''),
   (3465,41,-1243.674438,-3786.328125,25.281,0, ''),
   (3465,42,-1221.875488,-3784.124512,24.051,0, ''),
   (3465,43,-1204.011230,-3775.943848,24.437,0, ''),
   (3465,44,-1181.706787,-3768.934082,23.368,0, ''),
   (3465,45,-1156.913818,-3751.559326,21.074,0, ''),
   (3465,46,-1138.830688,-3741.809326,17.843,0, ''),
   (3465,47,-1080.101196,-3738.780029,19.805,0, 'SAY_GIL_SWEET'),
   (3465,48,-1069.065186,-3735.006348,19.302,0, ''),
   (3465,49,-1061.941040,-3724.062256,21.086,0, ''),
   (3465,50,-1053.593262,-3697.608643,27.320,0, ''),
   (3465,51,-1044.110474,-3690.133301,24.856,0, ''),
   (3465,52,-1040.260986,-3690.739014,25.342,0, ''),
   (3465,53,-1028.146606,-3688.718750,23.843,7500, 'SAY_GIL_FREED');

DELETE FROM `script_texts` WHERE `entry` BETWEEN '-1600380' AND '-1600370';
INSERT INTO `script_texts` VALUES ('-1600380', 'Captain Brightsun, $N here has freed me! $N, I am certain the Captain will reward your bravery.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '66', 'gilthares SAY_GIL_FREED');
INSERT INTO `script_texts` VALUES ('-1600379', 'Ah, the sweet salt air of Ratchet.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '0', 'gilthares SAY_GIL_SWEET');
INSERT INTO `script_texts` VALUES ('-1600378', 'Almost back to Ratchet! Let\'s keep up the pace...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '0', 'gilthares SAY_GIL_ALMOST');
INSERT INTO `script_texts` VALUES ('-1600377', 'Get this $C off of me!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '0', 'gilthares SAY_GIL_AGGRO_4');
INSERT INTO `script_texts` VALUES ('-1600376', '$C coming right at us!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '0', 'gilthares SAY_GIL_AGGRO_3');
INSERT INTO `script_texts` VALUES ('-1600375', '$C heading this way fast! Time for revenge!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '0', 'gilthares SAY_GIL_AGGRO_2');
INSERT INTO `script_texts` VALUES ('-1600374', 'Help! $C attacking!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '0', 'gilthares SAY_GIL_AGGRO_1');
INSERT INTO `script_texts` VALUES ('-1600373', 'Looks like the Southsea Freeboters are heavily entrenched on the coast. This could get rough.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '25', 'gilthares SAY_GIL_FREEBOOTERS');
INSERT INTO `script_texts` VALUES ('-1600372', 'Now I feel better. Let\'s get back to Ratchet. Come on, $n.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '23', 'gilthares SAY_GIL_PROCEED');
INSERT INTO `script_texts` VALUES ('-1600371', 'At last! Free from Northwatch Hold! I need a moment to catch my breath...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '5', 'gilthares SAY_GIL_AT_LAST');
INSERT INTO `script_texts` VALUES ('-1600370', 'Stay close, $n. I\'ll need all the help I can get to break out of here. Let\'s go!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '0', '1', '1', 'gilthares SAY_GIL_START');
UPDATE `creature_template` SET `ScriptName` = 'npc_gilthares' WHERE `entry` = '3465';
UPDATE `quest_template` SET `SpecialFlags` = '2' WHERE `entry` = '898';