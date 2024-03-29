/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include "units.h"

// Trace of net flow readings captured on a live device using pressure command
// mode at covent preset #2:
//
//  - PIP  20cm
//  - PEEP 10cm
//  - rr   12bpm (5s/breath)
//  - i:e  0.25 (itime=1s, etime=4s)
//
// Duration between each line is 10ms.
//
// There's a manual inspiratory attempt in here; in other words, I expanded the
// test lung a bit with my finger during the expiratory dwell.
inline const VolumetricFlow FLOW_TRACE_INSPIRATORY_EFFORT[] = {
    // Inspiratory period
    ml_per_sec(29.994f),
    ml_per_sec(0.518f),
    ml_per_sec(9.637f),
    ml_per_sec(99.013f),
    ml_per_sec(241.202f),
    ml_per_sec(515.562f),
    ml_per_sec(880.483f),
    ml_per_sec(1084.423f),
    ml_per_sec(1128.886f),
    ml_per_sec(980.812f),
    ml_per_sec(916.247f),
    ml_per_sec(948.431f),
    ml_per_sec(892.896f),
    ml_per_sec(1016.466f),
    ml_per_sec(1070.784f),
    ml_per_sec(990.811f),
    ml_per_sec(850.715f),
    ml_per_sec(838.165f),
    ml_per_sec(962.219f),
    ml_per_sec(766.710f),
    ml_per_sec(653.804f),
    ml_per_sec(816.798f),
    ml_per_sec(871.572f),
    ml_per_sec(651.192f),
    ml_per_sec(726.440f),
    ml_per_sec(561.129f),
    ml_per_sec(729.581f),
    ml_per_sec(581.710f),
    ml_per_sec(529.784f),
    ml_per_sec(595.013f),
    ml_per_sec(415.098f),
    ml_per_sec(437.192f),
    ml_per_sec(354.627f),
    ml_per_sec(514.877f),
    ml_per_sec(342.562f),
    ml_per_sec(266.887f),
    ml_per_sec(434.984f),
    ml_per_sec(285.165f),
    ml_per_sec(202.937f),
    ml_per_sec(392.346f),
    ml_per_sec(128.141f),
    ml_per_sec(4.351f),
    ml_per_sec(295.270f),
    ml_per_sec(356.833f),
    ml_per_sec(67.111f),
    ml_per_sec(99.110f),
    ml_per_sec(24.716f),
    ml_per_sec(-70.950f),
    ml_per_sec(3.640f),
    ml_per_sec(-3.330f),
    ml_per_sec(132.000f),
    ml_per_sec(-215.953f),
    ml_per_sec(-54.832f),
    ml_per_sec(59.624f),
    ml_per_sec(6.631f),
    ml_per_sec(4.156f),
    ml_per_sec(-38.851f),
    ml_per_sec(-27.548f),
    ml_per_sec(-29.802f),
    ml_per_sec(-78.587f),
    ml_per_sec(-27.042f),
    ml_per_sec(-4.162f),
    ml_per_sec(-33.578f),
    ml_per_sec(-74.315f),
    ml_per_sec(-94.177f),
    ml_per_sec(11.125f),
    ml_per_sec(-59.219f),
    ml_per_sec(-23.222f),
    ml_per_sec(-97.317f),
    ml_per_sec(-4.924f),
    ml_per_sec(11.322f),
    ml_per_sec(-93.533f),
    ml_per_sec(-124.538f),
    ml_per_sec(-94.631f),
    ml_per_sec(15.031f),
    ml_per_sec(-94.755f),
    ml_per_sec(-77.286f),
    ml_per_sec(-20.666f),
    ml_per_sec(-88.939f),
    ml_per_sec(-58.193f),
    ml_per_sec(0.305f),
    ml_per_sec(-85.707f),
    ml_per_sec(9.717f),
    ml_per_sec(27.796f),
    ml_per_sec(-124.764f),
    ml_per_sec(10.310f),
    ml_per_sec(-31.443f),
    ml_per_sec(-142.242f),
    ml_per_sec(63.241f),
    ml_per_sec(44.667f),
    ml_per_sec(1.025f),
    ml_per_sec(-17.144f),
    ml_per_sec(-0.189f),
    ml_per_sec(1.802f),
    ml_per_sec(-13.296f),
    ml_per_sec(-5.218f),
    ml_per_sec(58.489f),
    ml_per_sec(-13.301f),
    ml_per_sec(-103.101f),
    ml_per_sec(54.397f),
    ml_per_sec(35.830f),
    ml_per_sec(-45.264f),
    // Expiratory period starts at t = 1000
    ml_per_sec(-37.998f),
    ml_per_sec(-221.026f),
    ml_per_sec(-732.939f),
    ml_per_sec(-921.699f),
    ml_per_sec(-1033.845f),
    ml_per_sec(-1079.585f),
    ml_per_sec(-1172.592f),
    ml_per_sec(-1127.773f),
    ml_per_sec(-997.011f),
    ml_per_sec(-965.559f),
    ml_per_sec(-911.473f),
    ml_per_sec(-864.414f),
    ml_per_sec(-960.757f),
    ml_per_sec(-965.190f),
    ml_per_sec(-819.178f),
    ml_per_sec(-751.081f),
    ml_per_sec(-853.844f),
    ml_per_sec(-781.005f),
    ml_per_sec(-692.166f),
    ml_per_sec(-693.323f),
    ml_per_sec(-671.465f),
    ml_per_sec(-605.825f),
    ml_per_sec(-552.140f),
    ml_per_sec(-523.273f),
    ml_per_sec(-499.171f),
    ml_per_sec(-477.252f),
    ml_per_sec(-449.892f),
    ml_per_sec(-406.792f),
    ml_per_sec(-368.656f),
    ml_per_sec(-343.741f),
    ml_per_sec(-338.468f),
    ml_per_sec(-338.738f),
    ml_per_sec(-321.447f),
    ml_per_sec(-287.456f),
    ml_per_sec(-253.114f),
    ml_per_sec(-232.797f),
    ml_per_sec(-210.684f),
    ml_per_sec(-195.326f),
    ml_per_sec(-182.770f),
    ml_per_sec(-192.534f),
    ml_per_sec(-188.348f),
    ml_per_sec(-168.263f),
    ml_per_sec(-183.760f),
    ml_per_sec(-166.195f),
    ml_per_sec(-160.069f),
    ml_per_sec(-136.820f),
    ml_per_sec(-112.960f),
    ml_per_sec(-115.295f),
    ml_per_sec(-88.572f),
    ml_per_sec(-48.763f),
    ml_per_sec(-51.222f),
    ml_per_sec(-36.093f),
    ml_per_sec(-45.718f),
    ml_per_sec(-50.679f),
    ml_per_sec(-35.954f),
    ml_per_sec(-41.523f),
    ml_per_sec(-43.640f),
    ml_per_sec(-21.622f),
    ml_per_sec(-0.780f),
    ml_per_sec(2.451f),
    ml_per_sec(40.050f),
    ml_per_sec(20.009f),
    ml_per_sec(17.329f),
    ml_per_sec(6.683f),
    ml_per_sec(16.634f),
    ml_per_sec(39.907f),
    ml_per_sec(56.348f),
    ml_per_sec(50.067f),
    ml_per_sec(52.078f),
    ml_per_sec(60.248f),
    ml_per_sec(53.646f),
    ml_per_sec(39.837f),
    ml_per_sec(34.501f),
    ml_per_sec(30.754f),
    ml_per_sec(40.684f),
    ml_per_sec(40.611f),
    ml_per_sec(37.813f),
    ml_per_sec(43.424f),
    ml_per_sec(39.672f),
    ml_per_sec(64.649f),
    ml_per_sec(61.391f),
    // Inspiratory effort starts at t ~= 1800ms
    ml_per_sec(73.984f),
    ml_per_sec(67.343f),
    ml_per_sec(81.399f),
    ml_per_sec(150.530f),
    ml_per_sec(204.221f),
    ml_per_sec(285.325f),
    ml_per_sec(366.895f),
    ml_per_sec(412.675f),
    ml_per_sec(418.842f),
    ml_per_sec(475.655f),
    ml_per_sec(462.574f),
    ml_per_sec(495.203f),
    ml_per_sec(475.075f),
    ml_per_sec(445.167f),
    ml_per_sec(455.124f),
    ml_per_sec(382.147f),
    ml_per_sec(301.131f),
    ml_per_sec(269.780f),
    ml_per_sec(206.753f),
    ml_per_sec(184.041f),
    ml_per_sec(182.816f),
    ml_per_sec(101.476f),
    ml_per_sec(34.173f),
    ml_per_sec(-12.921f),
    // Inspiratory effort ends / rebound begins at t ~= 2040ms
    ml_per_sec(-138.099f),
    ml_per_sec(-224.701f),
    ml_per_sec(-304.100f),
    ml_per_sec(-300.459f),
    ml_per_sec(-276.917f),
    ml_per_sec(-262.918f),
    ml_per_sec(-201.154f),
    ml_per_sec(-147.697f),
    ml_per_sec(-104.766f),
    ml_per_sec(-90.482f),
    ml_per_sec(-84.544f),
    ml_per_sec(-73.511f),
    ml_per_sec(-36.800f),
    ml_per_sec(-41.491f),
    ml_per_sec(-30.778f),
    ml_per_sec(-24.415f),
    ml_per_sec(-26.684f),
    ml_per_sec(-40.533f),
    ml_per_sec(-23.179f),
    ml_per_sec(-4.608f),
    ml_per_sec(1.012f),
    // Dwell at PEEP
    ml_per_sec(14.621f),
    ml_per_sec(24.915f),
    ml_per_sec(32.386f),
    ml_per_sec(23.978f),
    ml_per_sec(2.340f),
    ml_per_sec(20.331f),
    ml_per_sec(2.968f),
    ml_per_sec(11.546f),
    ml_per_sec(17.580f),
    ml_per_sec(32.792f),
    ml_per_sec(44.427f),
    ml_per_sec(35.275f),
    ml_per_sec(17.602f),
    ml_per_sec(42.211f),
    ml_per_sec(36.791f),
    ml_per_sec(38.430f),
    ml_per_sec(43.514f),
    ml_per_sec(26.600f),
    ml_per_sec(16.994f),
    ml_per_sec(24.349f),
    ml_per_sec(42.179f),
    ml_per_sec(43.906f),
    ml_per_sec(50.756f),
    ml_per_sec(50.383f),
    ml_per_sec(24.020f),
    ml_per_sec(45.658f),
    ml_per_sec(33.715f),
    ml_per_sec(33.825f),
    ml_per_sec(48.966f),
    ml_per_sec(20.065f),
    ml_per_sec(19.918f),
    ml_per_sec(32.171f),
    ml_per_sec(24.341f),
    ml_per_sec(35.577f),
    ml_per_sec(26.344f),
    ml_per_sec(30.048f),
    ml_per_sec(38.957f),
    ml_per_sec(33.535f),
    ml_per_sec(25.523f),
    ml_per_sec(40.420f),
    ml_per_sec(28.218f),
    ml_per_sec(24.693f),
    ml_per_sec(13.843f),
    ml_per_sec(45.271f),
    ml_per_sec(28.339f),
    ml_per_sec(31.611f),
    ml_per_sec(37.180f),
    ml_per_sec(37.702f),
    ml_per_sec(39.036f),
    ml_per_sec(24.635f),
    ml_per_sec(17.553f),
    ml_per_sec(35.413f),
    ml_per_sec(40.725f),
    ml_per_sec(31.192f),
    ml_per_sec(26.863f),
    ml_per_sec(29.392f),
    ml_per_sec(79.649f),
    ml_per_sec(31.707f),
    ml_per_sec(41.773f),
    ml_per_sec(21.678f),
    ml_per_sec(26.182f),
    ml_per_sec(46.510f),
    ml_per_sec(36.588f),
    ml_per_sec(43.187f),
    ml_per_sec(69.280f),
    ml_per_sec(4.114f),
    ml_per_sec(28.229f),
    ml_per_sec(24.120f),
    ml_per_sec(19.309f),
    ml_per_sec(26.981f),
    ml_per_sec(6.350f),
    ml_per_sec(25.985f),
    ml_per_sec(45.078f),
    ml_per_sec(36.002f),
    ml_per_sec(22.264f),
    ml_per_sec(25.626f),
    ml_per_sec(37.015f),
    ml_per_sec(23.489f),
    ml_per_sec(40.681f),
    ml_per_sec(31.973f),
    ml_per_sec(34.159f),
    ml_per_sec(50.185f),
    ml_per_sec(63.810f),
    ml_per_sec(25.732f),
    ml_per_sec(29.343f),
    ml_per_sec(13.513f),
    ml_per_sec(35.661f),
    ml_per_sec(62.060f),
    ml_per_sec(51.744f),
    ml_per_sec(49.073f),
    ml_per_sec(21.500f),
    ml_per_sec(27.813f),
    ml_per_sec(8.779f),
    ml_per_sec(33.574f),
    ml_per_sec(25.063f),
    ml_per_sec(10.824f),
    ml_per_sec(-0.195f),
    ml_per_sec(28.174f),
    ml_per_sec(60.652f),
    ml_per_sec(76.375f),
    ml_per_sec(36.592f),
    ml_per_sec(11.067f),
    ml_per_sec(11.728f),
    ml_per_sec(21.679f),
    ml_per_sec(70.579f),
    ml_per_sec(47.501f),
    ml_per_sec(19.602f),
    ml_per_sec(42.750f),
    ml_per_sec(-0.782f),
    ml_per_sec(14.365f),
    ml_per_sec(19.273f),
    ml_per_sec(9.319f),
    ml_per_sec(20.806f),
    ml_per_sec(27.563f),
    ml_per_sec(33.156f),
    ml_per_sec(15.830f),
    ml_per_sec(27.788f),
    ml_per_sec(18.919f),
    ml_per_sec(31.596f),
    ml_per_sec(64.367f),
    ml_per_sec(20.171f),
    ml_per_sec(9.923f),
    ml_per_sec(-3.397f),
    ml_per_sec(4.301f),
    ml_per_sec(21.454f),
    ml_per_sec(9.835f),
    ml_per_sec(23.945f),
    ml_per_sec(27.427f),
    ml_per_sec(30.460f),
    ml_per_sec(53.647f),
    ml_per_sec(28.626f),
    ml_per_sec(24.230f),
    ml_per_sec(21.031f),
    ml_per_sec(36.673f),
    ml_per_sec(36.104f),
    ml_per_sec(31.785f),
    ml_per_sec(30.808f),
    ml_per_sec(3.748f),
    ml_per_sec(-11.878f),
    ml_per_sec(5.883f),
    ml_per_sec(22.684f),
    ml_per_sec(21.430f),
    ml_per_sec(69.894f),
    ml_per_sec(51.047f),
    ml_per_sec(21.947f),
    ml_per_sec(17.361f),
    ml_per_sec(9.600f),
    ml_per_sec(13.872f),
    ml_per_sec(13.441f),
    ml_per_sec(4.562f),
    ml_per_sec(23.975f),
    ml_per_sec(15.014f),
    ml_per_sec(13.665f),
    ml_per_sec(12.880f),
    ml_per_sec(23.262f),
    ml_per_sec(38.720f),
    ml_per_sec(53.122f),
    ml_per_sec(29.681f),
    ml_per_sec(-3.195f),
    ml_per_sec(5.787f),
    ml_per_sec(38.640f),
    ml_per_sec(38.850f),
    ml_per_sec(47.233f),
    ml_per_sec(37.776f),
    ml_per_sec(27.922f),
    ml_per_sec(1.104f),
    ml_per_sec(25.855f),
    ml_per_sec(6.911f),
    ml_per_sec(14.393f),
    ml_per_sec(57.027f),
    ml_per_sec(62.162f),
    ml_per_sec(9.420f),
    ml_per_sec(16.056f),
    ml_per_sec(-5.153f),
    ml_per_sec(45.913f),
    ml_per_sec(40.909f),
    ml_per_sec(52.164f),
    ml_per_sec(20.916f),
    ml_per_sec(19.941f),
    ml_per_sec(-1.948f),
    ml_per_sec(-9.756f),
    ml_per_sec(-14.089f),
    ml_per_sec(13.237f),
    ml_per_sec(30.688f),
    ml_per_sec(39.367f),
    ml_per_sec(34.559f),
    ml_per_sec(42.083f),
    ml_per_sec(24.369f),
    ml_per_sec(12.423f),
    ml_per_sec(10.435f),
    ml_per_sec(8.901f),
    ml_per_sec(10.941f),
    ml_per_sec(26.257f),
    ml_per_sec(47.034f),
    ml_per_sec(53.786f),
    ml_per_sec(62.364f),
    ml_per_sec(29.355f),
    ml_per_sec(15.388f),
    ml_per_sec(-26.943f),
    ml_per_sec(-20.186f),
    ml_per_sec(13.265f),
    ml_per_sec(4.144f),
    ml_per_sec(-1.599f),
    ml_per_sec(8.426f),
    ml_per_sec(-8.747f),
    ml_per_sec(28.658f),
    ml_per_sec(37.156f),
    ml_per_sec(29.146f),
    ml_per_sec(9.479f),
    ml_per_sec(-0.423f),
    ml_per_sec(16.467f),
    ml_per_sec(37.322f),
    ml_per_sec(13.128f),
    ml_per_sec(12.229f),
    ml_per_sec(9.765f),
    ml_per_sec(14.028f),
    ml_per_sec(38.620f),
    ml_per_sec(24.715f),
    ml_per_sec(39.174f),
    ml_per_sec(64.902f),
    ml_per_sec(41.311f),
    ml_per_sec(11.265f),
    ml_per_sec(7.432f),
    ml_per_sec(4.543f),
    ml_per_sec(10.928f),
    ml_per_sec(-0.184f),
    ml_per_sec(18.760f),
    ml_per_sec(21.730f),
    ml_per_sec(21.607f),
    ml_per_sec(0.717f),
    ml_per_sec(7.691f),
    ml_per_sec(18.011f),
    ml_per_sec(22.678f),
    ml_per_sec(54.228f),
    ml_per_sec(52.587f),
    ml_per_sec(-16.164f),
    ml_per_sec(-9.027f),
    ml_per_sec(15.494f),
    ml_per_sec(9.327f),
    ml_per_sec(31.807f),
    ml_per_sec(49.413f),
    ml_per_sec(31.598f),
    ml_per_sec(21.311f),
    ml_per_sec(4.017f),
    ml_per_sec(2.545f),
    ml_per_sec(36.066f),
    ml_per_sec(36.204f),
    ml_per_sec(8.968f),
    ml_per_sec(-14.111f),
    ml_per_sec(1.432f),
    ml_per_sec(25.677f),
    ml_per_sec(40.934f),
    ml_per_sec(27.716f),
    ml_per_sec(7.820f),
    ml_per_sec(14.980f),
    ml_per_sec(16.747f),
    ml_per_sec(2.267f),
    ml_per_sec(-7.764f),
    ml_per_sec(-10.908f),
    ml_per_sec(20.810f),
    ml_per_sec(51.332f),
    ml_per_sec(40.497f),
    ml_per_sec(15.023f),
    ml_per_sec(22.158f),
    ml_per_sec(6.579f),
    ml_per_sec(0.724f),
    ml_per_sec(-7.563f),
    ml_per_sec(20.649f),
    ml_per_sec(18.593f),
    ml_per_sec(18.095f),
    ml_per_sec(34.059f),
    ml_per_sec(15.625f),
    ml_per_sec(15.949f),
    ml_per_sec(16.020f),
};

