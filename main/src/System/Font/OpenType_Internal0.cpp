module OpenType_Internal0;
import Module;
import IO;
import Drawing;
import <iostream>;
using namespace System;

void CFF1Index::PrintNameIndex() const noexcept {
	std::cout << cout_green << "Name Index Begin" << cout_reset << std::endl;
	for (uint16_t i = 0; i < count; ++i) {
		CFF1IndexData data = GetData(i);
		std::cout << i << ": " << String(reinterpret_cast<const char8_t*>(data.data), data.size) << std::endl;
	}
	std::cout << cout_green << "Name Index End" << cout_reset << std::endl;
}
void CFF1Index::PrintDICTIndex() const noexcept {
	std::cout << cout_green << "DICT Index Begin" << cout_reset << std::endl;
	CFF1IndexData data = GetData(0);
	for (DICTKeyValue<number_c>& x : data.GetDICTKeyValues<number_c>()) {
		std::cout << "Operator: " << GetDICTOperatorName(x.opr) << ", Value: ";
		for (size_t i = 0; i < x.count; ++i) {
			std::cout << x.data[i] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << cout_green << "DICT Index End" << cout_reset << std::endl;
}
void CFF1Index::PrintStringIndex() const noexcept {
	std::cout << cout_green << "String Index Begin" << cout_reset << std::endl;
	for (uint16_t i = 0; i < count; ++i) {
		CFF1IndexData data = GetData(i);
		std::cout << (391 + i) << ": " << String(reinterpret_cast<const char8_t*>(data.data), data.size) << std::endl;
	}
	std::cout << cout_green << "String Index End" << cout_reset << std::endl;
}
void CFF1Index::PrintSubrIndex() const noexcept {}
void CFF1Index::PrintCharstringIndex() const noexcept {}
void CFF1Index::PrintLocalSubrIndex() const noexcept {}
void CFF1Index::PrintFDArrayIndex() const noexcept {
	std::cout << cout_green << "FDArray Index Begin" << cout_reset << std::endl;
	for (uint16_t i = 0; i < count; ++i) {
		std::cout << "FDArray Index " << i << ":" << std::endl;
		CFF1IndexData data = GetData(i);
		for (DICTKeyValue<number_c>& x : data.GetDICTKeyValues<number_c>()) {
			std::cout << "Operator: " << GetDICTOperatorName(x.opr) << ", Value: ";
			for (size_t i = 0; i < x.count; ++i) {
				std::cout << x.data[i] << " ";
			}
			std::cout << std::endl;
		}
	}
	std::cout << cout_green << "FDArray Index End" << cout_reset << std::endl;	
}


void Type2CharstringParser::Parse() noexcept {
	for (uint32_t i = 0; i < m_size; ++i) {
		if (m_data[i] == 28 || m_data[i] >= 32) {
			fixed32_c tmp;
			i = GetType2Value(tmp, m_data, i, m_size - 1) - 1;
			m_args[m_argCount++] = static_cast<int16_t>(tmp);
		}
		else {
			Type2Operator opr;
			if (m_data[i] != 12) opr = static_cast<Type2Operator>(m_data[i]);
			else {
				uint16_t lhs = static_cast<uint16_t>(m_data[i]) << 8;
				i += 1;
				uint16_t rhs = m_data[i];
				opr = static_cast<Type2Operator>(lhs | rhs);
			}
			if (opr == Type2Operator::Callsubr || opr == Type2Operator::Callgsubr) {
				int16_t subr = m_args[m_argCount - 1];
				m_args[--m_argCount] = 0;
				CFF1IndexData subrData = (opr == Type2Operator::Callsubr ? m_local : m_global).GetSubr(subr);
				const uint8_t* currentData = m_data;
				const uint32_t currentSize = m_size;
				m_data = subrData.data;
				m_size = subrData.size;
				Parse();
				m_data = currentData;
				m_size = currentSize;
			}
			else {
				if (!m_widthDelta && m_argCount) {
					m_widthDelta = m_args[0];
					for (uint8_t j = 1; j < m_argCount; ++j) m_args[j - 1] = m_args[j];
					--m_argCount;
				}
				switch (opr) {
				case Type2Operator::Hstem:
				case Type2Operator::Hstemhm:
					SetHStem();
					break;
				case Type2Operator::Vstem:
				case Type2Operator::Vstemhm:
					SetVStem();
					break;
				case Type2Operator::Hintmask:
				case Type2Operator::Cntrmask:
					if (m_argCount) {
						if (!m_hStemCountX2) SetHStem();
						else SetVStem();
					}
					if (opr == Type2Operator::Hintmask) {
						for (uint8_t j = 0, s = GetMaskSize(); j < s; ++j) {
							if (i + 1 == m_size) break;
							m_currentHintMask[j] = m_data[++i];
						}
					}
					else {
						for (uint8_t j = 0, s = GetMaskSize(); j < s; ++j) {
							if (i + 1 == m_size) break;
							m_currentCntrMask[j] = m_data[++i];
						}
					}
					break;
				default:
					CallPathOperator(opr);
					break;
				}
			}
		}
	}
}
