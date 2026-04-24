#pragma once
#include <cstdint>
#include <cstring>

namespace decrypts {

	inline uint32_t float_to_bits(float f) {
		uint32_t v;
		std::memcpy(&v, &f, sizeof(v));
		return v;
	}

	namespace BaseNetworkable {

		inline uint64_t decrypt_bn1(uint64_t encrypted) {
			uint32_t* p = reinterpret_cast<uint32_t*>(&encrypted);
			for (int i = 0; i < 2; i++) {
				uint32_t val = p[i];
				val -= 0x4456E120;
				val = (val >> 0x1C) | (val << 0x4);
				val ^= 0x7DF13224;
				val = (val >> 0xF) | (val << 0x11);
				p[i] = val;
			}
			return encrypted;
		}

		inline uint64_t decrypt_bn2(uint64_t encrypted) {
			uint32_t* p = reinterpret_cast<uint32_t*>(&encrypted);
			for (int i = 0; i < 2; i++) {
				uint32_t val = p[i];
				val ^= 0x8A3854A8;
				val -= 0x18AF0996;
				val = (val >> 0xE) | (val << 0x12);
				val -= 0x5E787974;
				p[i] = val;
			}
			return encrypted;
		}

	} // namespace BaseNetworkable

	namespace BasePlayer {

		inline uint64_t decrypt_clActiveItem(uint64_t encrypted) {
			uint32_t* p = reinterpret_cast<uint32_t*>(&encrypted);
			for (int i = 0; i < 2; i++) {
				uint32_t val = p[i];
				val = (val << 0x1C) | (val >> 0x4);
				val += 0xEA1A066B;
				val = (val << 0x1D) | (val >> 0x3);
				p[i] = val;
			}
			return encrypted;
		}

	} // namespace BasePlayer

	namespace PlayerWalkMovement {

		inline uint32_t decrypt_0x070(uint32_t val) {
			val = (val >> 0xA) | (val << 0x16);
			val -= 0x709C7ED0;
			val ^= 0x665DC87B;
			return val;
		}

		inline uint32_t encrypt_0x070(uint32_t val) {
			val ^= 0x665DC87B;
			val += 0x709C7ED0;
			val = (val << 0xA) | (val >> 0x16);
			return val;
		}

		inline uint32_t decrypt_0x078(uint32_t val) {
			val += 0xB456E2F0;
			val = (val << 0x1A) | (val >> 0x6);
			val ^= 0x9B7193DF;
			val += 0x56153C48;
			return val;
		}

		inline uint32_t encrypt_0x078(uint32_t val) {
			val -= 0x56153C48;
			val ^= 0x9B7193DF;
			val = (val >> 0x1A) | (val << 0x6);
			val -= 0xB456E2F0;
			return val;
		}

		inline uint32_t decrypt_0x080(uint32_t val) {
			val = (val << 0x18) | (val >> 0x8);
			val += 0x842276D3;
			val = (val << 0x1A) | (val >> 0x6);
			val -= 0x109928D3;
			return val;
		}

		inline uint32_t encrypt_0x080(uint32_t val) {
			val += 0x109928D3;
			val = (val >> 0x1A) | (val << 0x6);
			val -= 0x842276D3;
			val = (val >> 0x18) | (val << 0x8);
			return val;
		}

		inline uint32_t decrypt_0x088(uint32_t val) {
			val += 0xC71A9895;
			val = (val << 0x1E) | (val >> 0x2);
			val += 0xF9B1D20E;
			val = (val << 0x13) | (val >> 0xD);
			return val;
		}

		inline uint32_t encrypt_0x088(uint32_t val) {
			val = (val >> 0x13) | (val << 0xD);
			val -= 0xF9B1D20E;
			val = (val >> 0x1E) | (val << 0x2);
			val -= 0xC71A9895;
			return val;
		}

		inline uint32_t decrypt_0x090(uint32_t val) {
			val ^= 0x59180A8D;
			val = (val >> 0x1D) | (val << 0x3);
			val += 0x193F16E4;
			return val;
		}

		inline uint32_t encrypt_0x090(uint32_t val) {
			val -= 0x193F16E4;
			val = (val << 0x1D) | (val >> 0x3);
			val ^= 0x59180A8D;
			return val;
		}