// Trace of net flow readings.  Captured using pressure command mode at covent
// preset #2:
//
//  - PIP  20cm
//  - PEEP 10cm
//  - rr   12bpm (5s/breath)
//  - i:e  0.25 (itime=1s, etime=4s)
//
// Duration between each line is 10ms.
//
// No manual inspiratory effort here.
inline const VolumetricFlow FLOW_TRACE_NO_INSPIRATORY_EFFORT[] = {
    // Inspiratory period
    ml_per_sec(-19.828f),
    ml_per_sec(17.460f),
    ml_per_sec(42.323f),
    ml_per_sec(164.321f),
    ml_per_sec(426.431f),
    ml_per_sec(933.515f),
    ml_per_sec(1435.452f),
    ml_per_sec(1667.008f),
    ml_per_sec(1353.843f),
    ml_per_sec(1405.726f),
    ml_per_sec(1584.043f),
    ml_per_sec(1567.636f),
    ml_per_sec(1603.667f),
    ml_per_sec(1329.521f),
    ml_per_sec(1276.600f),
    ml_per_sec(1475.148f),
    ml_per_sec(1152.195f),
    ml_per_sec(1146.257f),
    ml_per_sec(1168.612f),
    ml_per_sec(1369.953f),
    ml_per_sec(1271.361f),
    ml_per_sec(955.010f),
    ml_per_sec(960.558f),
    ml_per_sec(852.964f),
    ml_per_sec(888.351f),
    ml_per_sec(1091.564f),
    ml_per_sec(792.820f),
    ml_per_sec(753.388f),
    ml_per_sec(812.439f),
    ml_per_sec(562.220f),
    ml_per_sec(392.479f),
    ml_per_sec(170.820f),
    ml_per_sec(-96.398f),
    ml_per_sec(-197.003f),
    ml_per_sec(-123.198f),
    ml_per_sec(152.056f),
    ml_per_sec(-50.420f),
    ml_per_sec(-57.252f),
    ml_per_sec(103.892f),
    ml_per_sec(-69.848f),
    ml_per_sec(-111.738f),
    ml_per_sec(-25.618f),
    ml_per_sec(-3.998f),
    ml_per_sec(-96.548f),
    ml_per_sec(-71.487f),
    ml_per_sec(-73.589f),
    ml_per_sec(15.143f),
    ml_per_sec(-65.946f),
    ml_per_sec(-18.797f),
    ml_per_sec(-19.636f),
    ml_per_sec(-89.132f),
    ml_per_sec(-7.350f),
    ml_per_sec(-25.254f),
    ml_per_sec(-80.258f),
    ml_per_sec(-72.309f),
    ml_per_sec(-65.461f),
    ml_per_sec(-73.998f),
    ml_per_sec(-19.002f),
    ml_per_sec(-30.627f),
    ml_per_sec(-55.715f),
    ml_per_sec(-69.667f),
    ml_per_sec(-70.531f),
    ml_per_sec(-46.452f),
    ml_per_sec(-96.665f),
    ml_per_sec(-86.095f),
    ml_per_sec(-8.009f),
    ml_per_sec(-34.337f),
    ml_per_sec(-31.020f),
    ml_per_sec(-61.113f),
    ml_per_sec(-41.097f),
    ml_per_sec(-69.484f),
    ml_per_sec(-46.205f),
    ml_per_sec(-64.445f),
    ml_per_sec(-46.588f),
    ml_per_sec(16.850f),
    ml_per_sec(-67.554f),
    ml_per_sec(-89.174f),
    ml_per_sec(-70.889f),
    ml_per_sec(-76.485f),
    ml_per_sec(-81.104f),
    ml_per_sec(-5.882f),
    ml_per_sec(-71.955f),
    ml_per_sec(-65.778f),
    ml_per_sec(-15.858f),
    ml_per_sec(-48.467f),
    ml_per_sec(-86.852f),
    ml_per_sec(-68.579f),
    ml_per_sec(-6.814f),
    ml_per_sec(-59.176f),
    ml_per_sec(5.875f),
    ml_per_sec(-9.663f),
    ml_per_sec(-88.482f),
    ml_per_sec(-80.962f),
    ml_per_sec(-65.994f),
    ml_per_sec(-74.132f),
    ml_per_sec(-57.808f),
    ml_per_sec(-60.162f),
    ml_per_sec(-79.745f),
    ml_per_sec(-20.863f),
    ml_per_sec(-70.590f),
    ml_per_sec(-70.730f),
    // Expiratory period begins
    ml_per_sec(-60.288f),
    ml_per_sec(-84.648f),
    ml_per_sec(-108.031f),
    ml_per_sec(-452.271f),
    ml_per_sec(-903.377f),
    ml_per_sec(-1341.959f),
    ml_per_sec(-1265.559f),
    ml_per_sec(-1216.003f),
    ml_per_sec(-1278.480f),
    ml_per_sec(-1238.215f),
    ml_per_sec(-1067.498f),
    ml_per_sec(-1047.230f),
    ml_per_sec(-1100.567f),
    ml_per_sec(-1047.842f),
    ml_per_sec(-905.884f),
    ml_per_sec(-981.229f),
    ml_per_sec(-950.565f),
    ml_per_sec(-833.385f),
    ml_per_sec(-906.198f),
    ml_per_sec(-905.932f),
    ml_per_sec(-780.039f),
    ml_per_sec(-792.679f),
    ml_per_sec(-834.301f),
    ml_per_sec(-781.379f),
    ml_per_sec(-789.638f),
    ml_per_sec(-722.246f),
    ml_per_sec(-730.906f),
    ml_per_sec(-769.480f),
    ml_per_sec(-753.128f),
    ml_per_sec(-733.425f),
    ml_per_sec(-738.871f),
    ml_per_sec(-714.309f),
    ml_per_sec(-648.587f),
    ml_per_sec(-639.303f),
    ml_per_sec(-655.080f),
    ml_per_sec(-644.348f),
    ml_per_sec(-680.986f),
    ml_per_sec(-694.751f),
    ml_per_sec(-680.322f),
    ml_per_sec(-657.170f),
    ml_per_sec(-600.282f),
    ml_per_sec(-669.633f),
    ml_per_sec(-662.507f),
    ml_per_sec(-660.540f),
    ml_per_sec(-589.326f),
    ml_per_sec(-556.094f),
    ml_per_sec(-592.699f),
    ml_per_sec(-640.012f),
    ml_per_sec(-561.065f),
    ml_per_sec(-532.751f),
    ml_per_sec(-526.433f),
    ml_per_sec(-549.377f),
    ml_per_sec(-524.448f),
    ml_per_sec(-517.044f),
    ml_per_sec(-494.830f),
    ml_per_sec(-508.471f),
    ml_per_sec(-497.268f),
    ml_per_sec(-472.202f),
    ml_per_sec(-435.896f),
    ml_per_sec(-426.990f),
    ml_per_sec(-416.193f),
    ml_per_sec(-425.481f),
    ml_per_sec(-435.360f),
    ml_per_sec(-401.592f),
    ml_per_sec(-391.097f),
    ml_per_sec(-359.849f),
    ml_per_sec(-350.959f),
    ml_per_sec(-322.958f),
    ml_per_sec(-336.170f),
    ml_per_sec(-300.595f),
    ml_per_sec(-266.642f),
    ml_per_sec(-222.401f),
    ml_per_sec(-203.067f),
    ml_per_sec(-213.142f),
    ml_per_sec(-211.666f),
    ml_per_sec(-197.069f),
    ml_per_sec(-202.017f),
    ml_per_sec(-183.064f),
    ml_per_sec(-189.344f),
    ml_per_sec(-171.604f),
    ml_per_sec(-135.286f),
    ml_per_sec(-89.496f),
    ml_per_sec(-60.461f),
    ml_per_sec(-60.446f),
    ml_per_sec(-40.998f),
    ml_per_sec(-21.880f),
    ml_per_sec(-16.017f),
    ml_per_sec(-18.747f),
    ml_per_sec(2.779f),
    ml_per_sec(-21.169f),
    ml_per_sec(-23.789f),
    ml_per_sec(5.720f),
    ml_per_sec(-1.408f),
    ml_per_sec(1.683f),
    ml_per_sec(9.334f),
    ml_per_sec(35.772f),
    ml_per_sec(55.957f),
    ml_per_sec(36.819f),
    ml_per_sec(64.308f),
    ml_per_sec(98.505f),
    ml_per_sec(117.487f),
    ml_per_sec(63.994f),
    ml_per_sec(76.471f),
    ml_per_sec(114.144f),
    ml_per_sec(120.425f),
    ml_per_sec(122.689f),
    ml_per_sec(115.913f),
    ml_per_sec(128.687f),
    ml_per_sec(107.270f),
    ml_per_sec(98.893f),
    ml_per_sec(128.641f),
    ml_per_sec(102.520f),
    ml_per_sec(122.582f),
    ml_per_sec(108.822f),
    ml_per_sec(79.052f),
    ml_per_sec(100.492f),
    ml_per_sec(101.680f),
    ml_per_sec(103.581f),
    ml_per_sec(98.260f),
    ml_per_sec(94.014f),
    ml_per_sec(146.125f),
    ml_per_sec(138.726f),
    ml_per_sec(127.913f),
    ml_per_sec(122.807f),
    ml_per_sec(160.387f),
    ml_per_sec(176.713f),
    ml_per_sec(151.317f),
    ml_per_sec(142.222f),
    ml_per_sec(145.826f),
    ml_per_sec(141.660f),
    ml_per_sec(119.771f),
    ml_per_sec(107.456f),
    ml_per_sec(124.530f),
    ml_per_sec(172.366f),
    ml_per_sec(173.630f),
    ml_per_sec(166.961f),
    ml_per_sec(152.621f),
    ml_per_sec(163.791f),
    ml_per_sec(84.067f),
    ml_per_sec(97.264f),
    ml_per_sec(158.020f),
    ml_per_sec(159.018f),
    ml_per_sec(124.666f),
    ml_per_sec(128.393f),
    ml_per_sec(150.898f),
    ml_per_sec(175.452f),
    ml_per_sec(130.795f),
    ml_per_sec(107.174f),
    ml_per_sec(154.307f),
    ml_per_sec(172.555f),
    ml_per_sec(123.513f),
    ml_per_sec(161.819f),
    ml_per_sec(178.736f),
    ml_per_sec(153.288f),
    ml_per_sec(161.294f),
    ml_per_sec(157.517f),
    ml_per_sec(154.946f),
    ml_per_sec(160.862f),
    ml_per_sec(162.316f),
    ml_per_sec(162.670f),
    ml_per_sec(146.099f),
    ml_per_sec(163.882f),
    ml_per_sec(153.511f),
    ml_per_sec(156.511f),
    ml_per_sec(128.934f),
    ml_per_sec(151.271f),
    ml_per_sec(130.258f),
    ml_per_sec(132.526f),
    ml_per_sec(141.165f),
    ml_per_sec(134.454f),
    ml_per_sec(115.448f),
    ml_per_sec(168.340f),
    ml_per_sec(168.180f),
    ml_per_sec(162.735f),
    ml_per_sec(109.471f),
    ml_per_sec(121.584f),
    ml_per_sec(144.688f),
    ml_per_sec(163.848f),
    ml_per_sec(194.538f),
    ml_per_sec(163.423f),
    ml_per_sec(134.954f),
    ml_per_sec(133.126f),
    ml_per_sec(135.975f),
    ml_per_sec(143.530f),
    ml_per_sec(131.018f),
    ml_per_sec(123.458f),
    ml_per_sec(131.059f),
    ml_per_sec(146.735f),
    ml_per_sec(96.399f),
    ml_per_sec(117.409f),
    ml_per_sec(148.192f),
    ml_per_sec(143.122f),
    ml_per_sec(132.979f),
    ml_per_sec(112.919f),
    ml_per_sec(150.267f),
    ml_per_sec(139.105f),
    ml_per_sec(101.097f),
    ml_per_sec(122.680f),
    ml_per_sec(136.127f),
    ml_per_sec(121.900f),
    ml_per_sec(99.776f),
    ml_per_sec(88.114f),
    ml_per_sec(80.704f),
    ml_per_sec(92.898f),
    ml_per_sec(112.154f),
    ml_per_sec(95.815f),
    ml_per_sec(188.572f),
    ml_per_sec(134.366f),
    ml_per_sec(78.841f),
    ml_per_sec(107.107f),
    ml_per_sec(144.912f),
    ml_per_sec(169.042f),
    ml_per_sec(116.688f),
    ml_per_sec(53.699f),
    ml_per_sec(108.754f),
    ml_per_sec(138.968f),
    ml_per_sec(168.174f),
    ml_per_sec(178.766f),
    ml_per_sec(75.044f),
    ml_per_sec(84.330f),
    ml_per_sec(65.146f),
    ml_per_sec(100.144f),
    ml_per_sec(85.560f),
    ml_per_sec(74.104f),
    ml_per_sec(104.665f),
    ml_per_sec(98.298f),
    ml_per_sec(122.177f),
    ml_per_sec(170.194f),
    ml_per_sec(125.870f),
    ml_per_sec(82.575f),
    ml_per_sec(49.946f),
    ml_per_sec(29.335f),
    ml_per_sec(72.912f),
    ml_per_sec(47.888f),
    ml_per_sec(42.037f),
    ml_per_sec(72.666f),
    ml_per_sec(34.762f),
    ml_per_sec(76.503f),
    ml_per_sec(47.316f),
    ml_per_sec(41.608f),
    ml_per_sec(66.271f),
    ml_per_sec(94.252f),
    ml_per_sec(80.844f),
    ml_per_sec(73.728f),
    ml_per_sec(56.289f),
    ml_per_sec(74.014f),
    ml_per_sec(129.282f),
    ml_per_sec(82.936f),
    ml_per_sec(70.011f),
    ml_per_sec(46.585f),
    ml_per_sec(60.265f),
    ml_per_sec(45.951f),
    ml_per_sec(74.085f),
    ml_per_sec(71.622f),
    ml_per_sec(76.864f),
    ml_per_sec(62.123f),
    ml_per_sec(67.147f),
    ml_per_sec(43.118f),
    ml_per_sec(78.073f),
    ml_per_sec(41.944f),
    ml_per_sec(34.001f),
    ml_per_sec(37.932f),
    ml_per_sec(64.364f),
    ml_per_sec(32.788f),
    ml_per_sec(22.982f),
    ml_per_sec(10.778f),
    ml_per_sec(55.856f),
    ml_per_sec(76.579f),
    ml_per_sec(55.249f),
    ml_per_sec(18.142f),
    ml_per_sec(40.523f),
    ml_per_sec(49.106f),
    ml_per_sec(72.755f),
    ml_per_sec(80.917f),
    ml_per_sec(74.615f),
    ml_per_sec(69.770f),
    ml_per_sec(80.902f),
    ml_per_sec(38.630f),
    ml_per_sec(-1.935f),
    ml_per_sec(8.013f),
    ml_per_sec(14.883f),
    ml_per_sec(23.412f),
    ml_per_sec(10.209f),
    ml_per_sec(37.046f),
    ml_per_sec(45.950f),
    ml_per_sec(46.330f),
    ml_per_sec(46.686f),
    ml_per_sec(0.755f),
    ml_per_sec(40.490f),
    ml_per_sec(33.916f),
    ml_per_sec(21.833f),
    ml_per_sec(7.023f),
    ml_per_sec(69.276f),
    ml_per_sec(26.824f),
    ml_per_sec(14.012f),
    ml_per_sec(24.244f),
    ml_per_sec(8.606f),
    ml_per_sec(13.728f),
    ml_per_sec(55.020f),
    ml_per_sec(78.840f),
    ml_per_sec(63.254f),
    ml_per_sec(42.024f),
    ml_per_sec(20.438f),
    ml_per_sec(17.818f),
    ml_per_sec(-4.827f),
    ml_per_sec(20.441f),
    ml_per_sec(31.217f),
    ml_per_sec(-1.194f),
    ml_per_sec(20.331f),
    ml_per_sec(22.107f),
    ml_per_sec(21.910f),
    ml_per_sec(26.674f),
    ml_per_sec(29.944f),
    ml_per_sec(35.389f),
    ml_per_sec(13.732f),
    ml_per_sec(62.415f),
    ml_per_sec(39.101f),
    ml_per_sec(22.861f),
    ml_per_sec(1.934f),
    ml_per_sec(11.136f),
    ml_per_sec(-2.403f),
    ml_per_sec(34.911f),
    ml_per_sec(12.792f),
    ml_per_sec(24.238f),
    ml_per_sec(50.086f),
    ml_per_sec(13.496f),
    ml_per_sec(-4.770f),
    ml_per_sec(48.511f),
    ml_per_sec(21.556f),
    ml_per_sec(-13.004f),
    ml_per_sec(8.752f),
    ml_per_sec(-20.784f),
    ml_per_sec(30.290f),
    ml_per_sec(16.834f),
    ml_per_sec(42.262f),
    ml_per_sec(24.462f),
    ml_per_sec(36.727f),
    ml_per_sec(23.364f),
    ml_per_sec(9.475f),
    ml_per_sec(0.146f),
    ml_per_sec(7.974f),
    ml_per_sec(11.362f),
    ml_per_sec(37.925f),
    ml_per_sec(20.757f),
    ml_per_sec(26.786f),
    ml_per_sec(15.707f),
    ml_per_sec(-14.681f),
    ml_per_sec(16.843f),
    ml_per_sec(-2.919f),
    ml_per_sec(20.526f),
    ml_per_sec(20.490f),
    ml_per_sec(29.338f),
    ml_per_sec(18.273f),
    ml_per_sec(27.264f),
    ml_per_sec(86.282f),
    ml_per_sec(65.775f),
    ml_per_sec(27.922f),
    ml_per_sec(-11.778f),
    ml_per_sec(5.094f),
    ml_per_sec(26.792f),
    ml_per_sec(27.248f),
    ml_per_sec(8.131f),
    ml_per_sec(-9.808f),
    ml_per_sec(16.190f),
    ml_per_sec(-0.407f),
    ml_per_sec(1.072f),
    ml_per_sec(16.345f),
    ml_per_sec(7.540f),
    ml_per_sec(36.202f),
    ml_per_sec(41.523f),
    ml_per_sec(4.569f),
    ml_per_sec(21.123f),
    ml_per_sec(9.738f),
    ml_per_sec(49.930f),
    ml_per_sec(37.927f),
    ml_per_sec(11.221f),
    ml_per_sec(-1.275f),
    ml_per_sec(-5.231f),
    ml_per_sec(-15.682f),
    ml_per_sec(-3.686f),
    ml_per_sec(-0.198f),
    ml_per_sec(12.154f),
    ml_per_sec(21.107f),
    ml_per_sec(8.044f),
    ml_per_sec(22.449f),
    ml_per_sec(-5.451f),
    ml_per_sec(-7.463f),
    ml_per_sec(-2.379f),
    ml_per_sec(21.435f),
    ml_per_sec(65.076f),
    ml_per_sec(37.414f),
    ml_per_sec(26.059f),
    ml_per_sec(-22.768f),
    ml_per_sec(12.205f),
    ml_per_sec(33.913f),
    ml_per_sec(5.763f),
    ml_per_sec(22.001f),
    ml_per_sec(59.427f),
    ml_per_sec(71.301f),
    ml_per_sec(20.192f),
    ml_per_sec(-17.382f),
};
