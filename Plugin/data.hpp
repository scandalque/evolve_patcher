#pragma once
#include <iostream>
#include <vector>

namespace data {
	struct st_textdraw {
		uint16_t id;
		uint8_t flags;
		float letter_width;
		float letter_height;
		uint32_t letter_color;
		float line_width;
		float line_height;
		uint32_t box_color;
		uint8_t shadow;
		uint8_t outline;
		uint32_t background_color;
		uint8_t style;
		uint8_t selectable;
		float x;
		float y;
		uint16_t model_id;
		float rotation_x;
		float rotation_y;
		float rotation_z;
		float zoom;
		int16_t color_1;
		int16_t color_2;
		uint16_t size;
		char text[64];
	};

	struct st_radar_icons {
		std::uintptr_t addr;
		float new_value;
	};

	inline std::vector<data::st_textdraw> textdraws = {
		{421, 2, 0.48f, 1.12f, 3238002688, 21.7999f, -15.14f, 2155905152, 0, 0, 4278190080, 4, 0, 546.899f, 34.1183f, 0, 0, 0, 0, 1, -1, -1, 21, "particle:lamp_shad_64"},
		{422, 24, 0.2937f, 1.5197f, 4294967295, 1280, 1280, 2155905152, 0, 1, 402653184, 1, 0, 588.565f, 8.3545f, 0, 0, 0, 0, 1, -1, -1, 6, "EVOLVE"},
		{423, 2, 0.48f, 1.12f, 4283382015, 28, 33, 2155905152, 0, 0, 4278190080, 4, 0, 544.132f, 3.3218f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{424, 24, 0.4544f, 1.927f, 4294967295, 1280, 1280, 2155905152, 0, 0, 1560281088, 1, 0, 558.199f, 10.1546f, 0, 0, 0, 0, 1, -1, -1, 1, "E"},
		{425, 2, 0.48f, 1.12f, 4283382015, 8, 10, 2155905152, 0, 0, 4278190080, 4, 0, 554.132f, 2.6328f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{426, 2, 0.48f, 1.12f, 4283382015, 16, 28, 2155905152, 0, 0, 4278190080, 4, 0, 549.866f, 1.6476f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{427, 2, 0.48f, 1.12f, 4283382015, 7, 9, 2155905152, 0, 0, 4278190080, 4, 0, 547.999f, 5.0367f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{428, 2, 0.48f, 1.12f, 4283382015, 19, 13, 2155905152, 0, 0, 4278190080, 4, 0, 546.499f, 5.9368f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{429, 2, 0.48f, 1.12f, 4283382015, 27, 13, 2155905152, 0, 0, 4278190080, 4, 0, 541.999f, 9.8141f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{430, 2, 0.48f, 1.12f, 4283382015, 4, 5, 2155905152, 0, 0, 4278190080, 4, 0, 545.232f, 8.7105f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{431, 2, 0.48f, 1.12f, 4283382015, 4.03f, 4.2199f, 2155905152, 0, 0, 4278190080, 4, 0, 551.466f, 2.4881f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{432, 2, 0.48f, 1.12f, 4283382015, 3, 4, 2155905152, 0, 0, 4278190080, 4, 0, 546.166f, 4.9067f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{433, 2, 0.48f, 1.12f, 4283382015, 3, 3, 2155905152, 0, 0, 4278190080, 4, 0, 547.832f, 1.8443f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{434, 2, 0.48f, 1.12f, 5191935, 27.03f, 5, 2155905152, 0, 0, 4278190080, 4, 0, 574.6f, 22.4295f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_SPAC:white"},
		{435, 2, 0.48f, 1.12f, 5191935, 6, 7.15f, 2155905152, 0, 0, 4278190080, 4, 0, 571.266f, 21.1847f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{436, 2, 0.48f, 1.12f, 5191935, 6, 7.15f, 2155905152, 0, 0, 4278190080, 4, 0, 598.399f, 21.3848f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{437, 2, 0.48f, 1.12f, 5191935, 5, 6, 2155905152, 0, 0, 4278190080, 4, 0, 603.466f, 21.5848f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{438, 2, 0.48f, 1.12f, 5191935, 3.7599f, 4.3498f, 2155905152, 0, 0, 4278190080, 4, 0, 607.333f, 24.2884f, 0, 0, 0, 0, 1, -1, -1, 12, "LD_BEAT:chit"},
		{439, 2, 0.48f, 1.12f, 4283382015, 38, -6, 2155905152, 0, 0, 4278190080, 4, 0, 569.233f, 28.1812f, 0, 0, 0, 0, 1, -1, -1, 21, "particle:lamp_shad_64"},
		{440, 24, 0.123f, 0.5504f, 4294967295, 1280, 1280, 2155905152, 0, 0, 4294967295, 1, 0, 588.067f, 22.0405f, 0, 0, 0, 0, 1, -1, -1, 11, "SAINT-LOUIS"},
		{441, 2, 0.48f, 1.12f, 4283382015, 38, 3, 2155905152, 0, 0, 4278190080, 4, 0, 569.233f, 24.1037f, 0, 0, 0, 0, 1, -1, -1, 21, "particle:lamp_shad_64"}
	};
	inline std::vector<data::st_radar_icons> icons = {
		{ 0x586047, 8.0f },
		{ 0x586060, 8.0f },
		{ 0x5886DC, 8.0f },
		{ 0x584192, 0.001563f },
		{ 0x5841B2, 0.002232f },
		{ 0x58410D, 0.001563f },
		{ 0x58412D, 0.002232f },
		{ 0x5842E8, 0.001563f },
		{ 0x5842C8, 0.002232f },
		{ 0x58424B, 0.001563f },
		{ 0x584209, 0.002232f },
		{ 0x584436, 0.001563f },
		{ 0x58440E, 0.002232f },
		{ 0x58439E, 0.001563f },
		{ 0x584348, 0.002232f }
	};
}