		inline uint32_t decrypt_0x098(uint32_t val) {
			val += 0x443325A2;
			val ^= 0x3B18078A;
			val = (val << 0x19) | (val >> 0x7);
			val += 0x59A8A187;
			return val;
		}

		inline uint32_t encrypt_0x098(uint32_t val) {
			val -= 0x59A8A187;
			val = (val >> 0x19) | (val << 0x7);
			val ^= 0x3B18078A;
			val -= 0x443325A2;
			return val;
		}

		inline uint32_t decrypt_0x0A0(uint32_t val) {
			val = (val << 0x9) | (val >> 0x17);
			val += 0x98B027AA;
			val = (val << 0x1E) | (val >> 0x2);
			return val;
		}

		inline uint32_t encrypt_0x0A0(uint32_t val) {
			val = (val >> 0x1E) | (val << 0x2);
			val -= 0x98B027AA;
			val = (val >> 0x9) | (val << 0x17);
			return val;
		}

		inline uint32_t decrypt_0x0A8(uint32_t val) {
			val ^= 0x3BA54E92;
			val += 0x32DA7EB4;
			val = (val << 0x6) | (val >> 0x1A);
			val -= 0x457C2597;
			return val;
		}

		inline uint32_t encrypt_0x0A8(uint32_t val) {
			val += 0x457C2597;
			val = (val >> 0x6) | (val << 0x1A);
			val -= 0x32DA7EB4;
			val ^= 0x3BA54E92;
			return val;
		}

		inline uint32_t decrypt_0x0B0(uint32_t val) {
			val ^= 0x6166676C;
			val += 0xD3B0A94A;
			val = (val << 0xE) | (val >> 0x12);
			return val;
		}

		inline uint32_t encrypt_0x0B0(uint32_t val) {
			val = (val >> 0xE) | (val << 0x12);
			val -= 0xD3B0A94A;
			val ^= 0x6166676C;
			return val;
		}

		inline uint32_t decrypt_0x0B8(uint32_t val) {
			val += 0x328394D5;
			val = (val << 0xF) | (val >> 0x11);
			val += 0xD0E28948;
			val = (val << 0x19) | (val >> 0x7);
			return val;
		}

		inline uint32_t encrypt_0x0B8(uint32_t val) {
			val = (val >> 0x19) | (val << 0x7);
			val -= 0xD0E28948;
			val = (val >> 0xF) | (val << 0x11);
			val -= 0x328394D5;
			return val;
		}

		inline uint32_t decrypt_0x0C0(uint32_t val) {
			val ^= 0xFDE9B7DF;
			val = (val << 0x4) | (val >> 0x1C);
			val += 0x7AD171C3;
			val = (val << 0x17) | (val >> 0x9);
			return val;
		}

		inline uint32_t encrypt_0x0C0(uint32_t val) {
			val = (val >> 0x17) | (val << 0x9);
			val -= 0x7AD171C3;
			val = (val >> 0x4) | (val << 0x1C);
			val ^= 0xFDE9B7DF;
			return val;
		}

		inline uint32_t decrypt_0x0C8(uint32_t val) {
			val += 0x32900AF6;
			val = (val << 0x4) | (val >> 0x1C);
			val ^= 0x5F72EEDB;
			val -= 0x75AA1D6B;
			return val;
		}

		inline uint32_t encrypt_0x0C8(uint32_t val) {
			val += 0x75AA1D6B;
			val ^= 0x5F72EEDB;
			val = (val >> 0x4) | (val << 0x1C);
			val -= 0x32900AF6;
			return val;
		}

		inline uint32_t decrypt_0x0D0(uint32_t val) {
			val += 0x4D550E82;
			val = (val << 0x10) | (val >> 0x10);
			val += 0x33BED499;
			val = (val << 0x15) | (val >> 0xB);
			return val;
		}

		inline uint32_t encrypt_0x0D0(uint32_t val) {
			val = (val >> 0x15) | (val << 0xB);
			val -= 0x33BED499;
			val = (val >> 0x10) | (val << 0x10);
			val -= 0x4D550E82;
			return val;
		}

		inline uint32_t decrypt_0x0F8(uint32_t val) {
			val ^= 0x8788D0D;
			val += 0x65552F1E;
			val = (val << 0xA) | (val >> 0x16);
			val += 0x721CA560;
			return val;
		}

