
int func_null(){
	return 0;
}

struct GUID {
  unsigned long Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[8];
};

template<class T> constexpr GUID const& __mingw_uuidof();

class ID3D12Object;

#define WINADAPTER_IID(InterfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
inline constexpr GUID InterfaceName##_IID = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }; \
template <> constexpr GUID const& __mingw_uuidof< InterfaceName >() \
{ \
    return InterfaceName##_IID; \
}
WINADAPTER_IID(ID3D12Object, 0xc4fec28f, 0x7966, 0x4e95, 0x9f, 0x94, 0xf4, 0x31, 0xcb, 0x56, 0xc3, 0xb8)

