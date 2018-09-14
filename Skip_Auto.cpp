// Skip Auto Source

#include "DxLib.h"
#include "Utility.hpp"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include <array>
#include <thread>
#include <chrono>

//�I���t���O
extern int EndFlag;

//������`��ʒu
extern int DrawPointX, DrawPointY;

// ���ǃX�L�b�v/�X�L�b�v/�I�[�g�ϐ�
extern int SkipAndAutoFlag;

//���ǃX�L�b�v����p�ϐ�
std::array<std::int32_t, 15> SkipData;

//���ǃX�L�b�v
struct alignas(4) SkipData_t {
	//���ǃX�L�b�v����p�ϐ�
	std::array<std::int32_t, 15> SkipFlag;
};

//���ǃX�L�b�v�f�[�^��������
void SkipDataWrite(const std::int32_t& RouteNum) noexcept {
	SkipData[RouteNum - 1] = 1;
}

//���ǔ���
constexpr void SkipDataCheck(const std::int32_t& RouteNum) noexcept {
	if (SkipData[RouteNum - 1] == 1)
		SkipAndAutoFlag = 1;
	else
		SkipAndAutoFlag = 0;
}

//���ǃX�L�b�v�f�[�^�ǂݍ��݁���������
template<typename T, std::size_t N>
constexpr std::array<T, N> SkipDataGet(std::array<T, N> ReadData, std::array<T, N> WriteData) {

	auto var = std::begin(WriteData);

	for (auto it = std::begin(ReadData); it != std::end(ReadData); it++) {
		*var = *it;
		var++;
	}

	return WriteData;
}

//���ǃX�L�b�v�f�[�^�̓Ǎ�
int SkipDataLoad() noexcept {
	SkipData_t Data;

	FILE *Fp;
	fopen_s(&Fp, "DATA/SAVE/SKIP_READ.bat", "rb");
	if (nullptr == Fp) {
		return 0;
	}
	fread(&Data, sizeof(SkipData_t), 1, Fp);

	SkipData = SkipDataGet(Data.SkipFlag, SkipData);

	fclose(Fp);
	return 0;
}

//SKIP_READ SAVE�֐�
int SkipDataSave() noexcept {

	SkipData_t Data;

	for (auto&& d : Data.SkipFlag)
		d = 0;

	Data.SkipFlag = SkipDataGet(SkipData, Data.SkipFlag);

	FILE *Fp;
	fopen_s(&Fp, "DATA/SAVE/SKIP_READ.bat", "wb");//�o�C�i���t�@�C�����J��
	if (nullptr == Fp) {//�G���[���N������nullptr��Ԃ�
		return 0;
	}
	fwrite(&Data, sizeof(SkipData_t), 1, Fp); // SkipData_t�\���̂̒��g���o��
	fclose(Fp);
	return 0;
}

// ���ǃX�L�b�v/�X�L�b�v/�I�[�g�؂�ւ��֐�
void SkipAndAutoTask(const std::int32_t& Num, const int Flag) noexcept {

	std::array<std::string, 4> Message = { "�I�[�g/�X�L�b�v���~���܂����H" , "�X�L�b�v�����s���܂����H",
											"�I�[�g�����s���܂����H", "���ǃX�L�b�v�����s���܂����H" };

	for (std::int32_t i = 0; i < 4; i++) {
		if (Num == i && Num == 3) {
			if (IDYES == MessageBoxYesNo(Message[i].c_str())) {
				SkipDataCheck(Flag);
			}
			WaitTime();
		} else if (Num == i) {
			if (IDYES == MessageBoxYesNo(Message[i].c_str())) {
				SkipAndAutoFlag = i;
			}
			WaitTime();
		}
	}

	EndFlag = Flag;
	DrawPointX = 0;
	DrawPointY = 0;
}