		inline uint32_t encrypt_0x0F8(uint32_t val) {
			val -= 0x721CA560;
			val = (val >> 0xA) | (val << 0x16);
			val -= 0x65552F1E;
			val ^= 0x8788D0D;
			return val;
		}

		inline uint32_t decrypt_0x100(uint32_t val) {
			val ^= 0xB1757ECC;
			val = (val << 0x1A) | (val >> 0x6);
			val ^= 0xD0E89EB7;
			val += 0x1ADB361F;
			return val;
		}

		inline uint32_t encrypt_0x100(uint32_t val) {
			val -= 0x1ADB361F;
			val ^= 0xD0E89EB7;
			val = (val >> 0x1A) | (val << 0x6);
			val ^= 0xB1757ECC;
			return val;
		}

		inline uint32_t decrypt_0x108(uint32_t val) {
			val += 0x10B24426;
			val = (val >> 0x1E) | (val << 0x2);
			val -= 0x606266D8;
			return val;
		}

		inline uint32_t encrypt_0x108(uint32_t val) {
			val += 0x606266D8;
			val = (val << 0x1E) | (val >> 0x2);
			val -= 0x10B24426;
			return val;
		}

		inline uint32_t decrypt_0x110(uint32_t val) {
			val ^= 0x607CC5AC;
			val += 0x1A44B4EF;
			val = (val << 0x7) | (val >> 0x19);
			return val;
		}

		inline uint32_t encrypt_0x110(uint32_t val) {
			val = (val >> 0x7) | (val << 0x19);
			val -= 0x1A44B4EF;
			val ^= 0x607CC5AC;
			return val;
		}

		inline uint32_t decrypt_0x118(uint32_t val) {
			val += 0x39A8457D;
			val ^= 0xDFA50245;
			val = (val << 0xE) | (val >> 0x12);
			val += 0x4FC93A01;
			return val;
		}

		inline uint32_t encrypt_0x118(uint32_t val) {
			val -= 0x4FC93A01;
			val = (val >> 0xE) | (val << 0x12);
			val ^= 0xDFA50245;
			val -= 0x39A8457D;
			return val;
		}

		inline uint32_t decrypt_0x120(uint32_t val) {
			val = (val << 0x10) | (val >> 0x10);
			val ^= 0x3286373C;
			val -= 0x75F5C609;
			return val;
		}

		inline uint32_t encrypt_0x120(uint32_t val) {
			val += 0x75F5C609;
			val ^= 0x3286373C;
			val = (val >> 0x10) | (val << 0x10);
			return val;
		}

		inline uint32_t decrypt_0x198(uint32_t val) {
			val = (val << 0x17) | (val >> 0x9);
			val += 0x52CAA8D6;
			val = (val << 0x1C) | (val >> 0x4);
			val += 0x34B76845;
			val = (val << 0x1B) | (val >> 0x5);
			val ^= 0xAA876454;
			val -= 0x5115F320;
			val ^= 0x6BF05527;
			val += 0x6E6AB812;
			val ^= 0x82262784;
			val += 0x2761ACF8;
			val ^= 0x3CF39FF7;
			val = (val << 0xD) | (val >> 0x13);
			val += 0x142A8D4E;
			val = (val << 0x16) | (val >> 0xA);
			val += 0x616C7E8;
			val = (val << 0xE) | (val >> 0x12);
			val += 0x1C0353D3;
			val = (val << 0x18) | (val >> 0x8);
			val ^= 0x7BD0AA05;
			val += 0x201FF599;
			val = (val << 0x11) | (val >> 0xF);
			val += 0x5929C4EC;
			val ^= 0x37D72CD9;
			val += 0x7CD65E14;
			val = (val >> 0x1E) | (val << 0x2);
			return val;
		}

		inline uint32_t encrypt_0x198(uint32_t val) {
			val = (val << 0x1E) | (val >> 0x2);
			val -= 0x7CD65E14;
			val ^= 0x37D72CD9;
			val -= 0x5929C4EC;
			val = (val >> 0x11) | (val << 0xF);
			val -= 0x201FF599;
			val ^= 0x7BD0AA05;
			val = (val >> 0x18) | (val << 0x8);
			val -= 0x1C0353D3;
			val = (val >> 0xE) | (val << 0x12);
			val -= 0x616C7E8;
			val = (val >> 0x16) | (val << 0xA);
			val -= 0x142A8D4E;
			val = (val >> 0xD) | (val << 0x13);
			val ^= 0x3CF39FF7;
			val -= 0x2761ACF8;
			val ^= 0x82262784;
			val -= 0x6E6AB812;
			val ^= 0x6BF05527;
			val += 0x5115F320;
			val ^= 0xAA876454;
			val = (val >> 0x1B) | (val << 0x5);
			val -= 0x34B76845;
			val = (val >> 0x1C) | (val << 0x4);
			val -= 0x52CAA8D6;
			val = (val >> 0x17) | (val << 0x9);
			return val;
		}

		inline uint32_t decrypt_0x1A0(uint32_t val) {
			val ^= 0x27515510;
			val += 0xD7EA20BE;
			val = (val << 0x13) | (val >> 0xD);
			return val;
		}

		inline uint32_t encrypt_0x1A0(uint32_t val) {
			val = (val >> 0x13) | (val << 0xD);
			val -= 0xD7EA20BE;
			val ^= 0x27515510;
			return val;
		}

		inline uint32_t decrypt_0x1A8(uint32_t val) {
			val ^= 0x943FB02C;
			val += 0x13A5687C;
			val ^= 0xFA6E1018;
			return val;
		}

		inline uint32_t encrypt_0x1A8(uint32_t val) {
			val ^= 0xFA6E1018;
			val -= 0x13A5687C;
			val ^= 0x943FB02C;
			return val;
		}

		inline uint32_t decrypt_0x1B0(uint32_t val) {
			val ^= 0x7C779F2D;
			val += 0x374F0BF;
			val = (val << 0x19) | (val >> 0x7);
			val += 0x1AEA4DEE;
			return val;
		}

		inline uint32_t encrypt_0x1B0(uint32_t val) {
			val -= 0x1AEA4DEE;
			val = (val >> 0x19) | (val << 0x7);
			val -= 0x374F0BF;
			val ^= 0x7C779F2D;
			return val;
		}

		inline uint32_t decrypt_0x1BC(uint32_t val) {
			val += 0x61015CE6;
			val = (val >> 0x1D) | (val << 0x3);
			val -= 0x311C6AC4;
			return val;
		}

		inline uint32_t encrypt_0x1BC(uint32_t val) {
			val += 0x311C6AC4;
			val = (val << 0x1D) | (val >> 0x3);
			val -= 0x61015CE6;
			return val;
		}

		inline uint32_t decrypt_0x1C4(uint32_t val) {
			val ^= 0x98D14BA6;
			val += 0xBF461E70;
			val = (val << 0x4) | (val >> 0x1C);
			val -= 0x1C6B4A98;
			return val;
		}

		inline uint32_t encrypt_0x1C4(uint32_t val) {
			val += 0x1C6B4A98;
			val = (val >> 0x4) | (val << 0x1C);
			val -= 0xBF461E70;
			val ^= 0x98D14BA6;
			return val;
		}

		inline uint32_t decrypt_0x1CC(uint32_t val) {
			val = (val << 0x1B) | (val >> 0x5);
			val += 0x9E13B502;
			val = (val << 0x12) | (val >> 0xE);
			return val;
		}

		inline uint32_t encrypt_0x1CC(uint32_t val) {
			val = (val >> 0x12) | (val << 0xE);
			val -= 0x9E13B502;
			val = (val >> 0x1B) | (val << 0x5);
			return val;
		}

		inline uint32_t decrypt_0x1D4(uint32_t val) {
			val += 0x83FE0E6A;
			val = (val << 0xD) | (val >> 0x13);
			val += 0x5AE8B10;
			val = (val << 0x1D) | (val >> 0x3);
			return val;
		}

		inline uint32_t encrypt_0x1D4(uint32_t val) {
			val = (val >> 0x1D) | (val << 0x3);
			val -= 0x5AE8B10;
			val = (val >> 0xD) | (val << 0x13);
			val -= 0x83FE0E6A;
			return val;
		}

		inline uint32_t decrypt_0x1DC(uint32_t val) {
			val += 0x40EB3A08;
			val = (val << 0x1C) | (val >> 0x4);
			val += 0xE441D7DC;
			val = (val << 0x16) | (val >> 0xA);
			return val;
		}

		inline uint32_t encrypt_0x1DC(uint32_t val) {
			val = (val >> 0x16) | (val << 0xA);
			val -= 0xE441D7DC;
			val = (val >> 0x1C) | (val << 0x4);
			val -= 0x40EB3A08;
			return val;
		}

		inline uint32_t decrypt_0x1E4(uint32_t val) {
			val = (val << 0xB) | (val >> 0x15);
			val ^= 0xD92BB47E;
			val = (val << 0x12) | (val >> 0xE);
			val -= 0x6DAAF7CE;
			return val;
		}

		inline uint32_t encrypt_0x1E4(uint32_t val) {
			val += 0x6DAAF7CE;
			val = (val >> 0x12) | (val << 0xE);
			val ^= 0xD92BB47E;
			val = (val >> 0xB) | (val << 0x15);
			return val;
		}

		inline uint32_t decrypt_0x1EC(uint32_t val) {
			val += 0x803FCB64;
			val = (val >> 0x1D) | (val << 0x3);
			val -= 0x781D6DCA;
			return val;
		}

		inline uint32_t encrypt_0x1EC(uint32_t val) {
			val += 0x781D6DCA;
			val = (val << 0x1D) | (val >> 0x3);
			val -= 0x803FCB64;
			return val;
		}

		inline uint32_t decrypt_0x1F4(uint32_t val) {
			val += 0xC98D8A6;
			val = (val << 0x7) | (val >> 0x19);
			val -= 0x2A57123C;
			return val;
		}

		inline uint32_t encrypt_0x1F4(uint32_t val) {
			val += 0x2A57123C;
			val = (val >> 0x7) | (val << 0x19);
			val -= 0xC98D8A6;
			return val;
		}

		inline uint32_t decrypt_0x1FC(uint32_t val) {
			val += 0x53D20EA8;
			val ^= 0x78B0F154;
			val += 0xA56477DA;
			val = (val << 0xC) | (val >> 0x14);
			return val;
		}

		inline uint32_t encrypt_0x1FC(uint32_t val) {
			val = (val >> 0xC) | (val << 0x14);
			val -= 0xA56477DA;
			val ^= 0x78B0F154;
			val -= 0x53D20EA8;
			return val;
		}

		inline uint32_t decrypt_0x204(uint32_t val) {
			val -= 0x12A69910;
			val ^= 0x92435E;
			val += 0x7F7FCEFE;
			val = (val >> 0x1D) | (val << 0x3);
			return val;
		}

		inline uint32_t encrypt_0x204(uint32_t val) {
			val = (val << 0x1D) | (val >> 0x3);
			val -= 0x7F7FCEFE;
			val ^= 0x92435E;
			val += 0x12A69910;
			return val;
		}

		inline uint32_t decrypt_0x20C(uint32_t val) {
			val += 0x19CC7040;
			val = (val << 0x14) | (val >> 0xC);
			val += 0x6F38D350;
			val = (val >> 0x1D) | (val << 0x3);
			return val;
		}

		inline uint32_t encrypt_0x20C(uint32_t val) {
			val = (val << 0x1D) | (val >> 0x3);
			val -= 0x6F38D350;
			val = (val >> 0x14) | (val << 0xC);
			val -= 0x19CC7040;
			return val;
		}

		inline uint32_t decrypt_0x214(uint32_t val) {
			val = (val << 0x6) | (val >> 0x1A);
			val ^= 0xFADFE64;
			val = (val << 0x13) | (val >> 0xD);
			val += 0x42A8BC9A;
			return val;
		}

		inline uint32_t encrypt_0x214(uint32_t val) {
			val -= 0x42A8BC9A;
			val = (val >> 0x13) | (val << 0xD);
			val ^= 0xFADFE64;
			val = (val >> 0x6) | (val << 0x1A);
			return val;
		}

	} // namespace PlayerWalkMovement

	inline uint32_t decrypt_fov(uint32_t val) {
		val = (val << 0x11) | (val >> 0xF);
		val += 0x87D432D0;
		val = (val << 0x17) | (val >> 0x9);
		return val;
	}

	inline uint32_t encrypt_fov(uint32_t val) {
		val = (val >> 0x17) | (val << 0x9);
		val -= 0x87D432D0;
		val = (val >> 0x11) | (val << 0xF);
		return val;
	}

} // namespace decrypts
